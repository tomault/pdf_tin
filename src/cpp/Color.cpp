#include "Color.hpp"
#include "PyObjectPtr.hpp"
#include <memory>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

static_assert(sizeof(uint16_t) == sizeof(unsigned short),
	      "sizeof(uint16_t) == sizeof(unsigned short)");

static PyObject* pdfTin_Color_new(PyTypeObject* type, PyObject* args,
				  PyObject* keywords) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new((PdfTin_Color*)self.get()) PdfTin_Color;
  return self.release();
}

static int pdfTin_Color_init(PdfTin_Color* self, PyObject* args,
			     PyObject* keywords) {
  static char* KEYWORD_LIST[] = { (char*)"red", (char*)"green", (char*)"blue",
				  nullptr };
  uint16_t red, green, blue;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "HHH", KEYWORD_LIST,
				   &red, &green, &blue)) {
    return -1;
  }

  self->red = red;
  self->green = green;
  self->blue = blue;

  return 0;
}

static void pdfTin_Color_destroy(PdfTin_Color* self) {
  self->~PdfTin_Color();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyMemberDef PDFTIN_COLOR_MEMBERS[] = {
  // name, type, offset, flags, doc
  { "red", T_USHORT, offsetof(PdfTin_Color, red), READONLY,
    "Red component of color" },
  { "green", T_USHORT, offsetof(PdfTin_Color, green), READONLY,
    "Green component of color" },
  { "blue", T_USHORT, offsetof(PdfTin_Color, blue), READONLY,
    "Blue component of color" },
  { 0, 0, 0, 0, 0 }
};

static PyTypeObject PDFTIN_COLOR_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pdf_tin.Color",              // tp_name
  sizeof(PdfTin_Color),         // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_Color_destroy, // tp_dealloc
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
  0,                            // tp_methods
  PDFTIN_COLOR_MEMBERS,         // tp_members
  0,                            // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_Color_init,  // tp_init
  0,                            // tp_alloc
  pdfTin_Color_new,             // tp_new
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

PyTypeObject* pdfTin_ColorType() {
  return &PDFTIN_COLOR_TYPE;
}
