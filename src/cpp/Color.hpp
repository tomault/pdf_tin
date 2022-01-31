#ifndef __PYEXT__PDF_TIN__COLOR_HPP__
#define __PYEXT__PDF_TIN__COLOR_HPP__

#include <stdint.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

struct PdfTin_Color {
  PyObject_HEAD;
  
  uint16_t red;
  uint16_t green;
  uint16_t blue;
};

PyTypeObject* pdfTin_ColorType();

#endif

