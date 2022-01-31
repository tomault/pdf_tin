#ifndef __PDF_TIN__PROPERTYACCESSERROR_HPP__
#define __PDF_TIN__PROPERTYACCESSERROR_HPP__

#include <exception>
#include <string>

namespace pdf_tin {

  class PropertyAccessError : public std::exception {
  public:
    PropertyAccessError(const std::string& propertyName);
    virtual ~PropertyAccessError() noexcept;

    const std::string& property() const { return property_; }
    virtual const char* what() const noexcept { return msg_.c_str(); }

  private:
    std::string property_;
    std::string msg_;
  };
  
}

#endif
