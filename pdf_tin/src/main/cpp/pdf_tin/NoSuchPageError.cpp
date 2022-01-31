#include "NoSuchPageError.hpp"
#include <sstream>

using namespace pdf_tin;

namespace {
  std::string createLabel(uint32_t n) {
    std::ostringstream tmp;
    tmp << n;
    return tmp.str();
  }

  std::string createMessage(uint32_t n) {
    std::ostringstream tmp;
    tmp << "No page at index " << n;
    return tmp.str();
  }
}

NoSuchPageError::NoSuchPageError(uint32_t index):
    page_(createLabel(index)), msg_(createMessage(index)) {
  // Intentionally left blank
}

NoSuchPageError::NoSuchPageError(const std::string& label):
    page_(label), msg_("No page with label \"" + label + "\"") {
  // Intentionally left blank
}

NoSuchPageError::~NoSuchPageError() noexcept {
  // Intentionally left blank
}
