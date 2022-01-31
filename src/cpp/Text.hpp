#ifndef __PYEXT__PDF_TIN__TEXT_HPP__
#define __PYEXT__PDF_TIN__TEXT_HPP__

#include <pdf_tin/BoundingBox.hpp>
#include <pdf_tin/Text.hpp>

#include <Caches.hpp>
#include <PyObjectPtr.hpp>
#include <PyStringPtr.hpp>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

struct PdfTin_Text {
  PyObject_HEAD;
  
  PyStringPtr text;
  PyObjectPtr styles;
  pdf_tin::BoundingBox bounds;

  PdfTin_Text(): text(), styles(), bounds(0, 0, 0, 0) { }
  
  int init(const pdf_tin::Text& source, TextStyleCache& styleCache);

  int init(PyObject* text, PyObject* styles,
	   const pdf_tin::BoundingBox& bounds) {
    this->text = PyStringPtr(addRef(text));
    this->styles = PyObjectPtr(addRef(styles));
    this->bounds = bounds;
    return 0;
  }

  static PyObject* create(const pdf_tin::Text& source,
			  TextStyleCache& styleCache);
};

PyTypeObject* pdfTin_TextType();

#endif
