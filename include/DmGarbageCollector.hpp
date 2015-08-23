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

//  Created by Alessandro Saccoia on 9/3/14.

#ifndef Dmaf_OSC_Utilities_DmGarbageCollector_hpp
#define Dmaf_OSC_Utilities_DmGarbageCollector_hpp

#include <list>
#include "DmTimedConsumerMT.hpp"

namespace dinahmoe {
namespace utils {

template <class T>
class DmGarbageCollector : public DmTimedConsumerMT {
public:
  typedef T* PtrType;
  DmGarbageCollector(unsigned int periodMs_) : DmTimedConsumerMT(periodMs_) {
  }
  
  void PushGarbage(typename std::list<PtrType>::iterator garbageBegin, typename std::list<PtrType>::iterator garbageEnd) {
    m_garbageList.insert(m_garbageList.begin(), garbageBegin, garbageEnd);
  }
  
  void TimedOperation() {
    for (auto item: m_garbageList) {
      delete item;
    }
    m_garbageList.clear();
  }
private:
  std::list<PtrType> m_garbageList;
};

}}


#endif
