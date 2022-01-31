#ifndef __PDF_TIN__DETAIL__GLISTPTR_HPP__
#define __PDF_TIN__DETAIL__GLISTPTR_HPP__

#include <functional>
#include <memory>
#include <glib.h>

namespace pdf_tin {
  namespace detail {

    typedef std::unique_ptr<GList, std::function<void (GList*)> > GListPtr;

  }
}

#endif
