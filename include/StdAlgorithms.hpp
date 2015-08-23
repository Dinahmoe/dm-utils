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
 

//
//  StdAlgorithms.hpp
//  DMAF
//
//  Created by Alessandro Saccoia on 11/2/12.
//
//

#ifndef DMAF_StdAlgorithms_hpp
#define DMAF_StdAlgorithms_hpp

#include <map>

namespace dinahmoe {


template<class ContainerType>
inline void clearCointainerOfPointers(ContainerType& container_) {
  typename ContainerType::iterator it = container_.begin();
  while (it != container_.end()) {
    delete *it;
    ++it;
  }
  container_.clear();
}

template <typename SetType>
bool overwriteInSet(SetType& m, const typename SetType::value_type& v) {
   std::pair<typename SetType::iterator, bool> result =
     m.insert(v);
   if (!result.second) {
     typename SetType::value_type& theIt = const_cast<typename SetType::value_type&>(*(result.first));
     theIt = v;
   }
   return result.second;
}


} // DMAF

#endif
