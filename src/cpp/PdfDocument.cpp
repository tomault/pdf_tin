#include "PdfDocument.hpp"
#include "ImmutableLazyList.hpp"
#include "Page.hpp"
#include "Text.hpp"
#include "TextStyleSegment.hpp"
#include "TextStyle.hpp"
#include <pdf_tin/DocumentCreationError.hpp>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>
#include <datetime.h>
#include <time.h>
#include <iostream>
#include <vector>


static PyObject* pdfTin_pdfDocument_new(PyTypeObject* type, PyObject* args,
					PyObject* keywords);
static int pdfTin_pdfDocument_init(PdfTin_Document* self, PyObject* args,
				   PyObject* keywords);
static void pdfTin_pdfDocument_destroy(PdfTin_Document* self);

static PyObject* pdfTin_pdfDocument_repr(PdfTin_Document* self) {
  std::ostringstream repr;
  repr << Py_TYPE(self)->tp_name << "("
       << PyUnicode_AsUTF8(self->impl.origin.get()) << ")";
  std::string tmp = repr.str();
  return PyUnicode_DecodeUTF8(tmp.c_str(), tmp.size(), nullptr);
}

static PyObject* pdfTin_pdfDocument_str(PdfTin_Document* self) {
  return pdfTin_pdfDocument_repr(self);
}


#define DECLARE_GETTER(FIELD) \
  static DECLARE_PyObjectPtr_GETTER(FIELD, PdfTin_Document, impl.FIELD)

DECLARE_GETTER(permanentId);
DECLARE_GETTER(updateId);
DECLARE_GETTER(numPages);
DECLARE_GETTER(title);
DECLARE_GETTER(author);
DECLARE_GETTER(subject);
DECLARE_GETTER(keywords);
DECLARE_GETTER(creator);
DECLARE_GETTER(producer);
DECLARE_GETTER(linearized);
DECLARE_GETTER(pageLayout);
DECLARE_GETTER(pageMode);
DECLARE_GETTER(metadata);
DECLARE_GETTER(pages);

static PyObject* pdfTin_PdfDocument_create_text(
    PdfTin_Document* self, PyObject* args, PyObject* keywords
) {
  static char* KEYWORD_LIST[] = {
    (char*)"font_name", (char*)"font_size", (char*)"left", (char*)"right",
    (char*)"top", (char*)"bottom", (char*)"styles", nullptr
  };
  const char* text = nullptr;
  Py_ssize_t textSize = -1;
  double left, right, top, bottom;
  PyObject* styles = nullptr;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "s#ddddO", KEYWORD_LIST,
				   &text, &textSize, &left, &right, &top,
				   &bottom, &styles)) {
    return nullptr;
  }

  std::vector<pdf_tin::TextStyleSegment> segments;
  PyObjectPtr iter(PyObject_GetIter(styles));
  if (!iter) {
    PyErr_SetString(PyExc_TypeError, "Value for \"styles\" must be iterable");
    return nullptr;
  }

  PyObjectPtr segment(PyIter_Next(iter.get()));
  Py_ssize_t index = 0;
  while (segment) {
    if (!PyObject_IsInstance(segment.get(),
			     (PyObject*)pdfTin_TextStyleSegmentType())) {
      std::ostringstream msg;
      msg << "Members of segments must be a "
	  << pdfTin_TextStyleSegmentType()->tp_name << ".  The value at "
	  << "index " << index << " is a " << Py_TYPE(segment.get())->tp_name;
      PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    }

    PdfTin_TextStyleSegment* const s = (PdfTin_TextStyleSegment*)segment.get();
    PdfTin_TextStyle* const textStyle = (PdfTin_TextStyle*)s->style.get();
    PdfTin_Color* const color = (PdfTin_Color*)textStyle->color.get();
    
    segments.emplace_back(textStyle->fontName.toStr(),
			  (uint32_t)textStyle->fontSize, textStyle->underlined,
			  pdf_tin::Color(color->red, color->green, color->blue),
			  s->start, s->end);
    segment = PyObjectPtr(PyIter_Next(iter.get()));
    ++index;
  }

  if (PyErr_Occurred()) {
    return nullptr;
  }

  pdf_tin::Text theText(std::string(text, text + textSize),
			pdf_tin::BoundingBox(left, bottom, right, top),
			segments.begin(), segments.end());
  PyObjectPtr textCapsule(PyCapsule_New((void*)&theText, nullptr, nullptr));
  PyObjectPtr styleCacheCapsule(
      PyCapsule_New((void*)self->impl.styleCache.get(), nullptr, nullptr)
  );
  PyObjectPtr textArgs(Py_BuildValue("OO", textCapsule.get(),
				     styleCacheCapsule.get()));
  return PyObject_Call((PyObject*)pdfTin_TextType(), textArgs.get(), nullptr);
}

