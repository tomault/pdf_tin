#include "PropertyAccessError.hpp"

using namespace pdf_tin;

PropertyAccessError::PropertyAccessError(const std::string& propertyName):
    property_(propertyName), msg_("Unable to access property " + propertyName) {
  // Intentionally left blank
}

PropertyAccessError::~PropertyAccessError() noexcept {
  // Intentionally left blank
}
