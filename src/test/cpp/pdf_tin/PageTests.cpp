#include <pdf_tin/Document.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

using namespace pdf_tin;

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
  
  // std::cout << "label = [" << page.label() << "]" << std::endl
  // 	    << "index = " << page.index() << std::endl
  // 	    << "width = " << page.width() << std::endl
  // 	    << "height = " << page.height() << std::endl
  // 	    << "duration = " << page.duration() << std::endl;
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
    Text{ "T", BoundingBox(56.800, 57.208, 64.096, 70.492), liberationSerif12 },
    Text{ "h", BoundingBox(64.096, 57.208, 70.096, 70.492), liberationSerif12 },
    Text{ "i", BoundingBox(70.096, 57.208, 73.396, 70.492), liberationSerif12 },
    Text{ "s", BoundingBox(73.396, 57.208, 78.064, 70.492), liberationSerif12 },
    Text{ "i", BoundingBox(81.184, 57.208, 84.484, 70.492), liberationSerif12 },
    Text{ "s", BoundingBox(84.484, 57.208, 89.152, 70.492), liberationSerif12 },
    Text{ "a", BoundingBox(92.176, 57.208, 97.492, 70.492), liberationSerif12 },
    Text{ "t", BoundingBox(100.48, 57.208, 103.78, 70.492), liberationSerif12 },
    Text{ "e", BoundingBox(64.096, 57.208, 70.096, 70.492), liberationSerif12 },
    Text{ "s", BoundingBox(64.096, 57.208, 70.096, 70.492), liberationSerif12 },
    Text{ "t", BoundingBox(64.096, 57.208, 70.096, 70.492), liberationSerif12 },
    Text{ ".", BoundingBox(64.096, 57.208, 70.096, 70.492), liberationSerif12 },

  };
  uint32_t ndx = 1;

  for (auto& t : text) {
    std::cout << "Text #" << ndx << " at (" << t.bounds().left() << ", "
	      << t.bounds().top() << " to " << t.bounds().right() << ", "
	      << t.bounds().bottom() << ")\n  [" << t.text() << "]\n";
    for (auto& style : t.styles()) {
      std::cout << "  (" << style.start() << ", " << style.end()
		<< ") font=" << style.fontName() << ":" << style.fontSize()
		<< ", underlined=" << (style.underlined() ? "yes" : "no")
		<< ", color=" << style.color() << "\n";
    }
    std::cout << std::endl;
    ++ndx;
  }
}
