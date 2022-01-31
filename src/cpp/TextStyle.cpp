#include "TextStyle.hpp"

#include "Caches.hpp"
#include "Color.hpp"
#include <iostream>
#include <sstream>

static PyObject* pdfTin_TextStyle_new(PyTypeObject* type, PyObject* args,
				      PyObject* keywords) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  if (!self) {
    return nullptr;
  }
  new((PdfTin_TextStyle*)self.get()) PdfTin_TextStyle;
  return self.release();
}


template <typename DesiredType>
static DesiredType* validateAndExtract(const std::string& argName,
				       PyObject* capsule) {
  if (!PyCapsule_CheckExact(capsule)) {
    std::ostringstream msg;
    msg << "Value for " << argName << " must be a PyCapsule not a "
	<< Py_TYPE(capsule)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return nullptr;
  }

  DesiredType* const value =
    (DesiredType*)PyCapsule_GetPointer(capsule, nullptr);

  if (!value) {
    std::ostringstream msg;
    msg << "Value for " << argName << " is null";
    PyErr_SetString(PyExc_ValueError, msg.str().c_str());
    return nullptr;
  }
      
  return value;
}
				 
static int pdfTin_TextStyle_init(PdfTin_TextStyle* self, PyObject* args,
				 PyObject* keywords) {
  static char* KEYWORD_LIST[] = { (char*)"style", (char*)"string_cache",
				  (char*)"color_cache", nullptr };
  PyObject* styleCapsule = nullptr;
  PyObject* stringCacheCapsule = nullptr;
  PyObject* colorCacheCapsule = nullptr;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "OOO", KEYWORD_LIST,
				   &styleCapsule, &stringCacheCapsule,
				   &colorCacheCapsule)) {
    return -1;
  }

  pdf_tin::TextStyle* const style =
    validateAndExtract<pdf_tin::TextStyle>("style", styleCapsule);
  StringCache* const stringCache =
    validateAndExtract<StringCache>("string_cache", stringCacheCapsule);
  ColorCache* const colorCache =
    validateAndExtract<ColorCache>("color_cache", colorCacheCapsule);

  return self->init(*style, *stringCache, *colorCache);
}

static void pdfTin_TextStyle_destroy(PdfTin_TextStyle* self) {
  self->~PdfTin_TextStyle();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

DECLARE_PyObjectPtr_GETTER(fontName, PdfTin_TextStyle, fontName);
DECLARE_PyBool_GETTER(underlined, PdfTin_TextStyle, underlined);
DECLARE_PyObjectPtr_GETTER(color, PdfTin_TextStyle, color);

static PyObject* get_fontSize_value(PdfTin_TextStyle* self, void* closure) {
  return PyLong_FromSsize_t(self->fontSize);
}

static PyGetSetDef PDFTIN_TEXT_STYLE_GETSET[] = {
  // name, getter, setter, docstring, closure
  { "font_name", (getter)get_fontName_value, 0,
    "(str) Name of the font that should be used to render the text", 0 },
  { "font_size", (getter)get_fontSize_value, 0,
    "(int) Size of the font that should be used to render the text, "
    "in points(?)", 0 },
  { "underlined", (getter)get_underlined_value, 0,
    "(int) Whether the text should be underlined or not.", 0 },
  { "color", (getter)get_color_value, 0,
    "(pypdf_tin.Color) Color that should be used to render the font", 0 },
  { 0, 0, 0, 0, 0 }
};

static PyTypeObject PDFTIN_TEXT_STYLE_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pdf_tin.TextStyle",          // tp_name
  sizeof(PdfTin_TextStyle),     // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_TextStyle_destroy, // tp_dealloc
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
  0,                            // tp_members
  PDFTIN_TEXT_STYLE_GETSET,     // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_TextStyle_init,   // tp_init
  0,                            // tp_alloc
  pdfTin_TextStyle_new,         // tp_new
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

PyTypeObject* pdfTin_TextStyleType() {
  return &PDFTIN_TEXT_STYLE_TYPE;
}

int PdfTin_TextStyle::init(const pdf_tin::TextStyle& style,
			   StringCache& stringCache,
			   ColorCache& colorCache) {
  static_assert(sizeof(Py_ssize_t) > sizeof(uint32_t),
		"sizeof(Py_ssize_t) > sizeof(uint32_t)");
  PyStringPtr fontName(stringCache[style.fontName()]);
  PyObjectPtr color((PyObject*)colorCache[style.color()]);

  return init(fontName.get(), (Py_ssize_t)style.fontSize(), style.underlined(),
	      color.get());
}

int PdfTin_TextStyle::init(PyObject* fontName, Py_ssize_t fontSize,
			   bool underlined, PyObject* color) {
  this->fontName = PyStringPtr(addRef(fontName));
  this->fontSize = fontSize;
  this->underlined = underlined;
  this->color = PyObjectPtr(addRef(color));
  return 0;
}
