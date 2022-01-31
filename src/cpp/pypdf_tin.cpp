#include "Color.hpp"
#include "ImmutableLazyList.hpp"
#include "PdfDocument.hpp"
#include "Page.hpp"
#include "Text.hpp"
#include "TextStyle.hpp"
#include "TextStyleSegment.hpp"

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <datetime.h>
#include <iostream>

static int registerType(PyObject* module, const char* name,
			PyTypeObject* type) {
  if (PyType_Ready(type) < 0) {
    std::cout << "Call to PyType_Ready() failed for " << name << std::endl;
    return -1;
  }

  Py_INCREF(type);
  if (PyModule_AddObject(module, name, (PyObject*)type) < 0) {
    std::cout << "Failed to add " << name << " to module" << std::endl;
    Py_DECREF(type);
    return -1;
  }

  return 0;
}

#define REGISTER_TYPE(MODULE, NAME, TYPE)                         \
  if (registerType(MODULE, NAME, TYPE)) {		       \
    Py_DECREF(module);					       \
    return nullptr;					       \
  }

static PyModuleDef PYPDF_TIN_MODULE {
  PyModuleDef_HEAD_INIT,
  .m_name = "pypdf_tin",
  .m_doc = "Python interface to poppler",
  .m_size = -1
};

PyMODINIT_FUNC PyInit_pypdf_tin() {
  PyObject* module = PyModule_Create(&PYPDF_TIN_MODULE);

  if (!module) {
    return nullptr;
  }
  
  PyDateTime_IMPORT;

  REGISTER_TYPE(module, "Color", pdfTin_ColorType());
  REGISTER_TYPE(module, "Document", pdfTin_DocumentType());
  REGISTER_TYPE(module, "Page", pdfTin_PageType());
  REGISTER_TYPE(module, "_ImmutableLazyList", pdfTin_ImmutableLazyListType());
  REGISTER_TYPE(module, "_ImmutableLazyListIterator",
		pdfTin_ImmutableLazyListIteratorType());
  REGISTER_TYPE(module, "Text", pdfTin_TextType());
  REGISTER_TYPE(module, "TextStyle", pdfTin_TextStyleType());
  REGISTER_TYPE(module, "TextStyleSegment", pdfTin_TextStyleSegmentType());
  
  return module;
}

