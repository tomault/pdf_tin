#include "Text.hpp"
#include "ImmutableLazyList.hpp"
#include "TextStyleSegment.hpp"
#include <pdf_tin/ImmutableList.hpp>
#include <pdf_tin/TextStyleSegment.hpp>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

static PyObject* pdfTin_Text_new(PyTypeObject* type, PyObject* args,
				 PyObject* keywords) {
  PyObjectPtr self(type->tp_alloc(type, 0));
  new ((PdfTin_ImmutableLazyList*)self.get()) PdfTin_Text;
  return self.release();
}

static int pdfTin_Text_init(PdfTin_Text* self, PyObject* args,
			     PyObject* keywords) {
  static char* KEYWORD_LIST[] = { (char*)"text", (char*)"style_cache",
					nullptr };
  static PyObject* textCapsule = nullptr;
  static PyObject* styleCacheCapsule = nullptr;

  if (!PyArg_ParseTupleAndKeywords(args, keywords, "OO", KEYWORD_LIST,
				   &textCapsule, &styleCacheCapsule)) {
    return -1;
  }

  if (!PyCapsule_CheckExact(textCapsule)) {
    std::ostringstream msg;
    msg << "Value for \"text\" has incorrect type " << Py_TYPE(self)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return -1;
  }

  pdf_tin::Text* const text =
    (pdf_tin::Text*)PyCapsule_GetPointer(textCapsule, nullptr);

  if (!text) {
    PyErr_SetString(PyExc_ValueError, "Value for \"text\" is null");
    return -1;
  }

  if (!PyCapsule_CheckExact(styleCacheCapsule)) {
    std::ostringstream msg;
    msg << "Value for \"style_cache\" has incorrect type "
	<< Py_TYPE(self)->tp_name;
    PyErr_SetString(PyExc_TypeError, msg.str().c_str());
    return -1;
  }
  
  TextStyleCache* const textStyleCache =
    (TextStyleCache*)PyCapsule_GetPointer(styleCacheCapsule, nullptr);

  if (!textStyleCache) {
    PyErr_SetString(PyExc_ValueError, "Value for \"style_cache\" is null");
    return -1;
  }

  return self->init(*text, *textStyleCache);
}

static void pdfTin_Text_destroy(PdfTin_Text* self) {
  self->~PdfTin_Text();
  Py_TYPE(self)->tp_free((PyObject*)self);
}

DECLARE_PyObjectPtr_GETTER(text, PdfTin_Text, text);
DECLARE_PyObjectPtr_GETTER(styles, PdfTin_Text, styles);

PyObject* get_left_boundary_value(PdfTin_Text* self, void* closure) {
  return PyFloat_FromDouble(self->bounds.left());
}

PyObject* get_right_boundary_value(PdfTin_Text* self, void* closure) {
  return PyFloat_FromDouble(self->bounds.right());
}

PyObject* get_top_boundary_value(PdfTin_Text* self, void* closure) {
  return PyFloat_FromDouble(self->bounds.top());
}

PyObject* get_bottom_boundary_value(PdfTin_Text* self, void* closure) {
  return PyFloat_FromDouble(self->bounds.bottom());
}

PyObject* get_text_width_value(PdfTin_Text* self, void* closure) {
  return PyFloat_FromDouble(self->bounds.width());
}

PyObject* get_text_height_value(PdfTin_Text* self, void* closure) {
  return PyFloat_FromDouble(self->bounds.height());
}

static PyGetSetDef PDFTIN_TEXT_GETSET[] = {
  // name, getter, setter, docstring, closure
  { "text", (getter)get_text_value, 0, "(str) The text itself", 0 },
  { "styles", (getter)get_styles_value, 0,
    "(sequence of pypdf_tin.TextStyleSegment) Styles applied to the text",
    0 },
  { "left", (getter)get_left_boundary_value, 0,
    "(float) Left boundary of the text's bounding box", 0 },
  { "right", (getter)get_right_boundary_value, 0,
    "(float) Right boundary of the text's bounding box", 0 },
  { "top", (getter)get_top_boundary_value, 0, 
    "(float) Top boundary of the text's bounding box", 0 },
  { "bottom", (getter)get_bottom_boundary_value, 0,
    "(float) Bottom boundary of the text's bounding box", 0 },
  { "width", (getter)get_text_width_value, 0,
    "(float) Width of the text's bounding box", 0 },
  { "height", (getter)get_text_height_value, 0,
    "(float) Height of the text's bounding box", 0 },
  { 0, 0, 0, 0, 0 }
};

static PyTypeObject PDFTIN_TEXT_TYPE = {
  PyVarObject_HEAD_INIT(NULL, 0)  
  "pdf_tin.Text",               // tp_name
  sizeof(PdfTin_Text),          // tp_basicsize
  0,                            // tp_itemsize
  (destructor)pdfTin_Text_destroy, // tp_dealloc
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
  PDFTIN_TEXT_GETSET,           // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)pdfTin_Text_init,   // tp_init
  0,                            // tp_alloc
  pdfTin_Text_new,              // tp_new
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

PyTypeObject* pdfTin_TextType() {
  return &PDFTIN_TEXT_TYPE;
}

static PyObject* createStyle(const pdf_tin::TextStyleSegment& segment,
			     TextStyleCache& styleCache) {
  PyObjectPtr style((PyObject*)styleCache[segment]);
  PyObjectPtr args(Py_BuildValue("OII", style.get(), segment.start(),
				 segment.end()));
  return PyObject_Call((PyObject*)pdfTin_TextStyleSegmentType(), args.get(),
		       nullptr);
}

int PdfTin_Text::init(const pdf_tin::Text& source,
		      TextStyleCache& styleCache) {
  PyStringPtr text(source.text());
  const auto& segments = source.styles();
  PyObjectPtr styles(
      PdfTin_ImmutableLazyList::createPrepopulated(
	  source.styles().size(),
	  [&segments, &styleCache](Py_ssize_t i) {
	    return createStyle(segments[i], styleCache);
	  }
      )
  );
  
  if (!text || !styles) {
    return -1;
  }
  return init(text.get(), styles.get(), source.bounds());
}
    
PyObject* PdfTin_Text::create(const pdf_tin::Text& source,
			      TextStyleCache& styleCache) {
  PyObjectPtr textCapsule(PyCapsule_New((void*)&source, nullptr, nullptr));
  PyObjectPtr styleCacheCapsule(PyCapsule_New((void*)&styleCache, nullptr,
					      nullptr));
  PyObjectPtr args(Py_BuildValue("OO", textCapsule.get(),
				 styleCacheCapsule.get()));
  return PyObject_Call((PyObject*)pdfTin_TextType(), args.get(), nullptr);
}

