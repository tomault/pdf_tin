#include "TextStyleSegment.hpp"
#include "TextStyle.hpp"

static_assert(sizeof(uint32_t) == sizeof(unsigned int),
	      "sizeof(uint32_t) == sizeof(unsigned int)");

static PyObject* pdfTin_TextStyleSegment_new(PyTypeObject* type, PyObject* args,
					     PyObject* keywords) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new((PdfTin_TextStyleSegment*)self.get()) PdfTin_TextStyleSegment;
  return self.release();
}

static int pdfTin_TextStyleSegment_init(PdfTin_TextStyleSegment* self,
					PyObject* args,
					PyObject* keywords) {
  static char* KEYWORD_LIST[] = { (char*)"style", (char*)"start",
				  (char*)"end", nullptr };
  PyObject* style = nullptr;
  uint32_t start = 0;
  uint32_t end = 0;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "OII", KEYWORD_LIST,
				   &style, &start, &end)) {
    return -1;
  }

  if (!PyObject_TypeCheck(style, pdfTin_TextStyleType())) {
    std::ostringstream msg;
    msg << "Value for \"style\" should be a "
	<< pdfTin_TextStyleType()->tp_name << ", not a "
	<< Py_TYPE(style)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return -1;
  }

  if (end < start) {
    PyErr_SetString(PyExc_ValueError, "Value for end < start");
    return -1;
  }
  return self->init(style, start, end);
}

static void pdfTin_TextStyleSegment_destroy(PdfTin_TextStyleSegment* self) {
  self->~PdfTin_TextStyleSegment();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

DECLARE_PyObjectPtr_GETTER(style, PdfTin_TextStyleSegment, style);

PyObject* get_start_value(PdfTin_TextStyleSegment* self, void* closure) {
  return PyLong_FromUnsignedLong(self->start);
}

PyObject* get_end_value(PdfTin_TextStyleSegment* self, void* closure) {
  return PyLong_FromUnsignedLong(self->end);
}

static PyGetSetDef PDFTIN_TEXT_STYLE_SEGMENT_GETSET[] = {
  // name, getter, setter, docstring, closure
  { "style", (getter)get_style_value, 0,
    "(pypdf_tin.TextStyle) Style for text in this segment", 0 },
  { "start", (getter)get_start_value, 0,
    "(int) Start of the segment to which the style applies, measured "
    "in characters from the first character in the text" },
  { "end", (getter)get_end_value, 0,
    "(int) End of the segment to which the style applies, measured "
    "in characters from the last character in the text", 0 },
  { 0, 0, 0, 0, 0 }
};

static PyTypeObject PDFTIN_TEXT_STYLE_SEGMENT_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pdf_tin.TextStyleSegment",     // tp_name
  sizeof(PdfTin_TextStyleSegment),  // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_TextStyleSegment_destroy,  // tp_dealloc
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
  Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,  // tp_flags
  "",                           // tp_doc
  0,                            // tp_traverse
  0,                            // tp_clear
  0,                            // tp_richcompare
  0,                            // tp_weaklistoffset
  0,                            // tp_iter
  0,                            // tp_iternext
  0,                            // tp_methods
  0,                            // tp_members
  PDFTIN_TEXT_STYLE_SEGMENT_GETSET,  // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_TextStyleSegment_init,   // tp_init
  0,                            // tp_alloc
  pdfTin_TextStyleSegment_new,  // tp_new
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

PyTypeObject* pdfTin_TextStyleSegmentType() {
  return &PDFTIN_TEXT_STYLE_SEGMENT_TYPE;
}
