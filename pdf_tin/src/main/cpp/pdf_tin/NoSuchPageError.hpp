#ifndef __PDF_TIN__NOSUCHPAGEERROR_HPP__
#define __PDF_TIN__NOSUCHPAGEERROR_HPP__

#include <exception>
#include <string>
#include <stdint.h>

namespace pdf_tin {

  class NoSuchPageError : public std::exception {
  public:
    NoSuchPageError(uint32_t index);
    NoSuchPageError(const std::string& label);
    virtual ~NoSuchPageError() noexcept;

    const std::string& page() const { return page_; }
    virtual const char* what() const noexcept { return msg_.c_str(); }

  private:
    std::string page_;
    std::string msg_;    
  };
}

#endif
