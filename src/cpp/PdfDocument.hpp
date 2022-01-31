#ifndef __PYEXT__PDF_TIN__PDFDOCUMENT_HPP__
#define __PYEXT__PDF_TIN__PDFDOCUMENT_HPP__

#include <Caches.hpp>
#include <PyIntPtr.hpp>
#include <PyObjectPtr.hpp>
#include <PyStringPtr.hpp>
#include <pdf_tin/Document.hpp>
#include <memory>
#include <string>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>

/** @brief Implementation details for a PdfTin_Document */
struct PdfTin_DocumentImpl {
  /** @brief The underlying document object
   *
   *  This needs to live as long as the PdfDocument or any PdfPages that
   *  represent one of its pages, so it is held in a shared pointer.
   */
  std::shared_ptr<pdf_tin::Document> doc;

  /** @brief Cache for text styles
   *
   *  This cache gets propagated down to the individual pages, which use 
   *  it to construct the TextStyle objects contained in the TextStyleSegment
   *  objects contained in Text objects.  Since most documents only use
   *  a handful of styles, caching them saves a lot of memory (one object
   *  per unique style rather than one object per Text object.
   */
  std::shared_ptr<TextStyleCache> styleCache;

  /** @defgroup PdfDocumentProperties Pre-constructed document properties
   *
   *  @todo Remove these and construct them on the fly in the getter.
   *  @{
   */
  PyStringPtr origin;       ///< Document's origin
  PyStringPtr permanentId;  ///< Document's permanent id
  PyStringPtr updateId;     ///< Document's update id
  PyIntPtr numPages;        ///< Number of pages in document
  PyStringPtr title;        ///< Document's title
  PyStringPtr author;       ///< Document's author
  PyStringPtr subject;      ///< Document's subject
  PyStringPtr keywords;     ///< Document's keywords
  PyStringPtr creator;      ///< Document's creator
  PyStringPtr producer;     ///< Document's producer
  PyObjectPtr creationDate;       ///< Document's creation date
  PyObjectPtr modificationDate;   ///< Document's modification date
  PyObjectPtr linearized;   ///< Whether document is linearized
  PyStringPtr pageLayout;   ///< Document's page layout
  PyStringPtr pageMode;     ///< Document's page mode
  PyStringPtr metadata;     ///< Any metadata supplied for the document
  /** @} */

  /** @brief Lazily-populated list of the document's oages */
  PyObjectPtr pages;

  PdfTin_DocumentImpl();

  /** @brief Initialize the document
   *
   *  This method sets the internal document structure ("doc") and
   *  text style cache.  It precomputes the the document properties and
   *  initializes the page list.
   *
   *  @returns 0 on success, nonzero on failure.
   */
  int init(const std::shared_ptr<pdf_tin::Document>& doc,
	   const std::shared_ptr<TextStyleCache>& styleCache,
	   const std::string& origin);
};

/** @brief Represents a PDF document
 *
 *  PdfTin_Document is ultimately a wrapper around pdf_tin::Document
 *  that exposes that document in a way that's easy for Python developers
 *  to consume it.
 */
struct PdfTin_Document {
  PyObject_HEAD;

  /** @brief The actual implementation of the PdfTin_Document
   *
   *  Using this implementation object makes it much easier to manage the
   *  component types that make up a PdfTin_Document.  We can create the
   *  entire object with a single placement new and destroy it with a
   *  single call to the destructor for PdfTin_DocumentImpl.
   *
   *  @todo  Eliminate the implemenation object and move everything
   *         from the implementation object into the document itself.
   */
  PdfTin_DocumentImpl impl;
};

/** @brief Returns the Python type object for PdfTin_Document */
PyTypeObject* pdfTin_DocumentType();

#endif
