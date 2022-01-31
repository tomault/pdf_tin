#include "PyBytesPtr.hpp"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>

static PyObject* createPyBytes(const uint8_t* bytes, size_t n) {
  if (!bytes) {
    return nullptr;
  } else {
    PyObject* s = PyBytes_FromStringAndSize((const char*)bytes, n);
    if (!s) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to create bytes object");
    }
    return s;
  }
}

PyBytesPtr::PyBytesPtr(const uint8_t* bytes, size_t n)
    : PyObjectPtr(createPyBytes(bytes, n)) {
}

