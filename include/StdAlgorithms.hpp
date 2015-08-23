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
