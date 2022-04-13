#ifndef CHAIN_HPP
#define CHAIN_HPP
#include <cmath>
#include <vector>

struct PPState {
  double m{};
  double x{};
  double v{};
};

class Hooke {
  double m_k;
  double l_k;

 public:
  Hooke(double k, double l) : m_k{k}, l_k{l} {};
  double operator()(PPState const& p1, PPState const& p2) const {
    double f;
    if (p2.x - p1.x == 0) {
      return f = 0;
    }
    f = (abs(p2.x - p1.x) - l_k) * m_k;
    return f;
  }
};

//From Now Not Tested
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