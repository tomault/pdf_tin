#ifndef __PDF_TIN__BOUUNDINGBOX_HPP__
#define __PDF_TIN__BOUUNDINGBOX_HPP__

struct _PopplerRectangle;

namespace pdf_tin {

  class BoundingBox {
  public:
    BoundingBox(double left, double bottom, double right, double top);
    BoundingBox(const _PopplerRectangle& bounds);
    BoundingBox(const BoundingBox&) = default;

    double left() const { return left_; }
    double right() const { return right_; }
    double top() const { return top_; }
    double bottom() const { return bottom_; }

    double width() const { return right() - left(); }
    double height() const { return bottom() - top(); }

    _PopplerRectangle* toRect(_PopplerRectangle* r) const;
    
    BoundingBox& operator=(const BoundingBox&) = default;
    BoundingBox& operator=(const _PopplerRectangle& other);
    
  private:
    double left_;
    double right_;
    double top_;
    double bottom_;
  };

  
}

#endif
