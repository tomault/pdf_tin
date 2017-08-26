#include "TextStyleSegment.hpp"
#include <poppler/glib/poppler.h>

using namespace pdf_tin;

TextStyleSegment TextStyleSegment::convert(_PopplerTextAttributes* a) {
  if (a) {
    return TextStyleSegment(a->font_name, a->font_size, a->is_underlined,
			    Color(a->color), a->start_index, a->end_index);
  } else {
    return TextStyleSegment(std::string(), 0, false, Color(0, 0, 0), 0, 0);
  }
}
