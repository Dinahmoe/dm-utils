/*
 * Copyright (c) 2015, Dinahmoe. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
/*
 * RingBuffer.hpp
 * Ringbuffer implementation (not thread safe)
 * Differently from the common implementations, this RingBuffer
 * doesn't advance automatically the read pointer but expects this
 * to be done from the user through the advance method, to have
 * overlap.
 *  
 */

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <cstring>
#include <cassert>

namespace dinahmoe {

template <typename T>
class RingBuffer
{
public:
	RingBuffer(size_t n = 8192);
	
  virtual ~RingBuffer();
	
	inline size_t getSize() const;
  
	inline void resize(size_t n);
	
  inline size_t getReadSpace() const;

  inline size_t getWriteSpace() const;

  inline size_t read(T* destination, size_t n, bool advance = false);

  inline size_t readInterleaved(T* destination, size_t n);

  inline size_t advance(size_t n);

  inline size_t write(const T *source, size_t n);
	
	inline size_t zero(size_t n);
    
  inline void reset() {
      m_writePosition = m_readPosition = 0;
  }
	
private:

    RingBuffer(const RingBuffer &);
    RingBuffer &operator=(const RingBuffer &);
    
	T      *m_buffer;
    size_t  m_writePosition;
    size_t  m_readPosition;
    size_t  m_size;	
};


template <typename T>
RingBuffer<T>::RingBuffer(size_t n) :
    m_buffer(new T[n + 1]),
    m_writePosition(0),
    m_readPosition(0),
    m_size(n + 1)
{
}

template <typename T>
RingBuffer<T>::~RingBuffer()
{
    delete[] m_buffer;
}

template <typename T>
void RingBuffer<T>::resize(size_t n)  {
  if (m_buffer) {
    delete [] m_buffer;
  }
  m_buffer = new T[n + 1];
  m_writePosition = 0;
  m_readPosition = 0;
  m_size = n + 1;
}


template <typename T>
size_t RingBuffer<T>::getSize() const
{
    return m_size - 1;
}

template <typename T>
size_t RingBuffer<T>::getReadSpace() const
{
    size_t space = 0;

    if (m_writePosition > m_readPosition) space = m_writePosition - m_readPosition;
    else space = ((m_writePosition + m_size) - m_readPosition) % m_size;

    return space;
}

template <typename T>
size_t
RingBuffer<T>::getWriteSpace() const
{
	//[0123R567W9]
	size_t space = (m_readPosition + m_size - m_writePosition - 1) % m_size;

    return space;
}

template <typename T>
size_t RingBuffer<T>::read(T *destination, size_t n, bool advance)
{
    size_t available = getReadSpace();
    if (n > available) 
    {
      memset(destination, 0, (n - available) * sizeof(T));
      destination = destination + n - available;
      n = available;
    }
    if (n == 0) return n;

	//[0R234567W9*10*]
	//here = 11-1
    size_t here = m_size - m_readPosition;
    if (here >= n) 
    {
		memcpy(destination, m_buffer + m_readPosition, n * sizeof(T));
    } 
    else 
    {
		memcpy(destination, m_buffer + m_readPosition, here * sizeof(T));
		memcpy(destination + here, m_buffer, (n - here) * sizeof(T));
    }

    // commented out to manage overlap  
    if (advance) m_readPosition = (m_readPosition + n) % m_size;

    return n;
}

template <typename T>
size_t RingBuffer<T>::readInterleaved(T *destination, size_t n)
{
    size_t available = getReadSpace();
    if (n > available) 
    {
      assert(false);
    }
    if (n == 0) return n;

	//[0R234567W9*10*]
	//here = 11-1
    size_t here = m_size - m_readPosition;
    if (here >= n) 
    {
      for (int i = 0, ctr = 0; i < n; ++i) {
        destination[ctr++] = *(m_buffer + m_readPosition + i);
        destination[ctr++] = T(0);
      }
    } 
    else 
    {
      for (int i = 0, ctr = 0; i < here; ++i) {
        destination[ctr++] = *(m_buffer + m_readPosition + i);
        destination[ctr++] = T(0);
      }
      for (int i = 0, ctr = 0; i < n - here; ++i) {
        destination[ctr++] = *(m_buffer + i);
        destination[ctr++] = T(0);
      }
    }

    // commented out to manage overlap  
    //m_readPosition = (m_readPosition + n) % m_size;

    return n;
}


template <typename T>
size_t RingBuffer<T>::advance(size_t n)
{
    m_readPosition = (m_readPosition + n) % m_size;
    return n;
}


template <typename T>
size_t RingBuffer<T>::write(const T *source, size_t n)
{
    size_t available = getWriteSpace();
    
    if (n > available) 
    {
    assert(false);
		n = available;
    }
    if (n == 0) return n;

    size_t here = m_size - m_writePosition;
    if (here >= n) 
    {
		memcpy(m_buffer + m_writePosition, source, n * sizeof(T));
    } 
    else 
    {
		memcpy(m_buffer + m_writePosition, source, here * sizeof(T));
		memcpy(m_buffer, source + here, (n - here) * sizeof(T));
    }

    m_writePosition = (m_writePosition + n) % m_size;

    return n;
}

template <typename T>
size_t
RingBuffer<T>::zero(size_t n)
{
    size_t available = getWriteSpace();
    if (n > available) 
    {
		n = available;
    }
    if (n == 0) return n;

    size_t here = m_size - m_writePosition;
    if (here >= n)
    {
		memset(m_buffer + m_writePosition, 0, n * sizeof(T));
    } 
    else
    {
		memset(m_buffer + m_writePosition, 0, here * sizeof(T));
		memset(m_buffer, 0, (n - here) * sizeof(T));
    }

    m_writePosition = (m_writePosition + n) % m_size;
    return n;
}

} // dinahmoe

#endif // _RINGBUFFER_H_
