#ifndef __PDF_TIN__DOCUMENTCREATIONERROR_HPP__
#define __PDF_TIN__DOCUMENTCREATIONERROR_HPP__

#include <exception>
#include <string>

namespace pdf_tin {
  class DocumentCreationError : public std::exception {
  public:
    DocumentCreationError(const std::string& source,
			  const std::string& details);
    DocumentCreationError(const std::string& details);
    virtual ~DocumentCreationError() noexcept;

    const std::string& details() const { return details_; }
    virtual const char* what() const noexcept { return msg_.c_str(); }

  private:
    std::string details_;
    std::string msg_;
  };
  
}

#endif
