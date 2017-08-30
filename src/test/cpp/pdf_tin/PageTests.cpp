#include <pdf_tin/Document.hpp>
#include <pdf_tin/NoSuchPageError.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <iomanip>

using namespace pdf_tin;

namespace pdf_tin {
  std::ostream& operator<<(std::ostream& out,
			   const TextStyleSegment& style) {
    return out << "(font=" << style.fontName() << ":" << style.fontSize()
	       << ", underlined=" << (style.underlined() ? "yes" : "no")
	       << ", color=" << style.color() << ", bounds=" << style.start()
	       << ":" << style.end() << ")";
  }
  
  std::ostream& operator<<(std::ostream& out,
			   const ImmutableList<TextStyleSegment>& styles) {
    out << "{";
    for (auto i = styles.begin(); i != styles.end(); ++i) {
      if (i != styles.begin()) {
	out << ", ";
      }
      out << *i;
    }
    return out << "}";
  }
  
  std::ostream& operator<<(std::ostream& out, const Text& t) {
    return out << "{[" << t.text() << "] at (" << std::setprecision(6)
	       << t.bounds().left() << ", " << std::setprecision(6)
	       << t.bounds().top() << ") to (" << std::setprecision(6)
	       << t.bounds().right() << ", " << std::setprecision(6)
	       << t.bounds().bottom() << ") with styles " << t.styles()
	       << "}";
  }
}

namespace {
  std::string getCurrentDir() {
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    return std::string(buf);
  }
  
  std::string findResourceDir() {
    const char* rdVar = getenv("TEST_RESOURCE_DIR");
    if (!rdVar || !rdVar[0]) {
      return getCurrentDir();
    } else {
      std::string dir;
      if (rdVar[0] == '/') {
	dir = std::string(rdVar);
      } else {
	dir = getCurrentDir() + "/" + std::string(rdVar);
      }
      if ((dir.size() > 1) && (dir[dir.size() -1 ] == '/')) {
	dir = dir.substr(0, dir.size() - 1);
      }
      return std::move(dir);
    }
  }

  const std::string& getResourceDir() {
    static const std::string RESOURCE_DIR = findResourceDir();
    return RESOURCE_DIR;
  }
  
  std::string getPdfUrl(const std::string& name) {
    std::ostringstream url;
    url << "file://" << getResourceDir() << "/" << name;
    return url.str();
  }

  bool areClose(double truth, double value) {
    return fabs(truth - value) < 1e-03;
  }
  
  bool areClose(const BoundingBox& truth, const BoundingBox& box) {
    return areClose(truth.left(), box.left()) &&
           areClose(truth.right(), box.right()) &&
           areClose(truth.top(), box.top()) &&
           areClose(truth.bottom(), box.bottom());
  }


  bool areSame(const ImmutableList<TextStyleSegment>& truth,
	       const ImmutableList<TextStyleSegment>& styles) {
    return (truth.size() == styles.size()) &&
           std::equal(truth.begin(), truth.end(), styles.begin());
  }

  std::string join(const std::string& sep,
		   const std::vector<std::string>& items) {
    std::ostringstream msg;
    for (auto i = items.begin(); i != items.end(); ++i) {
      if (i != items.begin()) {
	msg << sep;
      }
      msg << *i;
    }
    return msg.str();
  }
  
  ::testing::AssertionResult textAreSame(uint32_t index, const Text& truth,
					 const Text& text) {
    std::vector<std::string> errors;

    if (text.text() != truth.text()) {
      errors.push_back("Text does not match");
    }
    if (!areClose(truth.bounds(), text.bounds())) {
      std::ostringstream msg;
      msg << "Bounds do not match -- deltas ("
	  << (truth.bounds().left() - text.bounds().left()) << ", "
	  << (truth.bounds().top() - text.bounds().top()) << ", "
	  << (truth.bounds().right() - text.bounds().right()) << ", "
	  << (truth.bounds().bottom() - text.bounds().bottom()) << ")";
      errors.push_back(msg.str());
    }
    if (!areSame(truth.styles(), text.styles())) {
      errors.push_back("Styles do not match");
    }
    if (errors.empty()) {
      return ::testing::AssertionSuccess();
    } else {
      return ::testing::AssertionFailure()
	<< "Texts at index #" << index << " is incorrect.  It is "
	<< text << ", but it should be " << truth << " (" << join("; ", errors)
	<< ")";
    }
  }
}

