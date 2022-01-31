#ifndef __PYEXT__PDF_TIN__PAGEITERATOR_HPP__
#define __PYEXT__PDF_TIN__PAGEITERATOR_HPP__

#include <memory>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>

struct PdfTin_PageIterator {
  PyObject_HEAD;

  std::shared_ptr<pdf_tin::Document> doc;
  uint32_t current;
};

PyTypeObject* pdfTin_PageIteratorType();

#endif
