#include "Page.hpp"

#include "ImmutableLazyList.hpp"
#include "PdfDocument.hpp"
#include "Text.hpp"
#include <structmember.h>
#include <pdf_tin/Document.hpp>
#include <pdf_tin/NoSuchPageError.hpp>
#include <iostream>
#include <sstream>

static PyObject* pdfTin_page_new(PyTypeObject* type, PyObject* args,
				 PyObject* keywords) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new(&(((PdfTin_Page*)self.get())->impl)) PdfTin_PageImpl;
  return self.release();
}

static int pdfTin_page_init(PdfTin_Page* self, PyObject* args,
			    PyObject* keywords) {
  static char* KEYWORD_LIST[] = { (char*)"doc", (char*)"style_cache",
				  (char*)"index", nullptr };
  PyObject* docCapsule = nullptr;
  PyObject* styleCacheCapsule = nullptr;
  Py_ssize_t index = 0;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "OOn", KEYWORD_LIST,
				   &docCapsule, &styleCacheCapsule, &index)) {
    return -1;
  }

  std::shared_ptr<pdf_tin::Document> doc;

  if (PyObject_TypeCheck(docCapsule, pdfTin_DocumentType())) {
    doc = ((PdfTin_Document*)docCapsule)->impl.doc;
  } else if (PyCapsule_CheckExact(docCapsule)) {
    void* p = PyCapsule_GetPointer(docCapsule, nullptr);
    doc = *(std::shared_ptr<pdf_tin::Document>*)p;
  } else {
    std::ostringstream msg;
    msg << "Value for \"document\" has incorrect type "
	<< Py_TYPE(docCapsule)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return -1;
  }

  if (!PyCapsule_CheckExact(styleCacheCapsule)) {
    std::ostringstream msg;
    msg << "Value for \"style_cache\" has incorrect type "
	<< Py_TYPE(styleCacheCapsule)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return -1;
  }

  std::shared_ptr<TextStyleCache>* textStyleCache =
    (std::shared_ptr<TextStyleCache>*)PyCapsule_GetPointer(styleCacheCapsule,
							   nullptr);

  if (!textStyleCache || !*textStyleCache) {
    PyErr_SetString(PyExc_ValueError, "Value for \"style_cache\" is null");
    return -1;
  }
  
  if (index >= (Py_ssize_t)doc->numPages()) {
    std::ostringstream msg;
    msg << "Value for \"page\" (" << index << ") is out of range.  The "
	<< "document has " << doc->numPages() << " pages.";
    PyErr_SetString(PyExc_ValueError, msg.str().c_str());
    return -1;
  }
  
  std::shared_ptr<pdf_tin::Page> page(new pdf_tin::Page(doc->page(index)));
  return self->impl.init(doc, page, *textStyleCache);
}

static void pdfTin_page_destroy(PdfTin_Page* self) {
  self->impl.~PdfTin_PageImpl();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static DECLARE_PyObjectPtr_GETTER(label, PdfTin_Page, impl.label);

static PyObject* get_index_value(PdfTin_Page* self, void* closure) {
  return PyLong_FromLong(self->impl.page->index());
}

static PyObject* get_width_value(PdfTin_Page* self, void* closure) {
  return PyFloat_FromDouble(self->impl.page->width());
}

static PyObject* get_height_value(PdfTin_Page* self, void* closure) {
  return PyFloat_FromDouble(self->impl.page->height());
}

static PyObject* get_duration_value(PdfTin_Page* self, void* closure) {
  return PyFloat_FromDouble(self->impl.page->duration());
}

static PyObject* get_text_value(PdfTin_Page* self, void* closure) {
  if (!self->impl.text) {
    std::vector<pdf_tin::Text> pageText = self->impl.page->text();
    self->impl.text = PyObjectPtr(
	PdfTin_ImmutableLazyList::createPrepopulated(
	    pageText.size(),
	    [self, &pageText](Py_ssize_t i) {
	      return PdfTin_Text::create(pageText[i], *self->impl.styleCache);
	    }
	)
    );
  }
  return addRef(self->impl.text.get());
}

static PyMemberDef PDFTIN_PAGE_MEMBERS[] = {
  // name, type, offset, flags, docstring
  { 0, 0, 0, 0, 0 }
};

static PyMethodDef PDFTIN_PAGE_METHODS[] = {
  // name, impl, flags, docstring
  { 0, 0, 0, 0 }  
};

static PyGetSetDef PDFTIN_PAGE_GETSET[] = {
  // name, getter, setter, docstring, closure
  { "label", (getter)get_label_value, 0, "(str) This page's label", 0 },
  { "index", (getter)get_index_value, 0,
    "(int) Index of this page in its document.  The first page has index 0.",
    0 },
  { "width", (getter)get_width_value, 0, "(float) Width of this page.", 0 },
  { "height", (getter)get_height_value, 0, "(float) Height of this page.", 0 },
  { "duration", (getter)get_duration_value, 0,
    "(float) Duration (?) of this page.", 0 },
  { "text", (getter)get_text_value, 0,
    "(sequence of pypdf_tin.Text) Text on the page", 0 },
  { 0, 0, 0, 0, 0 }
};

static PyTypeObject PDFTIN_PAGE_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pdf_tin.Page",               // tp_name
  sizeof(PdfTin_Page),          // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_page_destroy, // tp_dealloc
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
  0,                            // tp_iter
  0,                            // tp_iternext
  PDFTIN_PAGE_METHODS,          // tp_methods
  PDFTIN_PAGE_MEMBERS,          // tp_members
  PDFTIN_PAGE_GETSET,           // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_page_init,   // tp_init
  0,                            // tp_alloc
  pdfTin_page_new,              // tp_new
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

PyTypeObject* pdfTin_PageType() {
  return &PDFTIN_PAGE_TYPE;
}

PdfTin_PageImpl::PdfTin_PageImpl()
    : doc(), page(), label() {
  // Intentionally left blank
}

int PdfTin_PageImpl::init(const std::shared_ptr<pdf_tin::Document>& doc,
			  const std::shared_ptr<pdf_tin::Page>& page,
			  const std::shared_ptr<TextStyleCache>& styleCache) {
  this->doc = doc;
  this->page = page;
  this->styleCache = styleCache;
  PyStringPtr_INIT(this->label, page->label());

  // this->text populated on first access
  return 0;
}
