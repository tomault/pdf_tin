#ifndef __PYEXT__PDF_TIN__TEXTSTYLE_HPP__
#define __PYEXT__PDF_TIN__TEXTSTYLE_HPP__

#include <pdf_tin/TextStyle.hpp>
#include <PyObjectPtr.hpp>
#include <PyStringPtr.hpp>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

class StringCache;
class ColorCache;

struct PdfTin_TextStyle {
  PyObject_HEAD;

  PyStringPtr fontName;
  Py_ssize_t fontSize;
  bool underlined;
  PyObjectPtr color;

  int init(const pdf_tin::TextStyle& style, StringCache& stringCache,
	   ColorCache& colorCache);

  int init(PyObject* fontName, Py_ssize_t fontSize, bool underlined,
	   PyObject* color);
};

PyTypeObject* pdfTin_TextStyleType();

#endif
