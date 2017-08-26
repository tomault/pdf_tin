#include "DocumentCreationError.hpp"

using namespace pdf_tin;

DocumentCreationError::DocumentCreationError(const std::string& source,
					     const std::string& details):
    details_(details), msg_(
        "Failed to create document \"" + source + "\": " + details
    ) {
  // Intentionally left blank
}

DocumentCreationError::DocumentCreationError(const std::string& details):
    details_(details), msg_("Failed to create document: " + details) {
  // Intentionally left blank
}

DocumentCreationError::~DocumentCreationError() noexcept {
  // Intentionally left blank
}
