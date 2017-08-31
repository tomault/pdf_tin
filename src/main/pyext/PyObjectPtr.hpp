#ifndef __PYEXT__PDF_TIN__PYOBJECTPTR_HPP__
#define __PYEXT__PDF_TIN__PYOBJECTPTR_HPP__

inline PyObject* addRef(PyObject* o) { Py_XINCREF(o); return o; }

class PyObjectPtr {
public:
  PyObjectPtr() : obj_(nullptr) { }
  PyObjectPtr(PyObject* obj) : obj_(obj) { }
  PyObjectPtr(const PyObjectPtr& other) : obj_(addRef(other.get())) { }
  PyObjectPtr(PyObjectPtr&& other) : obj_(other.obj_) {
    other.obj_ = nullptr;
  }
  virtual ~PyObjectPtr() { Py_XDECREF(obj_); }

  /** @brief Returns the underlying PyObject* */
  PyObject* get() const { return obj_; }

  PyObject* release() {
    PyObject* tmp = obj_;
    obj_ = nullptr;
    return tmp;
  }
  
  PyObjectPtr& operator=(const PyObjectPtr& other);
  PyObjectPtr& operator=(PyObjectPtr&& other);

  operator bool() const { return (bool)obj_; }
  bool operator==(const PyObjectPtr& other) const {
    return obj_ == other.obj_;
  }
  bool operator!=(const PyObjectPtr& other) const {
    return obj_ != other.obj_;
  }

  static PyObjectPtr none();
  static PyObjectPtr fromBool(bool v);
  static PyObjectPtr trueValue();
  static PyObjectPtr falseValue();
  
private:
  PyObject* obj_;
};

#endif
