#ifndef __PDF_TIN__PAGE_HPP__
#define __PDF_TIN__PAGE_HPP__

#include <pdf_tin/Image.hpp>
#include <pdf_tin/Text.hpp>
#include <pdf_tin/detail/GObjectPtr.hpp>

#include <memory>
#include <vector>

struct _PopplerPage;

namespace pdf_tin {

  class Page {
  public:
    Page(_PopplerPage* page) : page_(page) { }
    Page(const Page&) = default;
    Page(Page&&) = default;
    
    const char* label() const;
    int index() const;
    double width() const;
    double height() const;
    double duration() const;
    std::vector<Text> text() const;
    std::vector<Image> images() const;

    Page& operator=(const Page&) = default;
    Page& operator=(Page&&) = default;
    
  protected:
    Page(const detail::GObjectPtr<_PopplerPage>& page) : page_(page) { }
    Page(detail::GObjectPtr<_PopplerPage>&& page) : page_(std::move(page)) { }
    
  private:
    detail::GObjectPtr<_PopplerPage> page_;
  };
}

#endif
