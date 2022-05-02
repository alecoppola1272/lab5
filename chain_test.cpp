#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "chain.hpp"

#include "doctest.h"

TEST_CASE("Testing Hooke") {
  SUBCASE("Hooke's parameter k<0") { CHECK_THROWS(Hooke{-1., 1.}); }

  SUBCASE("Hooke's parameter k=0") { CHECK_THROWS(Hooke{0., 1.}); }

  SUBCASE("Hooke's parameter l<0") { CHECK_THROWS(Hooke{2., -1.}); }

  SUBCASE("Attractive Force F>0") {
    Hooke hooke{2., 10.};
    PPState p1{1., 0., 0.};
    PPState p2{1., 12., 0.};
    CHECK(hooke(p1, p2) == doctest::Approx(4.));
  }
  SUBCASE("Inverted Points (F>0)") {
    Hooke hooke{2., 10.};
    PPState p1{1., 0., 0.};
    PPState p2{1., 12., 0.};
    CHECK(hooke(p2, p1) == doctest::Approx(4.));
  }
  SUBCASE("Spring at Rest F=0") {
    Hooke hooke{2., 10.};
    PPState p1{1., 0., 0.};
    PPState p2{1., 10., 0.};
    CHECK(hooke(p1, p2) == (0.));
  }
  SUBCASE("Repulsive Force F<0") {
    Hooke hooke{2., 10.};
    PPState p1{1., 0., 0.};
    PPState p2{1., 8., 0.};
    CHECK(hooke(p1, p2) == doctest::Approx(-4.));
  }

  SUBCASE("Force on the same point") {
    Hooke hooke{2., 10.};
    PPState p1{1., 10., 0.};
    PPState p2{1., 10., 1.};
    CHECK(hooke(p1, p1) == (0.));
    CHECK(hooke(p1, p2) == (0.));
  }
}