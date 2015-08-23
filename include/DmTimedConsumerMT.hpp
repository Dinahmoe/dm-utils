  //
//  DmTimedConsumerMT.hpp
//  Dmaf OSC Utilities
//
//  Created by Alessandro Saccoia on 9/5/14.
//
//

#ifndef Dmaf_OSC_Utilities_DmTimedConsumerMT_hpp
#define Dmaf_OSC_Utilities_DmTimedConsumerMT_hpp

#include <thread>
#include <mutex>
#include <list>
#include <chrono>

namespace dinahmoe {
namespace utils {

class DmTimedConsumerMT {
public:
  DmTimedConsumerMT(unsigned int periodMs_) : m_periodMs(periodMs_), m_isRunning(false) {
  
  }
  virtual ~DmTimedConsumerMT() {
    Stop();
  }
  virtual void Start() {
    m_isRunning = true;
    m_thread = std::thread(&DmTimedConsumerMT::ThreadFunction, this);
  }
  virtual void Stop() {
    if (m_isRunning) {
      m_isRunning = false;
      m_thread.join();
    }
  }
  
  bool TryLock() {
    return m_mutex.try_lock();
  }
  
  void Lock() {
    return m_mutex.lock();
  }
  
  void Unlock() {
    return m_mutex.unlock();
  }
  
  void ThreadFunction() {
    while (m_isRunning) {
      m_iterationStartTime = std::chrono::system_clock::now();
      m_mutex.lock();
      TimedOperation();
      m_mutex.unlock();
      m_sleepTime = m_periodMs - (std::chrono::system_clock::now() - m_iterationStartTime);
      if (m_sleepTime > std::chrono::system_clock::duration::zero()) {
        std::this_thread::sleep_for(m_sleepTime);
      }
    }
  }
  
  virtual void TimedOperation() = 0;
  
private:
  std::chrono::milliseconds m_periodMs;
  volatile bool m_isRunning;
  std::thread m_thread;
  std::mutex m_mutex;
  std::chrono::system_clock::time_point m_iterationStartTime;
  std::chrono::system_clock::duration m_sleepTime;
};

}}



#endif
