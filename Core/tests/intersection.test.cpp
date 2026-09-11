#include "Physics/aabb.h"
#include "Physics/boundingSphere.h"
#include "Physics/plane.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(SPHERESPHERE, INTERSECTING) {
  auto bbs = Physics::BoundingSphere({0, 0, 0}, 2);
  auto other = Physics::BoundingSphere({0, 1, 0}, 2);
  auto result = bbs.IntersectBoundingSphere(other);
  std::cerr << result.GetDistance() << "\n";
  EXPECT_TRUE(result.GetDoesIntersect());
  EXPECT_EQ(result.GetDistance(), -3.0f);
}

TEST(SPHERESPHERE, NONINTERSECTING) {
  auto bbs = Physics::BoundingSphere({0, 0, 0}, 2);
  auto other = Physics::BoundingSphere({0, 5, 0}, 2);
  auto result = bbs.IntersectBoundingSphere(other);
  EXPECT_FALSE(result.GetDoesIntersect());
  EXPECT_EQ(result.GetDistance(), 1.0f);
};

TEST(AABBAABB, INTERSECTING) {
  auto aabb = Physics::Aabb({-1, -2, -3}, {1, 2, 3});
  auto other = Physics::Aabb({0, 1, 2}, {3, 4, 5});
  auto result = aabb.IntersectAABB(other);
  EXPECT_TRUE(result.GetDoesIntersect());
  EXPECT_EQ(result.GetDistance(), -1);
}

TEST(AABBAABB, NONINTERSECTING) {
  auto aabb = Physics::Aabb({-1, -2, -3}, {1, 2, 3});
  auto other = Physics::Aabb({4, 5, 6}, {10, 11, 12});
  auto result = aabb.IntersectAABB(other);
  EXPECT_FALSE(result.GetDoesIntersect());
  EXPECT_EQ(result.GetDistance(), 3);
}

TEST(PLANESPHERE, NONINTERSECTING) {
  auto plane = Physics::Plane({0, 0, 1}, 2);
  auto sphere = Physics::BoundingSphere({10, 10, 5}, 2);
  auto result = plane.IntersectBoundingSphere(sphere);
  EXPECT_FALSE(result.GetDoesIntersect());
  EXPECT_EQ(result.GetDistance(), 1);
}

TEST(PLANESPHERE, INTERSECTING) {
  auto plane = Physics::Plane({0, 0, 1}, 2);
  auto sphere = Physics::BoundingSphere({10, 10, 3}, 2);
  auto result = plane.IntersectBoundingSphere(sphere);
  EXPECT_TRUE(result.GetDoesIntersect());
  EXPECT_EQ(result.GetDistance(), -1);
}
