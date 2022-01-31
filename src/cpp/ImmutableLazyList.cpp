#include "ImmutableLazyList.hpp"
#include <sstream>
#include <iostream>

static PyObject* pdfTin_ImmutableLazyList_new(
    PyTypeObject* type, PyObject* args, PyObject* keywords
) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new ((PdfTin_ImmutableLazyList*)self.get()) PdfTin_ImmutableLazyList;
  return self.release();
}

static int pdfTin_ImmutableLazyList_init(
    PdfTin_ImmutableLazyList* self, PyObject* args, PyObject* keywords
) {
  static char* KEYWORD_LIST[] = { (char*)"size", (char*)"create_item",
				  nullptr };
  Py_ssize_t listSize = 0;
  PyObject* capsule = nullptr;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "nO", KEYWORD_LIST,
				   &listSize, &capsule)) {
    return -1;
  }

  if (listSize < 0) {
    PyErr_SetString(PyExc_ValueError,
		    "Value for \"listSize\" must be nonnegative");
    return -1;
  }

  std::function<PyObject* (Py_ssize_t)>* itemFactory =
    (std::function<PyObject* (Py_ssize_t)>*)PyCapsule_GetPointer(capsule,
								 nullptr);

  if (!itemFactory) {
    PyErr_SetString(PyExc_ValueError,
		    "Value for \"create_item\" must not be null");
    return -1;
  }

  return self->init(listSize, *itemFactory);
}

static void pdfTin_ImmutableLazyList_destroy(PdfTin_ImmutableLazyList* self) {
  self->~PdfTin_ImmutableLazyList();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* pdfTin_ImmutableLazyList_create_iterator(
    PdfTin_ImmutableLazyList* self
) {
  PyObject* args = Py_BuildValue("Nn", self, 0);
  return PyObject_Call((PyObject*)pdfTin_ImmutableLazyListIteratorType(),
		       args, nullptr);
}

static Py_ssize_t pdfTin_ImmutableLazyList_length(
    PdfTin_ImmutableLazyList* self
) {
  return self->size();
}

// static PyObject* pdfTin_ImmutableLazyList_get_item(
//     PdfTin_ImmutableLazyList* self, Py_ssize_t index
// ) {
//   if (index >= self->size()) {
//     std::ostringstream msg;
//     msg << "No page at index " << index << " in a document with "
// 	<< self->size() << " pages";
//     PyErr_SetString(PyExc_IndexError, msg.str().c_str());
//     return nullptr;
//   }
//   return self->getItem(index);
// }

static PyObject* pdfTin_ImmutableLazyList_subscript(
    PdfTin_ImmutableLazyList* self, PyObject* key
) {
  if (PyLong_Check(key)) {
    const Py_ssize_t l = self->size();
    const Py_ssize_t index = PyLong_AsSsize_t(key);

    if ((index >= l) || (index < -l)) {
      // Index out of bounds
      std::ostringstream msg;
      msg << "No page at index " << index << " in a document with " << l
	  << " pages";
      PyErr_SetString(PyExc_IndexError, msg.str().c_str());
      return nullptr;
    } else if (index >= 0) {
      // Positive index in [0, len(self))
      return self->getItem(index);
    } else if (PyErr_Occurred()) {
      // Conversion from PyObject* to Py_ssize_t for index failed
      return nullptr;
    } else {
      // Negative index in [-len(self), 0)
      return self->getItem(l + index);
    }    
  } else if (PySlice_Check(key)) {
    const Py_ssize_t l = self->size();
    Py_ssize_t start, end, step;
    
    if (PySlice_Unpack(key, &start, &end, &step) < 0) {
      return nullptr;
    }
    PySlice_AdjustIndices(l, &start, &end, step);

    assert(start >= 0);
    assert(start <= l);
    assert(end >= start);
    assert(end <= l);
    assert(0 < step);
    
    return self->getSlice(start, end, step);    
  } else {
    std::ostringstream msg;
    msg << "Invalid type " << Py_TYPE(key)->tp_name << " used as list index";
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return nullptr;
  }
}

// static PySequenceMethods PDFTIN_PAGES_SEQUENCE_METHODS = {
//   (lenfunc)pdfTin_ImmutableLazyList_length,  // sq_length
//   0,                                         // sq_concat
//   0,                                         // sq_repeat
//   (ssizeargfunc)pdfTin_ImmutableLazyList_get_item, // sq_item
//   0,                                         // sq_ass_item
//   0,                                         // sq_contains
//   0,                                         // sq_inplace_concat
//   0                                          // sq_inplace_repeat
// };

static PyMappingMethods PDFTIN_PAGES_MAPPING_METHODS = {
  (lenfunc)pdfTin_ImmutableLazyList_length,  // mp_length
  (binaryfunc)pdfTin_ImmutableLazyList_subscript, // mp_subscript
  0,                                         // mp_ass_subscript
};

static PyTypeObject PDFTIN_IMMUTABLELAZYLIST_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pypdf_tin._ImmutableLazyList",    // tp_name
  sizeof(PdfTin_ImmutableLazyList),  // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_ImmutableLazyList_destroy, // tp_dealloc
  0,                            // tp_vectorcall_offset
  0,                            // tp_getattr
  0,                            // tp_setattr
  0,                            // tp_as_async
  0,                            // tp_repr
  0,                            // tp_as_number
  0,                            // tp_as_sequence
  &PDFTIN_PAGES_MAPPING_METHODS,   // tp_as_mapping
  0,                            // tp_hash
  0,                            // tp_call
  0,                            // tp_str
  0,                            // tp_getattro
  0,                            // tp_setattro
  0,                            // tp_as_buffer
  Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE, // tp_flags
  "",                           // tp_doc
  0,                            // tp_traverse
  0,                            // tp_clear
  0,                            // tp_richcompare
  0,                            // tp_weaklistoffset
  (getiterfunc)pdfTin_ImmutableLazyList_create_iterator, // tp_iter
  0,                            // tp_iternext
  0,                            // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_ImmutableLazyList_init,  // tp_init
  0,                            // tp_alloc
  pdfTin_ImmutableLazyList_new, // tp_new
  0,                            // tp_free
  0,                            // tp_is_gc
  0,                            // tp_bases
  0,                            // tp_mro
  0,                            // tp_cache
  0,                            // tp_subclasses
  0,                            // tp_weaklist
  0,                            // tp_del
  0,                            // tp_version_tag
  0,                            // tp_finalize
  0                             // tp_vectorcall
};

