#include "Pages.hpp"
#include "Page.hpp"
#include "PageIterator.hpp"
#include "PdfDocument.hpp"
#include "PyObjectPtr.hpp"
#include <sstream>

static PyObject* pdfTin_pages_new(PyTypeObject* type, PyObject* args,
				  PyObject* keywords) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new(&(((PdfTin_Pages*)self.get())->impl)) PdfTin_PagesImpl;
  return self.release();
}

static int pdfTin_pages_init(PdfTin_Pages* self, PyObject* args,
			     PyObject* keywords) {
  PyObject* doc = nullptr;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "O", nullptr, &doc)) {
    return -1;
  }

  if (!PyObject_TypeCheck(doc, pdfTin_DocumentType())) {
    std::ostringstream msg;
    msg << "Value for \"document\" should be a "
	<< pdfTin_DocumentType()->tp_name << " not a "
	<< Py_TYPE(doc)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return -1;
  }

  return self->impl.init(((PdfTin_Document*)doc)->impl.doc);
}

static void pdfTin_pages_destroy(PdfTin_Pages* self) {
  self->impl.~PdfTin_PagesImpl();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* pdfTin_pages_create_iterator(PdfTin_Pages* self) {
  PyObjectPtr args(Py_BuildValue("NI", self, (unsigned int)0));
  if (!args) {
    return nullptr;
  }
  
  return PyObject_Call(pdfTin_PagesIteratorType(), args.get(), nullptr);
}

static Py_ssize_t pdfTin_pages_length(PyObject* self) {
  return (Py_ssize_t)((PdfTin_Pages*)self)->impl.doc->numPages();
}

static PyObject* pdfTin_pages_get_page(PyObject* self, Py_ssize_t i) {
  // Because PdfTin_Pages supplies a .sq_length slot, the interpreter
  // should convert negative indices to positive indices for us.
  // See PySequenceMethods.sq_item in
  // https://docs.python.org/3/c-api/typeobj.html#sequence-structs
  assert(i >= 0);

  if (i >= (Py_ssize_t)((PdfTin_Pages*)self)->impl.doc->numPages()) {
    std::ostringstream msg;
    msg << "No page at index " << i << ".  Pages range from 0 to "
	<< (((PdfTin_Pages*)self)->impl.doc->numPages() - 1);
    PyErr_SetString(PyExc_IndexError, msg.str().c_str());
    return nullptr;
  }

  return ((PdfTin_Pages*)self)->impl.getPage(i);
}

static PyMemberDef PDFTIN_PAGES_MEMBERS[] = {
  // name, type, offset, flags, docstring
  { 0, 0, 0, 0, 0 }
};

static PyMethodDef PDFTIN_PAGES_METHODS[] = {
  // name, impl, flags, docstring
  { 0, 0, 0, 0 }  
};

static PyGetSetDef PDFTIN_PAGE_GETSET[] = {
  // name, getter, setter, docstring, closure
  { 0, 0, 0, 0, 0 }
};

struct PySequenceMethods PDFTIN_PAGES_SEQUENCE_METHODS = {
  .sq_length = pdfTin_pages_length,
  .sq_concat = 0,
  .sq_repeat = 0,
  .sq_item = pdfTin_pages_get_page,
  .sq_ass_item = 0,
  .sq_contains = 0,
  .sq_inplace_concat = 0,
  .sq_inplace_repeat = 0
};

static PyTypeObject PDFTIN_PAGES_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pypdf_tin._Pages",           // tp_name
  sizeof(PdfTin_Pages),         // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_pages_destroy, // tp_dealloc
  0,                            // tp_vectorcall_offset
  0,                            // tp_getattr
  0,                            // tp_setattr
  0,                            // tp_as_async
  0,                            // tp_repr
  0,                            // tp_as_number
  PDFTIN_PAGES_SEQUENCE_METHODS,  // tp_as_sequence
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
  pdfTin_pages_create_iterator, // tp_iter
  0,                            // tp_iternext
  PDFTIN_PAGES_METHODS,         // tp_methods
  PDFTIN_PAGES_MEMBERS,         // tp_members
  PDFTIN_PAGES_GETSET,          // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_pages_init,  // tp_init
  0,                            // tp_alloc
  pdfTin_pages_new,             // tp_new
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

int PdfTin_PagesImpl::init(const std::shared_ptr<pdf_tin::Document>& doc) {
  this->doc = doc;
  cache = PyObjectPtr(PyList_New(doc->numPages()));
  if (!cache) {
    doc.reset();
    return -1;
  }
  return 0;
}

PyObject* PdfTin_PagesImpl::getPage(Py_ssize_t index) {
  // Assume that i is in range.  Caller should perform bounds checking

  PyObject* page = PyList_GET_ITEM(cache.get(), index);
  if (!page) {
    PyObjectPtr capsule(PyCapsule_New(&doc, nullptr, nullptr));
    if (!capsule) {
      return nullptr;
    }

    PyObjectPtr args(Py_BuildValue("Nn", capsule.get(), index));
    if (!args) {
      return nullptr;
    }

    page = PyObject_Call(pdfTin_PageType(), args.get(), nullptr);
    PyList_SET_ITEM(cache.get(), index, page);

    // PyList_SET_ITEM steals a reference, so the cache now owns the
    // new page.
  }

  // Caller expects a new reference, but the cache owns the page,
  // so increment the reference count so the caller doesn't steal
  // the page from the cache.
  Py_INCREF(page);
  return page;
}

static PyObject* pdfTin_pagesIterator_new(PyTypeObject* type, PyObject* args,
					  PyObject* keywords) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new(self.get()) PdfTin_PagesIterator;
  return self.release();
}

