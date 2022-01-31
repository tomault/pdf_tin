#ifndef __PYEXT__PDF_TIN__BOUNDINGBOX_HPP__
#define __PYEXT__PDF_TIN__BOUNDINGBOX_HPP__

#include <PyFloatPtr.hpp>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

struct PdfTin_BoundingBox {
  PyFloatPtr left;
  PyFloatPtr right;
  PyFloatPtr top;
  PyFloatPtr bottom;
};

PyTypeObject* pdfTin_BoundingBoxType();

#endif