TEST(PageTests, GetPageByIndex) {
  Document doc = Document::fromFile(getPdfUrl("TestDoc1.pdf"));
  Page page = doc.page(0);

  EXPECT_EQ("1", std::string(page.label()));
  EXPECT_EQ(0, page.index());
  EXPECT_EQ(612.0, page.width());
  EXPECT_EQ(792.0, page.height());
  EXPECT_EQ(-1.0, page.duration());

  page = doc.page(1);

  EXPECT_EQ("2", std::string(page.label()));
  EXPECT_EQ(1, page.index());
  EXPECT_EQ(612.0, page.width());
  EXPECT_EQ(792.0, page.height());
  EXPECT_EQ(-1.0, page.duration());

  EXPECT_THROW(doc.page(2), NoSuchPageError);
  // std::cout << "label = [" << page.label() << "]" << std::endl
  // 	    << "index = " << page.index() << std::endl
  // 	    << "width = " << page.width() << std::endl
  // 	    << "height = " << page.height() << std::endl
  // 	    << "duration = " << page.duration() << std::endl;
}

TEST(PageTests, GetPageByLabel) {
  Document doc = Document::fromFile(getPdfUrl("TestDoc1.pdf"));
  Page page = doc.page("1");

  EXPECT_EQ("1", std::string(page.label()));
  EXPECT_EQ(0, page.index());
  EXPECT_EQ(612.0, page.width());
  EXPECT_EQ(792.0, page.height());
  EXPECT_EQ(-1.0, page.duration());

  page = doc.page("2");

  EXPECT_EQ("2", std::string(page.label()));
  EXPECT_EQ(1, page.index());
  EXPECT_EQ(612.0, page.width());
  EXPECT_EQ(792.0, page.height());
  EXPECT_EQ(-1.0, page.duration());

  EXPECT_THROW(doc.page("3"), NoSuchPageError);
}

