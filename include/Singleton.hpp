/*
 *  Singleton.hpp
 *
 *  Created by Alessandro Saccoia on 22/01/11.
 *
 *  Refactored inerhently thread safe for c++11 on 12/18/2014:
 *
 *  If control enters the declaration concurrently while the variable is being
 *  initialized, the concurrent execution shall wait for completion of the initialization.
 *   —§6.7 [stmt.dcl] p4
 *
 */
 
#ifndef _SINGLETON_HPP
#define _SINGLETON_HPP

namespace dinahmoe {
namespace utils {

template <class T>
struct Singleton {
  static T& instance() {
    static T s;
    return s;
  }
};


} // utils
} // dinahmoe

#endif //_SINGLETON_HPP