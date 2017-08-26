#include "TextStyle.hpp"
#include <poppler/glib/poppler.h>

using namespace pdf_tin;

TextStyle TextStyle::convert(_PopplerTextAttributes* a) {
  if (a) {
    return TextStyle(a->font_name, a->font_size, a->is_underlined,
		     Color(a->color));
  } else {
    return TextStyle(std::string(), 0, false, Color(0, 0, 0));
  }
}
