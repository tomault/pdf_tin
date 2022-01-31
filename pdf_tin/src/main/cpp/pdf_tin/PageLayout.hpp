#ifndef __PDF_TIN__PAGELAYOUT_HPP__
#define __PDF_TIN__PAGELAYOUT_HPP__

#include <ostream>
#include <string>

namespace pdf_tin {

  enum class PageLayout {
    UNSET,
    SINGLE_PAGE,
    ONE_COLUMN,
    TWO_COLUMN_LEFT,
    TWO_COLUMN_RIGHT,
    TWO_PAGE_LEFT,
    TWO_PAGE_RIGHT,
    UNKNOWN
  };

  std::string toStr(PageLayout layout);
  std::ostream& operator<<(std::ostream& out, PageLayout layout);

}

#endif
