#ifndef __PDF_TIN__TEXTSTYLE_HPP__
#define __PDF_TIN__TEXTSTYLE_HPP__

struct _PopplerTextAttributes;

#include <pdf_tin/Color.hpp>
#include <string>
#include <stdint.h>

namespace pdf_tin {

  /** @brief Styling information (font, underlining, color) for text */
  class TextStyle {
  public:
    TextStyle(const std::string& fontName, uint32_t fontSize,
	      bool underlined, const Color& color):
        fontName_(fontName), fontSize_(fontSize), underlined_(underlined),
	color_(color) {
      // Intentionally left blank  
    }
    TextStyle(std::string&& fontName, uint32_t fontSize, bool underlined,
	      const Color& color):
        fontName_(std::move(fontName)), fontSize_(fontSize),
        underlined_(underlined), color_(color) {
      // Intentionally left blank
    }
      
    TextStyle(const TextStyle&) = default;
    TextStyle(TextStyle&&) = default;

    const std::string& fontName() const { return fontName_; }
    uint32_t fontSize() const { return fontSize_; }
    bool underlined() const { return underlined_; }
    const Color& color() const { return color_; }

    std::size_t hash() const {
      static const std::hash<std::string> HASH_STRING;
      static const std::hash<pdf_tin::Color> HASH_COLOR;
      return HASH_COLOR(color()) +
	         + 31 * ((underlined() ? 108301 : 0)
		     + 31 * (fontSize()
		         + 31 * (HASH_STRING(fontName()) + 17)));
    }

    TextStyle& operator=(const TextStyle&) = default;
    TextStyle& operator=(TextStyle&&) = default;
    
    bool operator==(const TextStyle& other) const {
      return (fontName() == other.fontName()) &&
	     (fontSize() == other.fontSize()) &&
	     (underlined() == other.underlined()) &&
	     (color() == other.color());
    }

    bool operator!=(const TextStyle& other) const {
      return (fontName() != other.fontName()) ||
	     (fontSize() != other.fontSize()) ||
	     (underlined() != other.underlined()) ||
	     (color() != other.color());
    }

    static TextStyle convert(_PopplerTextAttributes* a);

    
  private:
    std::string fontName_;  ///< Name of font
    uint32_t fontSize_;     ///< Size of font (in points?)
    bool underlined_;       ///< Whether text is underlined
    Color color_;           ///< Color of text
  };

  inline std::ostream& operator<<(std::ostream& out, const TextStyle& s) {
    return out << "font=" << s.fontName() << ":" << s.fontSize()
	       << ", underlined=" << (s.underlined() ? "YES" : "NO")
	       << ", color=#" << s.color();
  }
}

namespace std {
  template<>
  struct hash<pdf_tin::TextStyle> {
    auto operator()(const pdf_tin::TextStyle& s) const { return s.hash(); }
  };
}

#endif
