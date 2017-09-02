#include "PyStringPtr.hpp"
#include "StringCreationError.hpp"

static PyObject* createPyString(const char* text) {
  if (!text) {
    return nullptr;
  } else {
    PyObject* s = PyString_FromString(text);
    if (!s) {
      throw StringCreationError();
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
  return *this ? PyString_AS_STRING(get()) : EMPTY_STRING;
}