static PyObject* pdfTin_PdfDocument_create_text_style(
    PdfTin_Document* self, PyObject* args, PyObject* keywords
) {
  static char* KEYWORD_LIST[] = {
    (char*)"font_name", (char*)"font_size", (char*)"underlined",
    (char*)"color", nullptr
  };
  const char* fontName = nullptr;
  Py_ssize_t fontNameLength = -1;
  uint32_t fontSize = 0;
  int underlined = -1;
  PyObject* color = nullptr;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "s#kpO", KEYWORD_LIST,
				   &fontName, &fontNameLength, &fontSize,
				   &underlined, &color)) {
    return nullptr;
  }

  if (fontNameLength < 1) {
    PyErr_SetString(PyExc_ValueError, "font_name is empty");
    return nullptr;
  }

  if (!PyObject_IsInstance(color, (PyObject*)pdfTin_ColorType())) {
    std::ostringstream msg;
    msg << "Value for color should be a " << pdfTin_ColorType()->tp_name
	<< ", not a " << Py_TYPE(color)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return nullptr;
  }

  PdfTin_Color* const c = (PdfTin_Color*)color;
  pdf_tin::TextStyle s(std::string(fontName, fontName + fontNameLength),
		       fontSize, (bool)underlined,
		       pdf_tin::Color(c->red, c->green, c->blue));

  return (PyObject*)(*self->impl.styleCache)[s];
}

static PyMemberDef PDFTIN_DOCUMENT_MEMBERS[] = {
  // name, type, offset, flags, docstring
  { 0, 0, 0, 0, 0 }
};

static PyMethodDef PDFTIN_DOCUMENT_METHODS[] = {
  // name, impl, flags, docstring
  { "create_text",
    (PyCFunction)
        pdfTin_PdfDocument_create_text, METH_VARARGS|METH_KEYWORDS,
    "Create a pypdf_tin.Text instance" },
  { "create_text_style",
    (PyCFunction)
        pdfTin_PdfDocument_create_text_style,
    METH_VARARGS|METH_KEYWORDS,
    "Create a pypdf_tin.TextStyle instance" },
  { 0, 0, 0, 0 }
};

static PyGetSetDef PDFTIN_DOCUMENT_GETSET[] = {
  // name, getter, setter, docstring, closure
  { "permanent_id", (getter)get_permanentId_value, 0,
    "(str) Permanent id for document", 0 },
  { "update_id", (getter)get_updateId_value, 0,
    "(str) Update id for document", 0 },
  { "num_pages", (getter)get_numPages_value, 0,
    "(int) Number of pages in document", 0 },
  { "title", (getter)get_title_value, 0,
    "(str) Title for document", 0 },
  { "author", (getter)get_author_value, 0,
    "(str) Author for document", 0 },
  { "subject", (getter)get_subject_value, 0,
    "(str) Document's subject", 0 },
  { "keywords", (getter)get_keywords_value, 0,
    "(str) Keywords assigned to document", 0 },
  { "creator", (getter)get_creator_value, 0,
    "(str) Document's creator", 0 },
  { "producer", (getter)get_producer_value, 0,
    "(str) Document's producer", 0 },
  { "linearized", (getter)get_linearized_value, 0,
    "(bool) Whether document is linearized", 0 },
  { "page_layout", (getter)get_pageLayout_value, 0,
    "(str) How the document's pages are laid out.", 0 },
  { "page_mode", (getter)get_pageMode_value, 0,
    "(str) The page mode for the document.", 0 },  
  { "metadata", (getter)get_metadata_value, 0,
    "(str) The document's metadata.", 0 },
  { "pages", (getter)get_pages_value, 0,
    "(sequence of pypdf_tin.Page) The document's pages.  The returned "
    "sequence supports len(), iteration and the [] operator.",
    0 },
  { 0, 0, 0, 0, 0 }
};
  
