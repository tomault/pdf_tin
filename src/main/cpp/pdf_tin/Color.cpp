#include "Color.hpp"
#include <poppler/glib/poppler.h>

using namespace pdf_tin;

Color::Color(const _PopplerColor& c):
    red_(c.red), green_(c.green), blue_(c.blue) {
  // Intentionally left blank
}
