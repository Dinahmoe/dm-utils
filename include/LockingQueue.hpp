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
