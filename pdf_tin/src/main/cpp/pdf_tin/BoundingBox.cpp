#include "BoundingBox.hpp"
#include <poppler/glib/poppler.h>

using namespace pdf_tin;

BoundingBox::BoundingBox(double left, double top, double right, double bottom):
    left_(left), right_(right), top_(top), bottom_(bottom) {
  // Intentionally left blank
}

BoundingBox::BoundingBox(const _PopplerRectangle& bounds):
    left_(bounds.x1), right_(bounds.x2), top_(bounds.y1), bottom_(bounds.y2) {
  // Intentionally left blank
}

_PopplerRectangle* BoundingBox::toRect(_PopplerRectangle* r) const {
  r->x1 = left_;
  r->y1 = top_;
  r->x2 = right_;
  r->y2 = bottom_;
  return r;
}

BoundingBox& BoundingBox::operator=(const _PopplerRectangle& r) {
  left_ = r.x1;
  right_ = r.x2;
  top_ = r.y1;
  bottom_ = r.y2;
  return *this;
}
