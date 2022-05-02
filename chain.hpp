#ifndef CHAIN_HPP
#define CHAIN_HPP
#include <cmath>
#include <stdexcept>
#include <vector>

struct PPState {
  double m{};
  double x{};
  double v{};
};

class Hooke {
  double m_k;
  double m_l;

 public:
  Hooke(double k, double l) : m_k{k}, m_l{l} {
    if (m_k <= 0.) {
      throw std::runtime_error{"Hookes's parameter k must be > 0"};
    }

    if (m_l < 0) {
      throw std::runtime_error{"Hooke's parameter l must be =>0 "};
    }
  };

  double operator()(PPState const& p1, PPState const& p2) const {
    double F;
    if (p2.x - p1.x == 0) {
      return F = 0;
    }
    F = (abs(p2.x - p1.x) - m_l) * m_k;
    return F;
  }
};

// From Now Not Tested
class Chain {
  Hooke m_inter;
  std::vector<PPState> m_ppses;

 public:
  Chain(Hooke const& inter);
  void push_back(PPState const& pps);
  int size() const;

  void evolve(double delta_t);
  std::vector<PPState> const& state() const;
};

#endif