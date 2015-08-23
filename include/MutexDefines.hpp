 /*!
 * \file MutexDefines.hpp
 *
 * \author Alessandro Saccoia
 * \date 12/10/13
 *
 */

namespace dinahmoe {
namespace synchro {

#ifndef DMAF_REALTIME
  #define DMAF_REALTIME 1
#endif

// this is really bad actually. it's repeated in AudioDriver :(
// would need to templetize everything?
#if DMAF_REALTIME
  typedef std::mutex mutex_t;
  typedef std::recursive_mutex recursive_mutex_t;
#else
  typedef dinahmoe::synchro::NullMutex mutex_t;
  typedef dinahmoe::synchro::NullMutex recursive_mutex_t;
#endif

}
}