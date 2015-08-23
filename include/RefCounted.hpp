
#ifndef __REFCOUNTED_HPP__
#define __REFCOUNTED_HPP__

#include <functional>
#include <cstddef>

namespace dinahmoe {

#define DM_FORCEINLINE inline __attribute__((__always_inline__))

template <class T>
struct SingleThreadedPtr {

	int DM_FORCEINLINE getRefCount() {
		return m_refCount;
	}

	SingleThreadedPtr() : m_refCount(1) { }

	void DM_FORCEINLINE incrementRefCount() {
		++m_refCount;
	}

	void DM_FORCEINLINE decrementRefCount() {
		--m_refCount;
    if (!m_refCount) {
      delete static_cast<T*>(this);
    }
	}

	int m_refCount;
};


/** Reference counted class, the template class concept need to declare
 *  incrementRefCount and decrementRefCount
 */
template <class T>
class RefCounted {
public:
  DM_FORCEINLINE RefCounted() :
    ptr(0) {
  }

  DM_FORCEINLINE RefCounted(const RefCounted<T>& rhs) :
    ptr(rhs.ptr) {
		if (ptr != 0)
			ptr->incrementRefCount();
	}

  DM_FORCEINLINE RefCounted<T>& operator=(const RefCounted<T>& rhs) {
    ptr = rhs.ptr;
		if (ptr != 0)
			ptr->incrementRefCount();
    return *this;
  }

	DM_FORCEINLINE RefCounted(T* ptr_) :
    ptr(ptr_) {
		if (ptr != 0)
			ptr->incrementRefCount();
	}

	DM_FORCEINLINE ~RefCounted() {
    if (ptr != 0)
      ptr->decrementRefCount();
	}

  DM_FORCEINLINE T* operator->() const {
    return ptr;
  }

  // Ale TODO hating this but I have no time right now.
  // just remember not to ever use this in wrong places for now!
  DM_FORCEINLINE void operator=(const T* ptr_) {
    ptr = ptr_;
  }


  DM_FORCEINLINE T* get() const {
    return ptr;
  }

private:
  T* ptr;
};

template<typename T>
struct RefCountedHash :
  public std::unary_function<RefCounted<T>, std::size_t> {

  size_t operator()(const RefCounted<T> & ptr) const
  {
      return ((size_t) ptr.get()) / sizeof(T);
  }


  bool equal_to(const RefCounted<T> & left, const RefCounted<T> & right)
  {
      return left.get() == right.get();
  }

};

} // DMAF


#endif
