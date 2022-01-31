#ifndef __PDF_TIN__DOCUMENT_HPP__
#define __PDF_TIN__DOCUMENT_HPP__

#include <pdf_tin/Page.hpp>
#include <pdf_tin/PageLayout.hpp>
#include <pdf_tin/PageMode.hpp>
#include <pdf_tin/detail/GObjectPtr.hpp>

struct _PopplerDocument;

namespace pdf_tin {

  class Document {
  public:
    Document(const Document&) = default;
    Document(Document&&) = default;
    
    std::string permanentId() const;
    std::string updateId() const;
    uint32_t numPages() const;
    std::string pdfVersion() const;

    std::string title() const;
    std::string author() const;
    std::string subject() const;
    std::string keywords() const;
    std::string creator() const;
    std::string producer() const;
    time_t creationDate() const;
    time_t modificationDate() const;
    bool isLinearized() const;
    PageLayout pageLayout() const;
    PageMode pageMode() const;
    std::string metadata() const;
    
    Page page(uint32_t index) const;
    Page page(const std::string& label) const;

    Document& operator=(const Document&) = default;
    Document& operator=(Document&&) = default;
    
    static Document fromFile(const std::string& uri);
    static Document fromFile(const std::string& uri,
			     const std::string& password);
    static Document fromBytes(char* data, char* end);
    static Document fromBytes(char* data, char* end,
			      const std::string& password);
			     
  private:
    detail::GObjectPtr<_PopplerDocument> doc_;

    Document(_PopplerDocument* doc): doc_(doc) { }
    
    std::string getStringProperty_(char* (*readProperty)(_PopplerDocument*)) const;

    static Document createDocumentFromFile_(const char* uri,
					    const char* password);
    static Document createDocumentFromBytes_(char* data, char* end,
					     const char* password);
  };

}
#endif
