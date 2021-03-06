#ifndef __PYEXT__PDF_TIN__PYSTRINGPTR_HPP__
#define __PYEXT__PDF_TIN__PYSTRINGPTR_HPP__

#include <PyObjectPtr.hpp>
#include <string>

class PyStringPtr : public PyObjectPtr {
public:
  PyStringPtr() : PyObjectPtr() { }
  explicit PyStringPtr(const char* text);
  explicit PyStringPtr(const std::string& text) : PyStringPtr(text.c_str()) { }
  PyStringPtr(const PyStringPtr&) = default;
  PyStringPtr(PyStringPtr&&) = default;

  const char* text() const;
  std::string toStr() const { return std::string(text); }
  
  PyStringPtr& operator=(const PyStringPtr&) = default;
  PyStringPtr& operator=(PyStringPtr&&) = default;
};

#endif

