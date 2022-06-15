#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "chain.hpp"

#include "doctest.h"

// Semplifica la comparazione tra PPStates
// facendo uso di doctest::Approx
// From Solution
bool approx_eq(PPState const& l, PPState const& r) {
  return l.m == doctest::Approx(r.m) && l.x == doctest::Approx(r.x) &&
         l.v == doctest::Approx(r.v);
}

TEST_CASE("Testing Hooke") {
  SUBCASE("Hooke's parameter k<0") { CHECK_THROWS(Hooke{-1., 1.}); }

  SUBCASE("Hooke's parameter k=0") { CHECK_THROWS(Hooke{0., 1.}); }

  SUBCASE("Hooke's parameter l<0") { CHECK_THROWS(Hooke{2., -1.}); }

  Hooke const hooke{2.0, 10.0};

  SUBCASE("Attractive Force F>0") {
    PPState p1{1., 0., 0.};
    PPState p2{1., 12., 0.};
    double f = hooke(p1, p2);
    CHECK(f == doctest::Approx(4.));
  }
  SUBCASE("Inverted Points (F>0)") {
    PPState p1{1., 0., 0.};
    PPState p2{1., 12., 0.};
    CHECK(hooke(p2, p1) == doctest::Approx(4.));
  }
  SUBCASE("Spring at Rest F=0") {
    PPState p1{1., 0., 0.};
    PPState p2{1., 10., 0.};
    CHECK(hooke(p1, p2) == (0.));
  }
  SUBCASE("Repulsive Force F<0") {
    PPState p1{1., 0., 0.};
    PPState p2{1., 8., 0.};
    CHECK(hooke(p1, p2) == doctest::Approx(-4.));
  }

  SUBCASE("Force on the same point or points with same position") {
    PPState p1{1., 10., 0.};
    PPState p2{1., 10., 1.};
    CHECK(hooke(p1, p1) == (0.));
    CHECK(hooke(p1, p2) == (0.));
  }
}

TEST_CASE("Testing Chain") {
  SUBCASE("Basic Test with two points") {
    Chain c{Hooke{2., 10.}};
    c.push_back({1., 0., 0.});
    c.push_back({2., 12., 0.});
    const auto state_i = c.state();
    c.evolve(1.0);
    const auto state_f = c.state();
  }
  Chain c{Hooke{0.1, 2.0  }};
  REQUIRE(c.empty());  //? vector m_ppses empty?
  SUBCASE("Out-of-Order PPState Throw") {
    c.push_back({0.5, 1.0, 0.0});
    CHECK_THROWS(
        c.push_back({0.5, 0.0, 0.0}));  //?First point on posistion 0.0?
  }

  SUBCASE("PPState with same x Throw") {
    c.push_back({0.5, 1.0, 0.0});
    CHECK_THROWS(c.push_back({0.5, 1.0, 0.0}));  //?Next point with same x?
  }

  SUBCASE("f=0, 1 step") {
    c.push_back({0.5, 0.0, 0.0});
    c.push_back({0.5, 2.0, 0.0});

    c.evolve(1.0);
    const auto state = c.state();

    CHECK(approx_eq(state[0], {0.5, 0.0, 0.0}));
    CHECK(approx_eq(state[1], {0.5, 2.0, 0.0}));
  }
  // Not seen
  SUBCASE("f != 0, 1 step") {
    c.push_back({0.5, 0.0, 0.0});
    c.push_back({0.5, 4.0, 0.0});

    c.evolve(1.0);
    const auto state = c.state();

    CHECK(approx_eq(state[0], {0.5, 0.2, 0.4}));
    CHECK(approx_eq(state[1], {0.5, 3.8, -0.4}));
  }
  SUBCASE("f != 0, 2 steps forward") {
    c.push_back({1.0, 0.0, 0.2});
    c.push_back({1.0, 2.0, 0.0});
    c.push_back({1.0, 4.0, 0.0});

    c.evolve(1.0);
    c.evolve(1.0);
    const auto state = c.state();

    CHECK(approx_eq(state[0], {1.0, 0.39, 0.18}));
    CHECK(approx_eq(state[1], {1.0, 2.01, 0.02}));
    CHECK(approx_eq(state[2], {1.0, 4.0, 0.0}));
  }
  SUBCASE("f != 0, 2 steps backward") {
    c.push_back({1.0, 0.0, 0.0});
    c.push_back({1.0, 2.0, 0.0});
    c.push_back({1.0, 4.0, -0.2});

    c.evolve(1.0);
    c.evolve(1.0);
    const auto state = c.state();

    CHECK(approx_eq(state[0], {1.0, 0.0, 0.0}));
    CHECK(approx_eq(state[1], {1.0, 1.99, -0.02}));
    CHECK(approx_eq(state[2], {1.0, 3.61, -0.18}));
  }
}
