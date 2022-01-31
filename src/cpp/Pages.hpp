#ifndef __PYEXT__PDF_TIN__PAGES_HPP__
#define __PYEXT__PDF_TIN__PAGES_HPP__

#incldue <PyObjectPtr.hpp>
#include <pdf_tin/Document.hpp>
#include <memory>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>

struct PdfTin_PagesImpl {
  std::shared_ptr<pdf_tin::Document> doc;
  PyObjectPtr cache;

  int init(const std::shared_ptr<pdf_tin::Document>& doc);
  PyObject* getPage(Py_ssize_t index);
};

struct PdfTin_Pages {
  PyObject_HEAD;

  PdfTin_PagesImpl impl;
};

struct PdfTin_PagesIterator {
  PyObject_HEAD;

  PyObjectPtr pages;
  Py_ssize_t current;

  int init(PdfTin_Pages* pages, Py_ssize_t current);
  PyObject* next();
};

PyTypeObject* pdfTin_PagesType();
PyTypeObject* pdfTin_PagesIteratorType();

#endif

