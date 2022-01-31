#ifndef __PDF_TIN__IMAGE_HPP__
#define __PDF_TIN__IMAGE_HPP__

#include <pdf_tin/BoundingBox.hpp>

namespace pdf_tin {

  class Image {
  public:
    Image(int id, const BoundingBox& bounds): id_(id), bounds_(bounds) { }
    Image(const Image&) = default;

    int id() const { return id_; }
    const BoundingBox& bounds() const { return bounds_; }
    
    Image& operator=(const Image&) = default;

  private:
    int id_;
    BoundingBox bounds_;
  };
  
}

#endif