TEST(PageTests, GetTextOnPage) {
  Document doc = Document::fromFile(getPdfUrl("TestDoc1.pdf"));
  Page page = doc.page(0);
  TextStyleSegment liberationSerif12("LiberationSerif", 12, false,
				     Color(0, 0, 0), 0, 0);
  TextStyleSegment liberationSerif10("LiberationSerif", 10, false,
				     Color(0, 0, 0), 0, 0);
  std::vector<Text> text = page.text();
  std::vector<Text> truth{
    Text{ "T", BoundingBox( 56.800, 57.208,  64.096, 70.492),
	  { liberationSerif12  } },
    Text{ "h", BoundingBox( 64.096, 57.208,  70.096, 70.492),
	  { liberationSerif12 } },
    Text{ "i", BoundingBox( 70.096, 57.208,  73.396, 70.492),
	  { liberationSerif12 } },
    Text{ "s", BoundingBox( 73.396, 57.208,  78.064, 70.492),
	  { liberationSerif12 } },
    Text{ "i", BoundingBox( 81.184, 57.208,  84.484, 70.492),
	  { liberationSerif12 } },
    Text{ "s", BoundingBox( 84.484, 57.208,  89.152, 70.492),
	  { liberationSerif12 } },
    Text{ "a", BoundingBox( 92.176, 57.208,  97.492, 70.492),
	  { liberationSerif12 } },
    Text{ "t", BoundingBox(100.480, 57.208, 103.780, 70.492),
	  { liberationSerif12 } },
    Text{ "e", BoundingBox(103.780, 57.208, 109.084, 70.492),
	  { liberationSerif12 } },
    Text{ "s", BoundingBox(109.084, 57.208, 113.776, 70.492),
	  { liberationSerif12 } },
    Text{ "t", BoundingBox(113.776, 57.208, 117.076, 70.492),
	  { liberationSerif12 } },
    Text{ ".", BoundingBox(117.076, 57.208, 120.076, 70.492),
	  { liberationSerif12 } },
    Text{ "T", BoundingBox( 56.800, 70.990,  62.900, 82.060),
	  { liberationSerif10 } },
    Text{ "h", BoundingBox( 62.900, 70.990,  67.900, 82.060),
	  { liberationSerif10 } },
    Text{ "i", BoundingBox( 67.900, 70.990,  70.690, 82.060),
	  { liberationSerif10 } },
    Text{ "s", BoundingBox( 70.690, 70.990,  74.580, 82.060),
	  { liberationSerif10 } },
    Text{ "i", BoundingBox( 77.080, 70.990,  79.870, 82.060),
	  { liberationSerif10 } },
    Text{ "s", BoundingBox( 79.870, 70.990,  83.760, 82.060),
	  { liberationSerif10 } },
    Text{ "i", BoundingBox( 86.260, 70.990,  88.960, 82.060),
	  { liberationSerif10 } },
    Text{ "n", BoundingBox( 88.960, 70.990,  93.960, 82.060),
	  { liberationSerif10 } },
    Text{ "t", BoundingBox( 96.460, 70.990,  99.250, 82.060),
	  { liberationSerif10 } },
    Text{ "e", BoundingBox( 99.250, 70.990, 103.650, 82.060),
	  { liberationSerif10 } },
    Text{ "n", BoundingBox(103.650, 70.990, 108.650, 82.060),
	  { liberationSerif10 } },
    Text{ "-", BoundingBox(108.650, 70.990, 112.040, 82.060),
	  { liberationSerif10 } },
    Text{ "p", BoundingBox(112.040, 70.990, 117.040, 82.060),
	  { liberationSerif10 } },
    Text{ "o", BoundingBox(117.040, 70.990, 122.040, 82.060),
	  { liberationSerif10 } },
    Text{ "i", BoundingBox(122.040, 70.990, 124.830, 82.060),
	  { liberationSerif10 } },
    Text{ "n", BoundingBox(124.830, 70.990, 129.730, 82.060),
	  { liberationSerif10 } },
    Text{ "t", BoundingBox(129.730, 70.990, 132.500, 82.060),
	  { liberationSerif10 } },
    Text{ "f", BoundingBox(135.020, 70.990, 138.320, 82.060),
	  { liberationSerif10 } },
    Text{ "o", BoundingBox(138.320, 70.990, 143.320, 82.060),
	  { liberationSerif10 } },
    Text{ "n", BoundingBox(143.320, 70.990, 148.320, 82.060),
	  { liberationSerif10 } },
    Text{ "t", BoundingBox(148.320, 70.990, 151.110, 82.060),
	  { liberationSerif10 } },
    Text{ ".", BoundingBox(151.110, 70.990, 153.610, 82.060),
	  { liberationSerif10 } },      
  };
  uint32_t ndx = 1;
  auto i = text.begin();
  auto j = truth.begin();

  while ((i != text.end()) && (j != truth.end())) {
    EXPECT_TRUE(textAreSame(ndx, *j, *i));
    ++i; ++j; ++ndx;
  }

  while (i != text.end()) {
    EXPECT_TRUE(false) << "Extra text at index " << ndx << ": " << *i;
    ++i; ++ndx;
  }

  while (j != truth.end()) {
    EXPECT_TRUE(false) << "Missing text at index " << ndx << ": " << *j;
    ++j; ++ndx;
  }
  
  // for (auto& t : text) {
  //   std::cout << "Text #" << ndx << " at (" << std::setprecision(6)
  // 	      << t.bounds().left() << ", " << std::setprecision(6)
  // 	      << t.bounds().top() << " to " << std::setprecision(6)
  // 	      << t.bounds().right() << ", " << std::setprecision(6)
  // 	      << t.bounds().bottom() << ")\n  [" << t.text() << "]\n";
  //   for (auto& style : t.styles()) {
  //     std::cout << "  (" << style.start() << ", " << style.end()
  // 		<< ") font=" << style.fontName() << ":" << style.fontSize()
  // 		<< ", underlined=" << (style.underlined() ? "yes" : "no")
  // 		<< ", color=" << style.color() << "\n";
  //   }
  //   std::cout << std::endl;
  //   ++ndx;
  // }
}
