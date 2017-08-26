#ifndef __PDF_TIN__TEXT_HPP__
#define __PDF_TIN__TEXT_HPP__

#include <pdf_tin/BoundingBox.hpp>
#include <pdf_tin/TextStyleSegment.hpp>
#include <pdf_tin/ImmutableList.hpp>
#include <string>

namespace pdf_tin {

  /** @brief Some text, its bounding box and its styling information */
  class Text {
  public:
    template <typename InputIterator>
    Text(const std::string& text, const BoundingBox& bounds,
	 uint32_t numStyles, InputIterator styles):
        text_(text), bounds_(bounds), styles_(numStyles, styles) {
      // Intentionally left blank
    }

    template <typename FwdIterator>
    Text(const std::string& text, const BoundingBox& bounds,
	 FwdIterator startOfStyles, FwdIterator endOfStyles):
        text_(text), bounds_(bounds), styles_(startOfStyles, endOfStyles) {
      // Intentionally left blank
    }

    Text(const Text&) = default;
    Text(Text&&) = default;

    const std::string& text() const { return text_; }
    const BoundingBox& bounds() const { return bounds_; }
    const ImmutableList<TextStyleSegment>& styles() const { return styles_; }

    //Text& operator=(const Text&) = default;
    //Text& operator=(Text&&) = default;

  private:
    std::string text_;
    BoundingBox bounds_;
    ImmutableList<TextStyleSegment> styles_;
  };
  
}

#endif
