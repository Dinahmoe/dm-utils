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
 
/*! \file       Threading/LockingQueue.hpp
    \brief      Declaration of class LockingQueue.
    \author     Alessandro Saccoia
*/

#ifndef LOCKINGQUEUE_HPP_
#define LOCKINGQUEUE_HPP_

#include "NullMutex.hpp"
#include <mutex>

namespace dinahmoe {
namespace Threading {

//! \brief  Queue using a locking strategy for access from multiple threads.
//! \param  tItemType Type of object to be put in the queue.
//! \param  tMutexType Type of mutex (modeled after boost::mutex concept).
template<typename tItemType, typename tMutexType = synchro::NullMutex>
class LockingQueue
{
public:
    //! Item type.
    typedef tItemType ItemType;

    //! Type of mutex.
    typedef tMutexType MutexType;

    //  ------------------------------------------------------------------------

    //! \brief  Default constructor.
    LockingQueue()
    :
        mpHead(0),
        mpTail(0),
        mSize(0)
    {}

    //! \brief  Destructor.
    virtual ~LockingQueue()
    {}

    //  ------------------------------------------------------------------------

    virtual void Push(ItemType* apObj)
    {
        if (apObj == 0) return;

        typename std::lock_guard<MutexType> vLock(mMutex);

        apObj->mpNext = 0;

        if (mpTail == 0)
        {
            mpTail = apObj;
            mpHead = mpTail;
        }
        else
        {
            mpTail->mpNext = apObj;
            mpTail = apObj;
        }

        ++mSize;
    }

    virtual ItemType* Pop()
    {
        typename std::lock_guard<MutexType> vLock(mMutex);

        if ((mpHead == mpTail) && (mpTail == 0))
        {
            return 0;
        }

        ItemType* vpRet = mpHead;

        mpHead = (ItemType*)mpHead->mpNext;

        if (mpTail == vpRet)
        {
            mpTail = (ItemType*)mpTail->mpNext;
        }

        --mSize;
        return vpRet;
    }

    virtual ItemType* PopAll()
    {
        typename std::lock_guard<MutexType> vLock(mMutex);

        ItemType* vpRet = mpHead;
        mpHead = mpTail = 0;
        mSize = 0;

        return vpRet;
    }

    virtual bool IsEmpty()
    {
        typename std::lock_guard<MutexType> vLock(mMutex);
        return mSize == 0;
    }

    virtual unsigned int GetSize()
    {
        typename std::lock_guard<MutexType> vLock(mMutex);
        return mSize;
    }
    
    virtual void Clear() {
        typename std::lock_guard<MutexType> vLock(mMutex);
        
        ItemType* theItem = mpHead;
        ItemType* lastItem;
        while (theItem != 0) {
          lastItem = theItem;
          theItem = theItem->mpNext;
          delete lastItem;
        }
        mpHead = mpTail = 0;
        mSize = 0;
    }

    //  ------------------------------------------------------------------------

private:
    //! Mutex used to prevent race conditions while manipulating the queue.
    MutexType mMutex;

    //! Head of the queue.
    ItemType* mpHead;

    //! Tail of the queue.
    ItemType* mpTail;

    //! Size of the queue.
    unsigned int mSize;
};

} // namespace Threading
} // namespace dinahmoe

#endif // LOCKINGQUEUE_HPP_
