#ifndef __PDF_TIN__TEXTSTYLESEGMENT_HPP__
#define __PDF_TIN__TEXTSTYLESEGMENT_HPP__

#include <pdf_tin/TextStyle.hpp>
#include <ostream>
#include <stdint.h>

struct _PopplerTextAttributes;

namespace pdf_tin {

  /** @brief A text style applied to a segment of text */
  class TextStyleSegment : public TextStyle {
  public:
    TextStyleSegment(const std::string& fontName, uint32_t fontSize,
		     bool underlined, const Color& color, uint32_t startOffset,
		     uint32_t endOffset):
        TextStyle(fontName, fontSize, underlined, color),
	startOffset_(startOffset), endOffset_(endOffset) {
      // Intentionally left blank  
    }
    TextStyleSegment(std::string&& fontName, uint32_t fontSize, bool underlined,
		     const Color& color, uint32_t startOffset,
		     uint32_t endOffset):
        TextStyle(fontName, fontSize, underlined, color),
        startOffset_(startOffset), endOffset_(endOffset) {
      // Intentionally left blank
    }
    TextStyleSegment(const TextStyle& style, uint32_t startOffset,
		     uint32_t endOffset):
        TextStyle(style), startOffset_(startOffset), endOffset_(endOffset) {
      // Intentionally left blank
    }
    TextStyleSegment(const TextStyleSegment&) = default;
    TextStyleSegment(TextStyleSegment&&) = default;

    uint32_t start() const { return startOffset_; }
    uint32_t end() const { return endOffset_; }

    bool operator==(const TextStyleSegment& other) const {
      return TextStyle::operator==(other) && (start() == other.start()) &&
	     (end() == other.end());
    }

    bool operator!=(const TextStyleSegment& other) const {
      return TextStyle::operator!=(other) || (start() != other.start()) ||
	     (end() != other.end());
    }
    
    TextStyleSegment& operator=(const TextStyleSegment&) = default;
    TextStyleSegment& operator=(TextStyleSegment&&) = default;

    static TextStyleSegment convert(_PopplerTextAttributes* a);

  private:
    uint32_t startOffset_;
    uint32_t endOffset_;
  };
  
}

#endif
