#ifndef __PYEXT__PDF_TIN__PYBYTESPTR_HPP__
#define __PYEXT__PDF_TIN__PYBYTESPTR_HPP__

#include <PyObjectPtr.hpp>
#include <string>
#include <stdint.h>
#include <string.h>

class PyBytesPtr : public PyObjectPtr {
public:
  PyBytesPtr() : PyObjectPtr() { }
  PyBytesPtr(const uint8_t* bytes, size_t n);

  explicit PyBytesPtr(const std::string& data)
      : PyBytesPtr((const uint8_t*)data.c_str(), data.size()) {
  }
  
  PyBytesPtr(const PyBytesPtr&) = default;
  PyBytesPtr(PyBytesPtr&&) = default;

  size_t size() const { return get() ? PyBytes_GET_SIZE(get()) : 0; }

  const uint8_t* data() const {
    return get() ? (const uint8_t*)PyBytes_AS_STRING(get()) : nullptr;
  }
  
  std::string toStr() const { return std::string((const char*)data(), size()); }
  
  PyBytesPtr& operator=(const PyBytesPtr&) = default;
  PyBytesPtr& operator=(PyBytesPtr&&) = default;

  PyBytesPtr fromCStr(const char* s) {
    return PyBytesPtr((const uint8_t*)s, s ? ::strlen(s) : 0);
  }
};

#endif

