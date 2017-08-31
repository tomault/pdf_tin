#ifndef __PYEXT__PDF_TIN__PDFDOCUMENT_HPP__
#define __PYEXT__PDF_TIN__PDFDOCUMENT_HPP__

#include <PyIntPtr.hpp>
#include <PyObjectPtr.hpp>
#include <PyStringPtr.hpp>
#include <pdf_tin/Document.hpp>
#include <memory>

#include <Python.h>
#include <stdint.h>

/** @brief Implementation details for a PdfTin_Document */
struct PdfTin_DocumentImpl {
  /** @brief The underlying document object
   *
   *  This needs to live as long as the PdfDocument or any PdfPages that
   *  represent one of its pages, so it is held in a shared pointer.
   */
  std::shared_ptr<pdf_tin::Document doc>;
  PyStringPtr permanentId;
  PyStringPtr updateId;
  PyIntPtr numPages;
  PyStringPtr title;
  PyStringPtr author;
  PyStringPtr subject;
  PyStringPtr keywords;
  PyStringPtr creator;
  PyStringPtr producer;
  PyObjectPtr creationDate;
  PyObjectPtr modificationDate;
  PyObjectPtr linearized;
  PyStringPtr pageLayout;
  PyStringPtr pageMode;
  PyStringPtr metadata;

  PdfTin_DocumentImpl();
  int init(const std::shared_ptr<pdf_tin::Document>& doc);
};

struct PdfTin_Document {
  PyObject_HEAD

  /** @brief The actual implementation of the PdfTin_Document
   *
   *  Using this implementation object makes it much easier to manage the
   *  component types that make up a PdfTin_Document.  We can create the
   *  entire object with a single placement new and destroy it with a
   *  single call to the destructor for PdfTin_DocumentImpl.
   */
  PdfTin_DocumentImpl impl;
};

PyTypeObject* pdfTin_PdfDocumentType();

#endif
