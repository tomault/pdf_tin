#ifndef __PYEXT__DOCUMENT_HPP__
#define __PYEXT__DOCUMENT_HPP__

#include <pdf_tin/Document.hpp>
#include <Python.h>

struct PyDocument {
  PyObject_HEAD
  pdf_tin::Document doc;
};

#endif
