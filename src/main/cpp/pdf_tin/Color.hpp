#ifndef __PDF_TIN__COLOR_HPP__
#define __PDF_TIN__COLOR_HPP__

#include <iomanip>
#include <ostream>
#include <stdint.h>

struct _PopplerColor;

namespace pdf_tin {

  class Color {
  public:
    explicit Color(const _PopplerColor& c);
    Color(uint16_t red, uint16_t green, uint16_t blue):
        red_(red), green_(green), blue_(blue) {
      // Intentionally left blank
    }

    uint16_t red() const { return red_; }
    uint16_t green() const { return green_; }
    uint16_t blue() const { return blue_; }

    bool operator==(const Color& other) const {
      return (red() == other.red()) && (green() == other.green()) &&
	     (blue() == other.blue());
    }

    bool operator!=(const Color& other) const {
      return (red() != other.red()) || (green() != other.green()) ||
	     (blue() != other.blue());
    }
    
  private:
    uint16_t red_;
    uint16_t green_;
    uint16_t blue_;
  };

  inline std::ostream& operator<<(std::ostream& out, const Color& c) {
    return out << std::hex << std::setw(2) << std::setfill('0') << c.red()
	       << std::hex << std::setw(2) << std::setfill('0') << c.green()
	       << std::hex << std::setw(2) << std::setfill('0') << c.blue();
  }
  
}

#endif
