#include "Document.hpp"
#include "DocumentCreationError.hpp"
#include "NoSuchPageError.hpp"
#include "PropertyAccessError.hpp"
#include <poppler/glib/poppler.h>

using namespace pdf_tin;

std::string Document::permanentId() const {
  char* permId = nullptr;
  char* updId = nullptr;

  if (!poppler_document_get_id(doc_.get(), &permId, &updId)) {
    throw PropertyAccessError("Document::permanentId");
  }
  std::string value = permId ? std::string(permId, 32) : std::string();
  g_free((void*)permId);
  g_free((void*)updId);
  return std::move(value);
}

std::string Document::updateId() const {
  char* permId = nullptr;
  char* updId = nullptr;

  if (!poppler_document_get_id(doc_.get(), &permId, &updId)) {
    throw PropertyAccessError("Document::permanentId");
  }
  std::string value = updId ? std::string(updId, 32) : std::string();
  g_free((void*)permId);
  g_free((void*)updId);
  return std::move(value);
}

uint32_t Document::numPages() const {
  return poppler_document_get_n_pages(doc_.get());
}

std::string Document::pdfVersion() const {
  return getStringProperty_(poppler_document_get_pdf_version_string);
}

std::string Document::title() const {
  return getStringProperty_(poppler_document_get_title);
}

std::string Document::author() const {
  return getStringProperty_(poppler_document_get_author);
}

std::string Document::subject() const {
  return getStringProperty_(poppler_document_get_subject);
}

std::string Document::keywords() const {
  return getStringProperty_(poppler_document_get_keywords);
}

std::string Document::creator() const {
  return getStringProperty_(poppler_document_get_creator);
}

std::string Document::producer() const {
  return getStringProperty_(poppler_document_get_producer);
}

time_t Document::creationDate() const {
  return poppler_document_get_creation_date(doc_.get());
}

time_t Document::modificationDate() const {
  return poppler_document_get_modification_date(doc_.get());
}

bool Document::isLinearized() const {
  return poppler_document_is_linearized(doc_.get());
}

PageLayout Document::pageLayout() const {
  switch (poppler_document_get_page_layout(doc_.get())) {
    case POPPLER_PAGE_LAYOUT_UNSET:
      return PageLayout::UNSET;
    case POPPLER_PAGE_LAYOUT_SINGLE_PAGE:
      return PageLayout::SINGLE_PAGE;
    case POPPLER_PAGE_LAYOUT_ONE_COLUMN:
      return PageLayout::ONE_COLUMN;
    case POPPLER_PAGE_LAYOUT_TWO_COLUMN_LEFT:
      return PageLayout::TWO_COLUMN_LEFT;
    case POPPLER_PAGE_LAYOUT_TWO_COLUMN_RIGHT:
      return PageLayout::TWO_COLUMN_RIGHT;
    case POPPLER_PAGE_LAYOUT_TWO_PAGE_LEFT:
      return PageLayout::TWO_PAGE_LEFT;
    case POPPLER_PAGE_LAYOUT_TWO_PAGE_RIGHT:
      return PageLayout::TWO_PAGE_RIGHT;
    default:
      return PageLayout::UNKNOWN;
  }
}

PageMode Document::pageMode() const {
  switch (poppler_document_get_page_mode(doc_.get())) {
    case POPPLER_PAGE_MODE_UNSET:
      return PageMode::UNSET;
    case POPPLER_PAGE_MODE_NONE:
      return PageMode::NONE;
    case POPPLER_PAGE_MODE_USE_OUTLINES:
      return PageMode::USE_OUTLINES;
    case POPPLER_PAGE_MODE_USE_THUMBS:
      return PageMode::USE_THUMBNAILS;
    case POPPLER_PAGE_MODE_FULL_SCREEN:
      return PageMode::FULL_SCREEN;
    case POPPLER_PAGE_MODE_USE_OC:
      return PageMode::USE_OC;
    case POPPLER_PAGE_MODE_USE_ATTACHMENTS:
      return PageMode::USE_ATTACHMENTS;
    default:
      return PageMode::UNKNOWN;
  }
}

std::string Document::metadata() const {
  return getStringProperty_(poppler_document_get_metadata);
}

Page Document::page(uint32_t index) const {
  if (index >= numPages()) {
    throw NoSuchPageError(index);
  }
  _PopplerPage* p = poppler_document_get_page(doc_.get(), (int)index);
  return Page(p);
}

Page Document::page(const std::string& label) const {
  _PopplerPage* p = poppler_document_get_page_by_label(doc_.get(),
						       label.c_str());
  if (!p) {
    throw NoSuchPageError(label);
  }
  return Page(p);
}

Document Document::fromFile(const std::string& uri) {
  return createDocumentFromFile_(uri.c_str(), nullptr);
}

Document Document::fromFile(const std::string& uri,
			    const std::string& password) {
  return createDocumentFromFile_(uri.c_str(), password.c_str());
}

Document Document::fromBytes(char* data, char* end) {
  return createDocumentFromBytes_(data, end, nullptr);
}

Document Document::fromBytes(char* data, char* end,
			     const std::string& password) {
  return createDocumentFromBytes_(data, end, password.c_str());
}

std::string Document::getStringProperty_(
    char* (*readProperty)(_PopplerDocument*)
) const {
  char* text = readProperty(doc_.get());
  std::string value = text ? std::string(text) : std::string();
  g_free((void*)text);
  return std::move(value);
}

Document Document::createDocumentFromFile_(const char* uri,
					   const char* password) {
  static const char* DEFAULT_ERROR_MSG =
      "No error was reported, but the returned document is null";
  GError* error = nullptr;
  _PopplerDocument* doc = poppler_document_new_from_file(uri, password, &error);

  if (error) {
    throw DocumentCreationError(uri, error->message);
  } else if (!doc) {
    throw DocumentCreationError(uri, DEFAULT_ERROR_MSG);
  } else {
    return Document(doc);
  }
}

Document Document::createDocumentFromBytes_(char* data, char* end,
					    const char* password) {
  static const char* DEFAULT_ERROR_MSG = 
      "No error was reported, but the returned document is null";
  GError* error = nullptr;
  _PopplerDocument* doc = poppler_document_new_from_data(data,
							 (int)(end - data),
							 password, &error);
  
  if (error) {
    throw DocumentCreationError(error->message);
  } else if (!doc) {
    throw DocumentCreationError(DEFAULT_ERROR_MSG);
  } else {
    return Document(doc);
  }
}
