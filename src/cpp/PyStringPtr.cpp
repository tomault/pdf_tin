#include "PyStringPtr.hpp"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* createPyString(const char* text) {
  if (!text) {
    return nullptr;
  } else {
    PyObject* s = PyUnicode_FromString(text);
    if (!s) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to create unicode object");
    }
    return s;
  }
}
PyStringPtr::PyStringPtr(const char* text)
    : PyObjectPtr(createPyString(text)) {
  // Intentionally left blank
}

const char* PyStringPtr::text() const {
  static const char* EMPTY_STRING = "";
  return get() ? PyUnicode_AsUTF8(get()) : EMPTY_STRING;
}
