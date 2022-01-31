#ifndef __PYEXT__PDF_TIN__IMMUTABLELAZYLIST_HPP__
#define __PYEXT__PDF_TIN__IMMUTABLELAZYLIST_HPP__

#include <PyObjectPtr.hpp>
#include <functional>
#include <memory>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>

struct PdfTin_ImmutableLazyList {
  PyObject_HEAD;

  PyObjectPtr items;
  std::function<PyObject* (Py_ssize_t)> createItem;

  int init(const Py_ssize_t size,
	   const std::function<PyObject* (Py_ssize_t)>& itemFactory);
  Py_ssize_t size() { return PyList_GET_SIZE(items.get()); }
  PyObject* getItem(Py_ssize_t index);
  PyObject* getSlice(const Py_ssize_t start, const Py_ssize_t end,
		     const Py_ssize_t step);

  static PyObject* create(
      const Py_ssize_t size,
      const std::function<PyObject* (Py_ssize_t)>& itemFactory
  );
  
  static PyObject* createForPrepopulation(const Py_ssize_t size);
  
  template <typename CreateItemFunction>
  static PyObject* createPrepopulated(
      const Py_ssize_t size, const CreateItemFunction& createItem
  ) {
    PyObjectPtr l(createForPrepopulation(size));
    for (auto i = 0; i < size; ++i) {
      PyObject* o = createItem(i);
      if (!o) {
	// Error creating item.  Assume exception has been set.
	return nullptr;
      }
      PyList_SET_ITEM(((PdfTin_ImmutableLazyList*)l.get())->items.get(), i, o);
    }
    return l.release();
  }
};

struct PdfTin_ImmutableLazyListIterator {
  PyObject_HEAD;

  PyObjectPtr list;
  Py_ssize_t current;

  int init(PdfTin_ImmutableLazyList* list, Py_ssize_t start) {
    this->list = PyObjectPtr(addRef((PyObject*)list));
    current = start;
    return 0;
  }

  PyObject* next();
};

PyTypeObject* pdfTin_ImmutableLazyListType();
PyTypeObject* pdfTin_ImmutableLazyListIteratorType();

#endif
