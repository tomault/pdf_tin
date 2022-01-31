#ifndef __PDF_TIN__PAGEMODE_HPP__
#define __PDF_TIN__PAGEMODE_HPP__

#include <ostream>
#include <string>

namespace pdf_tin {

  enum class PageMode {
    UNSET,
    NONE,
    USE_OUTLINES,
    USE_THUMBNAILS,
    FULL_SCREEN,
    USE_OC,
    USE_ATTACHMENTS,
    UNKNOWN
  };

  std::string toStr(PageMode mode);
  std::ostream& operator<<(std::ostream& out, PageMode mode);
}

#endif
