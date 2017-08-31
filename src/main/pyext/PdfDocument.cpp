#include "PdfDocument.hpp"
#include "StringCreationError.hpp"
#include <pdf_tin/DocumentCreationError.hpp>
#include <Python.h>
#include <datetime.h>
#include <time.h>

static PyObject* pdfTin_pdfDocument_new(PyTypeObject* type, PyObject* args,
					PyObject* keywords);
static int pdfTin_pdfDocument_init(PdfDocument* self, PyObject* args,
				   PyObject* keywords);
static void pdfTin_pdfDocument_destroy(PdfDocument* self);

static PyTypeObject PDF_DOCUMENT_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pdf_tin.Document",           // tp_name
  sizeof(PdfTin_Document),      // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_pdfDocument_destroy, // tp_dealloc
  0,                            // tp_print
  0,                            // tp_getattr
  0,                            // tp_setattr
  0,                            // tp_compare
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
  0,                            // tp_traverse
  0,                            // tp_clear
  0,                            // tp_richcompare
  0,                            // tp_weaklistoffset
  0,                            // tp_iter
  0,                            // tp_iternext
  PDFTIN_DOCUMENT_METHODS,      // tp_methods
  PDFTIN_DOCUMENT_MEMBERS,      // tp_members
  0,                            // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_pdfDocument_init, // tp_init
  0,                            // tp_alloc
  pdfTin_pdfDocument_new,       // tp_new
};

static pdf_tin::Document* loadDocumentFromFile(const char* url,
					       const char* password) {
  try {
    if (password) {
      return pdf_tin::Document::fromFile(url, password);
    } else {
      return pdf_tin::Document::fromFile(url);
    }
  } catch(const pdf_tin::DocumentCreationError& e) {
    PyErr_SetString(PyExc_IOError, e.what());
    return nullptr;
  }
}

static pdf_tin::Document* createDocumentFromBytes(const char* data,
						  const char* password) {
  try {
    if (password) {
      return pdf_tin::Document::fromBytes(const_cast<char*>(data)<
					  password);
    } else {
      return pdf_tin::Document::fromBytes(const_cast<char*>(data));
    }
  } catch(const pdf_tin::DocumentCreationError& e) {
    PyErr_SetString(PyExc_IOError, e.what());
    return nullptr;
  }
}

static PyObject* pdfTin_pdfDocument_new(PyTypeObject* type, PyObject* args,
					PyObject* keywords) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new(&self->impl) PdfTin_DocumentImpl
  return self.release();
}

static int pdfTin_pdfDocument_init(PdfDocument* self, PyObject* args,
				   PyObject* keywords) {
  static char* KEYWORD_LIST = { "url", "data", "password" };
  const char* url = nullptr;
  const char* data = nullptr;
  const char* password = nullptr;
  int dataSize = 0;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "|zz#z", KEYWORD_LIST,
				   &url, &data, &dataSize, &password)) {
    return -1;
  }
  if (url && data) {
    PyErr_SetString(PyExc_ValueError,
		    "Cannot specify both \"url\" and \"data\"");
    return -1;
  } else if (url) {
    std::shared_ptr<pdf_tin::Document> doc(loadDocumentFromFile(url, password));
    return doc ? self->init(doc) : -1;
  } else if (data) {
    std::shared_ptr<pdf_tin::Document> doc(createDocumentFromBytes(url,
								   password));
    return doc ? self->init(doc) : -1;
  } else {
    PyErr_SetString(PyExc_ValueError,
		    "Either \"url\" or \"data\" must have a value");
    return -1;
  }
}

static void pdfTin_pdfDocument_destroy(PdfDocument* self); {
  self->impl.~PdfDocumentImpl();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* convertToPyDateTime(time_t t) {
  struct tm tmp;

  localtime_r(&t, &tmp);
  return PyDateTime_FromDateAndTime(tm.tm_year + 1900, tm.tm_mon + 1,
				    tm.tm_mday, tm.tm_hour, tm.tm_min,
				    tm.tm_sec, 0);
}

PyTypeObject* pdfTin_PdfDocumentType() {
  return &PDF_DOCUMENT_TYPE;
}

  
PdfTin_DocumentImpl::PdfTin_DocumentImpl()
    : doc(), permanentId(PyStringPtr::none()), updateId(PyStringPtr::none()),
      numPages(0), title(PyStringPtr::none()), author(PyStringPtr::none()),
      subject(PyStringPtr::none()), keywords(PyStringPtr::none()),
      creator(PyStringPtr::none()), producer(PyStringPtr::none()),
      creationDate(PyObjectPtr::none()), modificationDate(PyObjectPtr::none()),
      linearized(PyObjectPtr::falseValue()), pageLayout(PyStringPtr::none()),
      pageMode(PyStringPtr::none()), metadata(PyStringPtr::none()) {
  // Intentionally left blank
}

int PdfTin_DocumentImpl::init(const std::shared_ptr<pdf_tin::Document>& doc) {
  this->doc = doc;
  try {
    this->permanentId = PyStringPtr(doc->permanentId());
    this->updateId = PyStringPtr(doc->updateId());
    this->numPages = PyIntPtr(doc->numPages());
    this->pdfVersion = PyStringPtr(doc->pdfVersion());
    this->title = PyStringPtr(doc->title());
    this->author = PyStringPtr(doc->author());
    this->subject = PyStringPtr(doc->subject());
    this->keywords = PyStringPtr(doc->keywords());
    this->creator = PyStringPtr(doc->creator());
    this->producer = PyStringPtr(doc->producer());
    this->creationDate = convertToPyDateTime(doc->creationDate());
    this->modificationDate = convertToPyDateTime(doc->modificationDate());
    this->linearized = PyObjectPtr::fromBool(doc->isLinearized());
    this->pageLayout = PyStringPtr(toStr(doc->pageLayout()));
    this->pageMode = PyStringPtr(toStr(doc->pageMode()));
    this->metadata = PyStringPtr(doc->metadata());
    return 0;
  } catch(const StringCreationError& e) {
    return -1;
  }
}