PyTypeObject* pdfTin_ImmutableLazyListType() {
  return &PDFTIN_IMMUTABLELAZYLIST_TYPE;
}

int PdfTin_ImmutableLazyList::init(
    const Py_ssize_t size,
    const std::function<PyObject* (Py_ssize_t)>& itemFactory
) {
  this->items = PyObjectPtr(PyList_New(size));
  if (!this->items) {
    return -1;
  }
  this->createItem = itemFactory;
  return 0;
}

PyObject* PdfTin_ImmutableLazyList::getItem(const Py_ssize_t index) {
  PyObject* item = PyList_GET_ITEM(items.get(), index);

  if (!item) {
    item = createItem(index);
    if (!item) {
      // Failed to create item.  createItem(index) should have set an
      // exception, so abort here.
      return nullptr;
    }
    PyList_SET_ITEM(items.get(), index, item);
  }

  // The item is owned by the cache, but the caller expects a new reference,
  // so increment the reference count by 1 before returning it so the
  // caller doesn't steal the item from the cache.
  Py_INCREF(item);
  return item;  
}

PyObject* PdfTin_ImmutableLazyList::getSlice(const Py_ssize_t start,
					     const Py_ssize_t end,
					     const Py_ssize_t step) {
  // Assume that:
  //   0 <= start <= len(this)
  //   0 <= end <= len(this)
  //   0 < step
  //   start <= end
  
  std::function<PyObject* (Py_ssize_t)> createItemFunc(createItem);
  std::function<PyObject* (Py_ssize_t)> newCreateItem(
      [createItemFunc, start, step](Py_ssize_t index) {
	return createItemFunc(start + step * index);
      }
  );
  
  const Py_ssize_t newLength = (end - start + step - 1) / step;
  PyObjectPtr capsule(PyCapsule_New((void*)&newCreateItem, nullptr, nullptr));
  PyObjectPtr args(Py_BuildValue("nO", newLength, capsule.get()));

  return PyObject_Call((PyObject*)pdfTin_ImmutableLazyListType(), args.get(),
		       nullptr);
}

PyObject* PdfTin_ImmutableLazyList::create(
    const Py_ssize_t size,
    const std::function<PyObject* (Py_ssize_t)>& itemFactory
) {
  PyObjectPtr itemFactoryCapsule(PyCapsule_New((void*)&itemFactory, nullptr,
					       nullptr));
  PyObjectPtr args(Py_BuildValue("nO", size, itemFactoryCapsule.get()));
  return PyObject_Call((PyObject*)pdfTin_ImmutableLazyListType(), args.get(),
		       nullptr);  
}