static PyObject* pdfTin_pagesIterator_init(
    PdfTin_PagesIterator* self, PyObject* args, PyObject* keywords
) {
  PyObject* pages = nullptr;
  Py_ssize_t index = 0;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "On", &pages, &index)) {
    return nullptr;
  }

  if (!PyObject_TypeCheck(pages, pdfTin_PagesType())) {
    std::ostringstream msg;
    msg << "Value for \"pages\" must be a " << pdfTin_PagesType()->tp_name
	<< ", not a " << Py_TYPE(pages)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return -1;
  }

  if (index < 0) {
    PyErr_SetString(PyExc_ValueError,
		    "Value for \"index\" must be nonnegative");
    return -1;
  }

  return self->init((PdfTin_Pages*)pages, current);
}

static void pdfTin_pagesIterator_destroy(PdfTin_PagesIterator* self) {
  self->~PdfTin_PagesIterator();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* pdfTin_pagesIterator_create_iterator(
    PdfTin_PagesIterator* self
) {
  // Caller expects new reference
  return addRef(self);
}

static PyOjbect* pdfTin_pagesIterator_next(PdfTin_PagesIterator* self) {
  return self->next();
}

struct PyTypeObject PDFTIN_PAGESITERATOR_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pypdf_tin._PagesIterator",   // tp_name
  sizeof(PdfTin_PagesIterator), // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_pagesIterator_destroy, // tp_dealloc
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
  (getiterfunc)pdfTin_pagesIterator_create_iterator, // tp_iter
  (iternextfunc)pdfTin_pagesIterator_next, // tp_iternext
  0,                            // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_pagesIterator_init,   // tp_init
  0,                            // tp_alloc
  (newfunc)pdfTin_pagesIterator_new, // tp_new
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

PyTypeObject* pdfTin_PagesIteratorType() {
  return &PDFTIN_PAGESITERATOR_TYPE;
}

int PdfTin_PagesIterator::init(PdfTin_Pages* pages, Py_ssize_t current) {
  pages = PyObjectPtr(addRef((PyObject*)pages));
  current = current;
  return 0;
}

PyObject* PdfTin_PagesIterator::next() {
  if (current >= ((PdfTin_Pages*)pages.get())->doc->numPages()) {
    return nullptr;
  }

  return ((PdfTin_Pages*)pages.get())->impl.getPage(current++);
}