static PyTypeObject PDFTIN_DOCUMENT_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pdf_tin.Document",           // tp_name
  sizeof(PdfTin_Document),      // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_pdfDocument_destroy, // tp_dealloc
  0,                            // tp_vectorcall_offset
  0,                            // tp_getattr
  0,                            // tp_setattr
  0,                            // tp_as_async
  (reprfunc)pdfTin_pdfDocument_repr,      // tp_repr
  0,                            // tp_as_number
  0,                            // tp_as_sequence
  0,                            // tp_as_mapping
  0,                            // tp_hash
  0,                            // tp_call
  (reprfunc)pdfTin_pdfDocument_str,       // tp_str
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
  PDFTIN_DOCUMENT_METHODS,      // tp_methods
  PDFTIN_DOCUMENT_MEMBERS,      // tp_members
  PDFTIN_DOCUMENT_GETSET,       // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_pdfDocument_init, // tp_init
  0,                            // tp_alloc
  pdfTin_pdfDocument_new,       // tp_new
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

static pdf_tin::Document* loadDocumentFromFile(const char* url,
					       const char* password) {
  try {
    if (password) {
      return new pdf_tin::Document(pdf_tin::Document::fromFile(url, password));
    } else {
      return new pdf_tin::Document(pdf_tin::Document::fromFile(url));
    }
  } catch(const pdf_tin::DocumentCreationError& e) {
    PyErr_SetString(PyExc_IOError, e.what());
    return nullptr;
  }
}

static pdf_tin::Document* createDocumentFromBytes(const char* data,
						  const char* end,
						  const char* password) {
  try {
    if (password) {
      return new pdf_tin::Document(
	  pdf_tin::Document::fromBytes(const_cast<char*>(data),
				       const_cast<char*>(end), password)
      );
    } else {
      return new pdf_tin::Document(
	  pdf_tin::Document::fromBytes(const_cast<char*>(data),
				       const_cast<char*>(end))
      );
    }
  } catch(const pdf_tin::DocumentCreationError& e) {
    PyErr_SetString(PyExc_IOError, e.what());
    return nullptr;
  }
}

static PyObject* pdfTin_pdfDocument_new(PyTypeObject* type, PyObject* args,
					PyObject* keywords) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new(&(((PdfTin_Document*)self.get())->impl)) PdfTin_DocumentImpl;
  return self.release();
}

static int pdfTin_pdfDocument_init(PdfTin_Document* self, PyObject* args,
				   PyObject* keywords) {
  static char* KEYWORD_LIST[] = { (char*)"url", (char*)"data",
				  (char*)"password", nullptr };
  const char* url = nullptr;
  const char* data = nullptr;
  const char* password = nullptr;
  int dataSize = 0;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "|zz#z", KEYWORD_LIST,
				   &url, &data, &dataSize, &password)) {
    return -1;
  }

  std::shared_ptr<StringCache> stringCache(new StringCache);
  std::shared_ptr<ColorCache> colorCache(new ColorCache);
  std::shared_ptr<TextStyleCache> textStyleCache(
      new TextStyleCache(stringCache, colorCache)
  );
  
  if (url && data) {
    PyErr_SetString(PyExc_ValueError,
		    "Cannot specify both \"url\" and \"data\"");
    return -1;
  } else if (url) {
    std::cout << "Load from url" << std::endl;
    std::shared_ptr<pdf_tin::Document> doc(
	loadDocumentFromFile(url, password)
    );
    std::ostringstream origin;
    origin << "url = " << url;
    return doc ? self->impl.init(doc, textStyleCache, origin.str()) : -1;
  } else if (data) {
    std::cout << "Load from data" << std::endl;
    std::shared_ptr<pdf_tin::Document> doc(
	createDocumentFromBytes(data, data + dataSize, password)
    );
    std::ostringstream origin;
    origin << "from " << dataSize << " bytes";
    return doc ? self->impl.init(doc, textStyleCache, origin.str()) : -1;
  } else {
    PyErr_SetString(PyExc_ValueError,
		    "Either \"url\" or \"data\" must have a value");
    return -1;
  }
}

