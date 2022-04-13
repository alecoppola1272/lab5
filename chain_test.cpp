#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "chain.hpp"

#include "doctest.h"

TEST_CASE("Testing Hooke") {
  Hooke hooke{2., 10.};

  SUBCASE("Attractive Force") {
    PPState p1{1., 0., 0.};
    PPState p2{1., 12., 0.};
    double f = hooke(p1, p2);
    CHECK(f == doctest::Approx(4.));
  }
  SUBCASE("Inverted Points") {
    PPState p1{1., 0., 0.};
    PPState p2{1., 12., 0.};
    double f = hooke(p2, p1);
    CHECK(f == doctest::Approx(4.));
  }
  SUBCASE("Spring at Rest") {
    PPState p1{1., 0., 0.};
    PPState p2{1., 10., 0.};
    double f = hooke(p1, p2);
    CHECK(f == (0.));
  }
  SUBCASE("Repulsive Force") {
    PPState p1{1., 0., 0.};
    PPState p2{1., 8., 0.};
    double f = hooke(p1, p2);
    CHECK(f == doctest::Approx(-4.));
  }

  SUBCASE("Force on the same point") {
    PPState p1{1., 10., 0.};
    double f = hooke(p1, p1);
    CHECK(f == (0.));
  }
}