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

//  Created by Alessandro Saccoia on 9/5/14.

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
