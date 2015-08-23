//
//  DmGarbageCollector.hpp
//  Dmaf OSC Utilities
//
//  Created by Alessandro Saccoia on 9/3/14.
//
//

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
