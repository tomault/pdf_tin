#include <pdf_tin/BoundingBox.hpp>
#include <gtest/gtest.h>
#include <poppler/glib/poppler.h>

using namespace pdf_tin;

TEST(BoundingBoxTests, CreateFromBoundaries) {
  BoundingBox box(10.0, 20.0, 40.0, 70.0);

  EXPECT_EQ(10.0, box.left());
  EXPECT_EQ(20.0, box.top());
  EXPECT_EQ(40.0, box.right());
  EXPECT_EQ(70.0, box.bottom());

  EXPECT_EQ(30.0, box.width());
  EXPECT_EQ(50.0, box.height());
}

TEST(BoundingBoxTests, CreateFromPopplerRectangle) {
  PopplerRectangle rect;
  rect.x1 = 10.0;
  rect.y1 = 20.0;
  rect.x2 = 40.0;
  rect.y2 = 70.0;

  BoundingBox box(rect);
  EXPECT_EQ(10.0, box.left());
  EXPECT_EQ(20.0, box.top());
  EXPECT_EQ(40.0, box.right());
  EXPECT_EQ(70.0, box.bottom());
}

TEST(BoundingBoxTests, ToRect) {
  BoundingBox box(10.0, 20.0, 40.0, 70.0);
  PopplerRectangle rect;

  box.toRect(&rect);
  EXPECT_EQ(rect.x1, 10.0);
  EXPECT_EQ(rect.y1, 20.0);
  EXPECT_EQ(rect.x2, 40.0);
  EXPECT_EQ(rect.y2, 70.0);
}

