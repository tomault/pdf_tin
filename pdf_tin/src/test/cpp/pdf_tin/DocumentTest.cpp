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

TEST(DocumentTests, LoadDocument) {
  Document doc = Document::fromFile(getPdfUrl("TestDoc1.pdf"));

  EXPECT_EQ("456b69217988ac9dab3415f519b3c9ba", doc.permanentId());
  EXPECT_EQ("PDF-1.4", doc.pdfVersion());
  EXPECT_EQ("", doc.title());
  EXPECT_EQ("", doc.author());
  EXPECT_EQ("", doc.subject());
  EXPECT_EQ("", doc.keywords());
  EXPECT_EQ("Writer", doc.creator());
  EXPECT_EQ("LibreOffice 5.1", doc.producer());
  EXPECT_EQ(1503322981, doc.creationDate());
  EXPECT_EQ(-1, doc.modificationDate());
  EXPECT_EQ(PageLayout::UNSET, doc.pageLayout());
  EXPECT_EQ(PageMode::UNSET, doc.pageMode());
  EXPECT_EQ("", doc.metadata());

  EXPECT_EQ(2, doc.numPages());

  // std::cout << "permanentId = [" << doc.permanentId() << "]" << std::endl
  // 	    << "updateId = [" << doc.updateId() << "]" << std::endl
  // 	    << "pdfVersion = [" << doc.pdfVersion() << "]" << std::endl
  // 	    << "title = [" << doc.title() << "]" << std::endl
  // 	    << "author = [" << doc.author() << "]" << std::endl
  // 	    << "subject = [" << doc.subject() << "]" << std::endl
  // 	    << "keywords = [" << doc.keywords() << "]" << std::endl
  // 	    << "creator = [" << doc.creator() << "]" << std::endl
  // 	    << "producer = [" << doc.producer() << "]" << std::endl
  // 	    << "creationDate = " << doc.creationDate() << std::endl
  // 	    << "modificationDate = " << doc.modificationDate() << std::endl
  // 	    << "isLinearized = " << (doc.isLinearized() ? "YES" : "NO") << std::endl
  // 	    << "pageLayout = " << doc.pageLayout() << std::endl
  // 	    << "pageMode = " << doc.pageMode() << std::endl
  // 	    << "metadata = [" << doc.metadata() << "]" << std::endl;
}