static void pdfTin_pdfDocument_destroy(PdfTin_Document* self) {
  self->impl.~PdfTin_DocumentImpl();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObjectPtr convertToPyDateTime(time_t t) {
  struct tm tmp;

  std::cout << "Convert " << t << " to Python datetime" << std::endl;
  
  localtime_r(&t, &tmp);
  std::cout << "Converted date is " << tmp.tm_year << "/" << tmp.tm_mon
	    << "/" << tmp.tm_mday << " " << tmp.tm_hour << ":"
	    << tmp.tm_min << ":" << tmp.tm_sec << std::endl;
  PyObject* dt = PyDateTime_FromDateAndTime(tmp.tm_year + 1900,
					    tmp.tm_mon + 1,
					    tmp.tm_mday, tmp.tm_hour,
					    tmp.tm_min,
					    tmp.tm_sec, 0);
  if (!dt) {
    std::cout << "Failed to create datetime" << std::endl;
  } else {
    std::cout << "datetime created" << std::endl;
  }

  return PyObjectPtr(dt);
}

PyTypeObject* pdfTin_DocumentType() {
  return &PDFTIN_DOCUMENT_TYPE;
}

PdfTin_DocumentImpl::PdfTin_DocumentImpl()
    : doc(), styleCache(), permanentId(PyStringPtr::none()),
      updateId(PyStringPtr::none()), numPages(0), title(PyStringPtr::none()),
      author(PyStringPtr::none()), subject(PyStringPtr::none()),
      keywords(PyStringPtr::none()), creator(PyStringPtr::none()),
      producer(PyStringPtr::none()), creationDate(PyObjectPtr::none()),
      modificationDate(PyObjectPtr::none()),
      linearized(PyObjectPtr::falseValue()), pageLayout(PyStringPtr::none()),
      pageMode(PyStringPtr::none()), metadata(PyStringPtr::none()),
      pages(PyObjectPtr::none()) {
  // Intentionally left blank
}

int PdfTin_DocumentImpl::init(const std::shared_ptr<pdf_tin::Document>& doc,
			      const std::shared_ptr<TextStyleCache>& styleCache,
			      const std::string& origin) {
  this->doc = doc;
  this->styleCache = styleCache;

  PyStringPtr_INIT(this->origin, origin);
  PyStringPtr_INIT(this->permanentId, doc->permanentId());
  PyStringPtr_INIT(this->updateId, doc->updateId());
  this->numPages = PyIntPtr(doc->numPages());
  //  PyStringPtr_INIT(this->pdfVersion, doc->pdfVersion());
  PyStringPtr_INIT(this->title, doc->title());
  PyStringPtr_INIT(this->author, doc->author());
  PyStringPtr_INIT(this->subject, doc->subject());
  PyStringPtr_INIT(this->keywords, doc->keywords());
  PyStringPtr_INIT(this->creator, doc->creator());
  PyStringPtr_INIT(this->producer, doc->producer());

  //  std::cout << "Initialize creationDate" << std::endl;
  //  PyObjectPtr_INIT(this->creationDate,
  //                   convertToPyDateTime(doc->creationDate()));
  //  std::cout << "Initialize modificationDate" << std::endl;
  //  PyObjectPtr_INIT(this->modificationDate,
  //                   convertToPyDateTime(doc->modificationDate()));
  
  this->linearized = PyObjectPtr::fromBool(doc->isLinearized());
  PyStringPtr_INIT(this->pageLayout, toStr(doc->pageLayout()));
  PyStringPtr_INIT(this->pageMode, toStr(doc->pageMode()));
  PyStringPtr_INIT(this->metadata, PyStringPtr(doc->metadata()));

  std::function<PyObject* (Py_ssize_t)> pageFactory =
      [doc, styleCache](Py_ssize_t index) {
    PyObjectPtr docCapsule(PyCapsule_New((void*)&doc, nullptr, nullptr));
    PyObjectPtr styleCacheCapsule(PyCapsule_New((void*)&styleCache, nullptr,
						nullptr));
    PyObjectPtr args(Py_BuildValue("OOn", docCapsule.get(),
				   styleCacheCapsule.get(), index));
    return PyObject_Call((PyObject*)pdfTin_PageType(), args.get(), nullptr);
  };

  this->pages = PyObjectPtr(
      PdfTin_ImmutableLazyList::create(doc->numPages(), pageFactory)
  );
  if (!this->pages) {
    return -1;
  }
  
  return 0;
}
