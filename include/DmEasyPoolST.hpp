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

//  Created by Alessandro Saccoia on 9/17/14.

#ifndef Dmaf_OSC_Utilities_DmEasyPoolST_hpp
#define Dmaf_OSC_Utilities_DmEasyPoolST_hpp

#include <list>
#include <exception>
#include <cassert>
#include "Log.h"

namespace dinahmoe {
namespace utils {

template <class T>
class DmEasyPoolST {
public:
  void initialize(Log* log_,
    size_t initialSize_,
    int resizingRatio_ = 2,
    float fillPercentageBeforeResize_ = 0.75F) {
    assert(initialSize_ > 2);
    assert(resizingRatio_ > 1.0F);
    assert(fillPercentageBeforeResize_ < 1.0F);
    m_log = log_;
    m_currentSize = initialSize_;
    m_fullSize = initialSize_;
    m_resizingRatio = resizingRatio_;
    m_guardPercentage = fillPercentageBeforeResize_;
    m_pool.resize(m_fullSize);
  }

  typename std::list<T>::iterator pushBackInList(std::list<T>& list_, size_t N = 1) {
    typename std::list<T>::iterator toReturn;
    typename std::list<T>::iterator poolEnd = m_pool.begin();
    for (int i = 0; i < N; ++i) ++poolEnd;
    if (m_currentSize < N) {
      size_t addedSize = size_t(m_fullSize * (m_resizingRatio - 1.0));
      dm_debug_log(m_log, Log::Warning, "The pool of %s is empty, resizing from %i to %i", typeid(T).name(), m_fullSize, m_fullSize + addedSize);
      m_fullSize += addedSize;
      m_pool.resize(addedSize);
      m_currentSize = addedSize;
    }
    list_.splice(list_.end(), m_pool, m_pool.begin(), poolEnd);
    m_currentSize-= N;
    toReturn = list_.end();
    for (int i = 0; i < N; ++i) --toReturn;
    return toReturn;
  }

  void returnToPool(std::list<T>& list_, typename std::list<T>::iterator sit, typename std::list<T>::iterator eit) {
    size_t nItems = std::distance(sit, eit);
    if (nItems > 0) {
      m_currentSize += nItems;
      m_pool.splice(m_pool.end(), list_, sit, eit);
    }
  }
//
//  void checkResize() {
//    if (needsResize) {
//      m_fullSize = m_resizingRatio * m_fullSize;
//      m_guardSize(initialSize_ * fillPercentageBeforeResize_),
//      needsResize = true;
//    }
//  }

  volatile bool needsResize;

private:
  std::list<T> m_pool;
  size_t m_currentSize;
  size_t m_fullSize;
  size_t m_guardSize;
  float m_guardPercentage;
  float m_resizingRatio;
  Log* m_log;
};

}}

#endif