PyObject* PdfTin_ImmutableLazyList::createForPrepopulation(
    const Py_ssize_t size
) {
  static const std::function<PyObject* (Py_ssize_t)> CREATE_ITEM_FAILS(
      [](Py_ssize_t i) {
	std::ostringstream msg;
	msg << "Attempt to create item at index " << i
	    << " in prepoluated list";
	PyErr_SetString(PyExc_RuntimeError, msg.str().c_str());
	return (PyObject*)0;
      }
  );
  return PdfTin_ImmutableLazyList::create(size, CREATE_ITEM_FAILS);
}

static PyObject* pdfTin_ImmutableLazyListIterator_new(
    PyTypeObject* type, PyObject* args, PyObject* keywords
) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new ((PdfTin_ImmutableLazyListIterator*)self.get())
      PdfTin_ImmutableLazyListIterator;
  return self.release();
}

static int pdfTin_ImmutableLazyListIterator_init(
    PdfTin_ImmutableLazyListIterator* self, PyObject* args, PyObject* keywords
) {
  PyObject* list = nullptr;
  Py_ssize_t start = 0;

  if (!PyArg_ParseTuple(args, "On", &list, &start)) {
    return -1;
  }

  if (!PyObject_TypeCheck(list, pdfTin_ImmutableLazyListType())) {
    std::ostringstream msg;
    msg << "Value for \"list\" must be a "
	<< pdfTin_ImmutableLazyListType()->tp_name << " not a "
	<< Py_TYPE(list)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return -1;
  }
  
  if (start < 0) {
    PyErr_SetString(PyExc_ValueError,
		    "Value for \"start\" must be nonnegative");
    return -1;
  }

  return self->init((PdfTin_ImmutableLazyList*)list, start);
}

static void pdfTin_ImmutableLazyListIterator_destroy(
     PdfTin_ImmutableLazyListIterator* self
) {
  self->~PdfTin_ImmutableLazyListIterator();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* pdfTin_ImmutableLazyListIterator_create_iterator(
    PdfTin_ImmutableLazyListIterator* self
) {
  // Caler expects a new reference
  Py_INCREF(self);
  return (PyObject*)self;
}

static PyObject* pdfTin_ImmutableLazyListIterator_next(
    PdfTin_ImmutableLazyListIterator* self
) {
  return self->next();
}

static PyTypeObject PDFTIN_IMMUTABLELAZYLISTITERATOR_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pypdf_tin._ImmutableLazyListIterator",    // tp_name
  sizeof(PdfTin_ImmutableLazyListIterator),  // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_ImmutableLazyListIterator_destroy, // tp_dealloc
  0,                            // tp_vectorcall_offset
  0,                            // tp_getattr
  0,                            // tp_setattr
  0,                            // tp_as_async
  0,                            // tp_repr
  0,                            // tp_as_number
  0,                            // tp_as_sequence
  0,                            // tp_as_mapping
  0,                            // tp_hash
  0,                            // tp_call
  0,                            // tp_str
  0,                            // tp_getattro
  0,                            // tp_setattro
  0,                            // tp_as_buffer
  Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE, // tp_flags
  "",                           // tp_doc
  0,                            // tp_traverse
  0,                            // tp_clear
  0,                            // tp_richcompare
  0,                            // tp_weaklistoffset
  (getiterfunc)pdfTin_ImmutableLazyListIterator_create_iterator, // tp_iter
  (iternextfunc)pdfTin_ImmutableLazyListIterator_next, // tp_iternext
  0,                            // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_ImmutableLazyListIterator_init,  // tp_init
  0,                            // tp_alloc
  pdfTin_ImmutableLazyListIterator_new, // tp_new
  0,                            // tp_free
  0,                            // tp_is_gc
  0,                            // tp_bases
  0,                            // tp_mro
  0,                            // tp_cache
  0,                            // tp_subclasses
  0,                            // tp_weaklist
  0,                            // tp_del
  0,                            // tp_version_tag
  0,                            // tp_finalize
  0                             // tp_vectorcall
};

PyTypeObject* pdfTin_ImmutableLazyListIteratorType() {
  return &PDFTIN_IMMUTABLELAZYLISTITERATOR_TYPE;
}

PyObject* PdfTin_ImmutableLazyListIterator::next() {
  PdfTin_ImmutableLazyList* const p = (PdfTin_ImmutableLazyList*)list.get();
  if (current >= p->size()) {
    return nullptr;  // End of list
  }
  return p->getItem(current++);
}

