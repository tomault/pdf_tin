#ifndef __PYEXT__PDF_TIN__PYFLOATPTR_HPP__
#define __PYEXT__PDF_TIN__PYFLOATPTR_HPP__

#include <PyObjectPtr.hpp>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

class PyFloatPtr : public PyObjectPtr {
public:
  PyFloatPtr() : PyObjectPtr() { }
  PyFloatPtr(PyObject* o) : PyObjectPtr(o) { }
  explicit PyFloatPtr(double v) : PyObjectPtr(PyFloat_FromDouble(v)) { }
  PyFloatPtr(const PyFloatPtr&) = default;
  PyFloatPtr(PyFloatPtr&&) = default;

  double value() const { return get() ? PyFloat_AS_DOUBLE(get()) : 0.0 }
  
  PyFloatPtr(const PyFloatPtr&) = default;
  PyFloatPtr(PyFloatPtr&&) = default;

  static PyFloatPtr none() { return PyFloatPtr(addRef(Py_None)); }
};

#endif
