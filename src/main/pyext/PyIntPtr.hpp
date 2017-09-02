#ifndef __PYEXT__PDF_TIN__PYINTPTR_HPP__
#define __PYEXT__PDF_TIN__PYINTPTR_HPP__

#include <PyObjectPtr.hpp>
#include <stdint.h>

class PyIntPtr : public PyObjectPtr {
public:
  PyIntPtr() : PyObjectPtr() { }
  explicit PyIntPtr(long v) : PyObjectPtr(addRef(PyInt_FromLong(v))) { }
  PyIntPtr(const PyIntPtr&) = default;
  PyIntPtr(PyIntPtr&&) = default;

  long value() const { return *this ? PyInt_AS_LONG(get()) : 0; }
  
  PyIntPtr& operator=(const PyIntPtr&) = default;
  PyIntPtr& operator=(PyIntPtr&&) = default;
};

#endif

