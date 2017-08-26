#include "PageLayout.hpp"
#include <algorithm>
#include <stdint.h>

namespace {
  const char* label(uint32_t n) {
    static const char* TEXT[] = {
      "UNSET", "SINGLE_PAGE", "ONE_COLUMN", "TWO_COLUMN_LEFT",
      "TWO_COLUMN_RIGHT", "TWO_PAGE_LEFT", "TWO_PAGE_RIGHT", "UNKNOWN"
    };
    static const uint32_t MAX_INDEX = sizeof(TEXT)/sizeof(const char*);
    return TEXT[std::min(n, MAX_INDEX)];
  }
}

std::string pdf_tin::toStr(PageLayout layout) {
  return std::string(label((uint32_t)layout));
}

std::ostream& pdf_tin::operator<<(std::ostream& out, PageLayout layout) {
  return out << label((uint32_t)layout);
}

