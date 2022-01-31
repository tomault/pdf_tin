#include "PageMode.hpp"
#include <algorithm>
#include <stdint.h>

using namespace pdf_tin;

namespace {
  const char* label(uint32_t n) {
    static const char* TEXT[] = { "UNSET", "NONE", "USE_OUTLINES",
				  "USE_THUMBNAILS", "FULL_SCREEN", "USE_OC",
				  "USE_ATTACHMENTS", "UNKNOWN" };
    static const uint32_t MAX_INDEX = sizeof(TEXT)/sizeof(const char*) - 1;
    return TEXT[std::min(n, MAX_INDEX)];
  }
}

std::string pdf_tin::toStr(PageMode mode) {
  return std::string(label((uint32_t)mode));
}

std::ostream& pdf_tin::operator<<(std::ostream& out, PageMode mode) {
  return out << label((uint32_t)mode);
}
