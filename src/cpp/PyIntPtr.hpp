#ifndef __PYEXT__PDF_TIN__PYINTPTR_HPP__
#define __PYEXT__PDF_TIN__PYINTPTR_HPP__

#include <PyObjectPtr.hpp>
#include <stdint.h>

class PyIntPtr : public PyObjectPtr {
public:
  PyIntPtr() : PyObjectPtr() { }
  explicit PyIntPtr(long v) : PyObjectPtr(PyLong_FromLong(v)) { }
  PyIntPtr(const PyIntPtr&) = default;
  PyIntPtr(PyIntPtr&&) = default;

  long value() const { return get() ? PyLong_AsLong(get()) : 0; }
  
  PyIntPtr& operator=(const PyIntPtr&) = default;
  PyIntPtr& operator=(PyIntPtr&&) = default;
};

#endif

