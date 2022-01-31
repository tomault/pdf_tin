#ifndef __PYEXT__PDF_TIN__PAGE_HPP__
#define __PYEXT__PDF_TIN__PAGE_HPP__

#include <Caches.hpp>
#include <PyObjectPtr.hpp>
#include <PyStringPtr.hpp>
#include <pdf_tin/Document.hpp>
#include <pdf_tin/Page.hpp>
#include <memory>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

struct PdfTin_PageImpl {
  std::shared_ptr<pdf_tin::Document> doc;
  std::shared_ptr<pdf_tin::Page> page;
  std::shared_ptr<TextStyleCache> styleCache;
  PyStringPtr label;
  PyObjectPtr text;

  PdfTin_PageImpl();
  int init(const std::shared_ptr<pdf_tin::Document>& doc,
	   const std::shared_ptr<pdf_tin::Page>& page,
	   const std::shared_ptr<TextStyleCache>& styleCache);
};

struct PdfTin_Page {
  PyObject_HEAD;

  /** @brief The implementation object for the page
   *
   *  @todo Eliminate the implementation object and move everything into
   *        PdfTin_Page
   */
  PdfTin_PageImpl impl;
};

PyTypeObject* pdfTin_PageType();



#endif
