#include <pdf_tin/ImmutableList.hpp>
#include <algorithm>
#include <iterator>
#include <gtest/gtest.h>

using pdf_tin::ImmutableList;

TEST(ImmutableListTests, CreateEmpty) {
  ImmutableList<int> l;

  EXPECT_EQ(0, l.size());
  EXPECT_TRUE(l.begin() == l.end());
}

TEST(ImmutableListTests, CreateFromRange) {
  std::vector<int> DATA{1, 2, 4};
  ImmutableList<int> l(DATA.size(), DATA.begin());
  ImmutableList<int> l2(DATA.begin(), DATA.end());
  std::vector<int> tmp;

  EXPECT_EQ(DATA.size(), l.size());
  std::copy(l.begin(), l.end(), std::back_inserter(tmp));
  ASSERT_EQ(DATA.size(), tmp.size());
  EXPECT_EQ(DATA[0], tmp[0]);
  EXPECT_EQ(DATA[1], tmp[1]);
  EXPECT_EQ(DATA[2], tmp[2]);

  tmp.clear();
  EXPECT_EQ(DATA.size(), l2.size());
  std::copy(l2.begin(), l2.end(), std::back_inserter(tmp));
  ASSERT_EQ(DATA.size(), tmp.size());
  EXPECT_EQ(DATA[0], tmp[0]);
  EXPECT_EQ(DATA[1], tmp[1]);
  EXPECT_EQ(DATA[2], tmp[2]);
}

TEST(ImmutableListTests, CreateFromInitializerList) {
  ImmutableList<int> l{10, 20, 40};
  ASSERT_EQ(3, l.size());

  std::vector<int> tmp(l.begin(), l.end());
  ASSERT_EQ(3, tmp.size());
  EXPECT_EQ(10, tmp[0]);
  EXPECT_EQ(20, tmp[1]);
  EXPECT_EQ(40, tmp[2]);
}

TEST(ImmutableListTests, CopyTest) {
  ImmutableList<int> src{10, 20, 40};
  ImmutableList<int> copy(src);

  ASSERT_EQ(3, src.size());
  EXPECT_EQ(10, src[0]);
  EXPECT_EQ(20, src[1]);
  EXPECT_EQ(40, src[2]);
  
  ASSERT_EQ(3, copy.size());
  EXPECT_EQ(src[0], copy[0]);
  EXPECT_EQ(src[1], copy[1]);
  EXPECT_EQ(src[2], copy[2]);
}

TEST(ImmutableListTests, MoveTest) {
  ImmutableList<int> src{10, 20, 40};
  
  ASSERT_EQ(3, src.size());
  EXPECT_EQ(10, src[0]);
  EXPECT_EQ(20, src[1]);
  EXPECT_EQ(40, src[2]);

  ImmutableList<int> copy(std::move(src));
  EXPECT_EQ(0, src.size());
  ASSERT_EQ(3, copy.size());
  EXPECT_EQ(10, copy[0]);
  EXPECT_EQ(20, copy[1]);
  EXPECT_EQ(40, copy[2]);
}

TEST(ImmutableListTests, FindTest) {
  ImmutableList<int> l{10, 20, 40};
  ImmutableList<int>::ConstIterator p = l.find(20);
  ImmutableList<int>::ConstIterator q = l.begin();

  ++q;
  EXPECT_EQ(q, p);
  EXPECT_EQ(20, *p);

  p = l.find(0);
  EXPECT_EQ(l.end(), p);
}

