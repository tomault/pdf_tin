#ifndef __PYEXT__PDF_TIN__PYOBJECTPTR_HPP__
#define __PYEXT__PDF_TIN__PYOBJECTPTR_HPP__

#define PY_SSIZE_T_CLEAN
#include <Python.h>

inline PyObject* addRef(PyObject* o) { Py_XINCREF(o); return o; }

class PyObjectPtr {
public:
  PyObjectPtr() : obj_(nullptr) { }
  explicit PyObjectPtr(PyObject* obj) : obj_(obj) { }
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

  void reset() {
    Py_XDECREF(obj_);
    obj_ = nullptr;
  } 
  
  PyObjectPtr& operator=(const PyObjectPtr& other) {
    set_(other.obj_);
    return *this;
  }
  
  PyObjectPtr& operator=(PyObjectPtr&& other) {
    move_(other.obj_);
    return *this;
  }

  operator bool() const { return (bool)obj_; }
  bool operator==(const PyObjectPtr& other) const {
    return obj_ == other.obj_;
  }
  bool operator!=(const PyObjectPtr& other) const {
    return obj_ != other.obj_;
  }

  static PyObjectPtr none() { return PyObjectPtr(addRef(Py_None)); }
  static PyObjectPtr fromBool(bool v) { return v ? trueValue() : falseValue(); }
  static PyObjectPtr trueValue() { return PyObjectPtr(addRef(Py_True)); }
  static PyObjectPtr falseValue() { return PyObjectPtr(addRef(Py_False)); }

protected:
  void set_(PyObject* v) {
    if (obj_ != v) {
      Py_XDECREF(obj_);
      obj_ = addRef(v);
    }
  }

  void move_(PyObject*& v) {
    if (obj_ != v) {
      Py_XDECREF(obj_);
      obj_ = v;
      v = nullptr;
    }
  }
  
private:
  PyObject* obj_;
};

#define DECLARE_PyObjectPtr_GETTER(NAME, TYPE, FIELD)		\
  PyObject* get_##NAME##_value(TYPE* self, void* closure) {     \
    return addRef(self->FIELD.get());				\
  }

#define DECLARE_PyObjectPtr_SETTER(NAME, TYPE, FIELD)              \
  int set_##NAME##_value(TYPE* self, PyObject* v, void* closure) { \
    self->FIELD = PyObjectPtr(addRef(v));			   \
    return 0;							   \
  }

#define DECLARE_PyBool_GETTER(NAME, TYPE, FIELD)                   \
  PyObject* get_##NAME##_value(TYPE* self, void* closure) {        \
    return addRef(self->FIELD ? Py_True : Py_False);		   \
  }

#define DECLARE_PyBool_SETTER(NAME, TYPE, FIELD)                   \
  int set_##NAME##_value(TYPE* self, PyObject* v, void* closure) { \
    self->FIELD = (bool)PyObject_IsTrue(v);                        \
    return 0;                                                      \
  }

#define PyObjectPtr_INIT(v, o)                                     \
  v = PyObjectPtr(o);						   \
  if (!(v)) {							   \
    return -1;							   \
  }

#endif
