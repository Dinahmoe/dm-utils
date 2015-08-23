/*! \file       NullMutex.hpp
    \brief      Declaration of class NullMutex.
    \author     Alessandro Saccoia
*/

#ifndef NULLMUTEX_HPP_
#define NULLMUTEX_HPP_

// null implementation of the http://en.cppreference.com/w/cpp/concept/Lockable concept

namespace dinahmoe {
namespace synchro {

struct NullMutex
{
   void lock() {}
   void unlock() noexcept {}
   bool try_lock() { return true; }
};

} // synchro
} // dinahmoe


#endif
