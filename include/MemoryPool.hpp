//
//  MemoryPool.hpp
//  dinahmoe/utils
//
//  Created by Alessandro Saccoia on 12/11/14.
//
//

#ifndef _MemoryPool_hpp
#define _MemoryPool_hpp

#include <list>
#include <cassert>
#include <iostream>
#include <stdlib.h>
#include "NullMutex.hpp"

namespace dinahmoe {
namespace utils {

template <class T, class MUTEX = synchro::NullMutex, int INITIAL_SIZE = 0>
class MemoryPool : public MUTEX {
public:
  struct element {
    bool free;
    void* itemPtr;
  };
  
  MemoryPool() {
    for (int i = 0; i < INITIAL_SIZE; ++i) {
      m_nodesList.emplace_back();
      m_nodesList.back().free = true;
      m_nodesList.back().itemPtr = malloc(sizeof(T));
    }
  }
  
  ~MemoryPool() {
      //todo we are leaking
//    for (auto& node: m_nodesList) {
//      if (!node.free) ((T*)node.itemPtr)->T::~T();
//    }
  }
  
  void* acquire() {
    MUTEX::lock();
    for (auto& node: m_nodesList) {
      if (node.free) {
        node.free = false;
        MUTEX::unlock();
        return node.itemPtr;
      }
    }
    m_nodesList.emplace_back();
    m_nodesList.back().free = false;
    m_nodesList.back().itemPtr = malloc(sizeof(T));
    void* toReturn = m_nodesList.back().itemPtr;
    MUTEX::unlock();
    return toReturn;
  }

  void release(T* ptr_) {
    MUTEX::lock();
    for (auto& node: m_nodesList) {
      if (node.itemPtr == ptr_) {
        node.free = true;
        ((T*)node.itemPtr)->T::~T();
        MUTEX::unlock();
        return;
      }
    }
    assert(false);
  }
  
  void releaseWithExternalLock(T* ptr_) {
    for (auto& node: m_nodesList) {
      if (node.itemPtr == ptr_) {
        node.free = true;
        ((T*)node.itemPtr)->T::~T();
        return;
      }
    }
    assert(false);
  }
  
  bool tryRelease(T* ptr_) {
    if (!MUTEX::try_lock()) return false;
    for (auto& node: m_nodesList) {
      if (node.itemPtr == ptr_) {
        node.free = true;
        ((T*)node.itemPtr)->T::~T();
        MUTEX::unlock();
        return true;
      }
    }
    assert(false);
  }
  
  size_t size() {
    MUTEX::lock();
    size_t returnMe = m_nodesList.size();
    MUTEX::unlock();
    return returnMe;
  }
  
  size_t freeElements() {
    MUTEX::lock();
    size_t returnMe = 0;
    for (auto& node: m_nodesList) {
      if (node.free) {
        ++returnMe;
      }
    }
    MUTEX::unlock();
    return returnMe;
  }

private:
  std::list<element> m_nodesList;
};

}};

#endif
