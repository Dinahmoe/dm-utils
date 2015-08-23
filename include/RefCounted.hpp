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
 

#ifndef __REFCOUNTED_HPP__
#define __REFCOUNTED_HPP__

#include <functional>
#include <cstddef>

namespace dinahmoe {

#define DM_FORCEINLINE inline __attribute__((__always_inline__))

template <class T>
struct SingleThreadedPtr {

	int DM_FORCEINLINE getRefCount() {
		return m_refCount;
	}

	SingleThreadedPtr() : m_refCount(1) { }

	void DM_FORCEINLINE incrementRefCount() {
		++m_refCount;
	}

	void DM_FORCEINLINE decrementRefCount() {
		--m_refCount;
    if (!m_refCount) {
      delete static_cast<T*>(this);
    }
	}

	int m_refCount;
};


/** Reference counted class, the template class concept need to declare
 *  incrementRefCount and decrementRefCount
 */
template <class T>
class RefCounted {
public:
  DM_FORCEINLINE RefCounted() :
    ptr(0) {
  }

  DM_FORCEINLINE RefCounted(const RefCounted<T>& rhs) :
    ptr(rhs.ptr) {
		if (ptr != 0)
			ptr->incrementRefCount();
	}

  DM_FORCEINLINE RefCounted<T>& operator=(const RefCounted<T>& rhs) {
    ptr = rhs.ptr;
		if (ptr != 0)
			ptr->incrementRefCount();
    return *this;
  }

	DM_FORCEINLINE RefCounted(T* ptr_) :
    ptr(ptr_) {
		if (ptr != 0)
			ptr->incrementRefCount();
	}

	DM_FORCEINLINE ~RefCounted() {
    if (ptr != 0)
      ptr->decrementRefCount();
	}

  DM_FORCEINLINE T* operator->() const {
    return ptr;
  }

  // Ale TODO hating this but I have no time right now.
  // just remember not to ever use this in wrong places for now!
  DM_FORCEINLINE void operator=(const T* ptr_) {
    ptr = ptr_;
  }


  DM_FORCEINLINE T* get() const {
    return ptr;
  }

private:
  T* ptr;
};

template<typename T>
struct RefCountedHash :
  public std::unary_function<RefCounted<T>, std::size_t> {

  size_t operator()(const RefCounted<T> & ptr) const
  {
      return ((size_t) ptr.get()) / sizeof(T);
  }


  bool equal_to(const RefCounted<T> & left, const RefCounted<T> & right)
  {
      return left.get() == right.get();
  }

};

} // DMAF


#endif
