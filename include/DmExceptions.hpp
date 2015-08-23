//
//  DmExceptions.hpp
//
//  Created by Alessandro Saccoia on 3/27/15.
//
//

#ifndef _DmExceptions_hpp
#define _DmExceptions_hpp

#include <exception>
#include <stdexcept>

#define dm_runtime_error(format_, ...) \
 { \
   char buf[256]; \
   sprintf(buf, format_, __VA_ARGS__); \
   throw std::runtime_error(buf); \
 }
#define dm_invalid_argument(format_, ...) \
 { \
   char buf[256]; \
   sprintf(buf, format_, __VA_ARGS__); \
   throw std::invalid_argument(buf); \
 }

#ifndef NDEBUG
#define dm_print_debug(str) { std::cout << str << std::endl; }
#else
#define dm_print_debug(str) {;}
#endif

#endif
