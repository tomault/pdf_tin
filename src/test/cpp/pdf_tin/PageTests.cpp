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

TEST(PageTests, GetPage) {
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
