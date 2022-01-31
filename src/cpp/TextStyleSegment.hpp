#ifndef __PYEXT__PDF_TIN__TEXTSTYLESEGMENT_HPP__
#define __PYEXT__PDF_TIN__TEXTSTYLESEGMENT_HPP__

#include <PyObjectPtr.hpp>
#include <stdint.h>

struct PdfTin_TextStyleSegment {
  PyObject_HEAD;

  PyObjectPtr style;
  uint32_t start;
  uint32_t end;
  
  int init(PyObject* style, uint32_t start, uint32_t end) {
    this->style = PyObjectPtr(style);
    this->start = start;
    this->end = end;
    return 0;
  }
};

PyTypeObject* pdfTin_TextStyleSegmentType();

#endif
