#include <pdf_tin/NoSuchPageError.hpp>
#include <gtest/gtest.h>

using pdf_tin::NoSuchPageError;

TEST(NoSuchPageErrorTests, CreateFromIndex) {
  NoSuchPageError ex(3);

  EXPECT_EQ(ex.page(), "3");
  EXPECT_STREQ(ex.what(), "No page at index 3");
}

TEST(NoSuchPageErrorTests, CreateFromLabel) {
  NoSuchPageError ex("cow");

  EXPECT_EQ(ex.page(), "cow");
  EXPECT_STREQ(ex.what(), "No page with label \"cow\"");
}
