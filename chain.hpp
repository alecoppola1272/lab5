#ifndef CHAIN_HPP
#define CHAIN_HPP
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

struct PPState {
  double m{};
  double x{};
  double v{};
};

// Opzionale
std::ostream& operator<<(std::ostream& os, PPState const& p) {
  os << "pps ( m: " << p.m << ", x: " << p.x << ", v: " << p.v << ')';
  return os;
}
//
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
    return (abs(p2.x - p1.x) == 0) ? 0 : (abs(p2.x - p1.x) - m_l) * m_k;
  }
};

class Chain {
  Hooke m_inter;
  std::vector<PPState> m_ppses;
  PPState solve(PPState const& pps, double f, double delta_t) const {
    auto const a = f / pps.m;
    auto const v = pps.v + a * delta_t;
    auto const x = pps.x + (pps.v * delta_t + 0.5 * a * delta_t) * delta_t;
    return PPState{pps.m, x, v};
  }

 public:
  Chain(Hooke const& inter) : m_inter{inter} {};

  bool empty() { return m_ppses.empty(); }

  int size() const { return m_ppses.size(); }

  void push_back(PPState const& pps) {
    // From solution
    if (!empty() && pps.x <= m_ppses.back().x) {
      throw std::runtime_error{
          "Added PPState x must be larger than x of rightmost chain's PPState"};
    }
    //
    m_ppses.push_back(pps);
  }

  std::vector<PPState> const& state() const { return m_ppses; }

  void evolve(double const delta_t) {
    auto pps_it = m_ppses.begin();
    auto pps_it_next = std::next(pps_it);
    auto pps_last = std::prev(m_ppses.end());

    double f_prev{0.0};

    for (; pps_it != pps_last; ++pps_it, ++pps_it_next) {
      double f = m_inter(*pps_it, *pps_it_next);
      *pps_it = solve(*pps_it, f - f_prev, delta_t);
      f_prev = f;
    }
    *pps_last = solve(*pps_last, -f_prev, delta_t);
  }
};

#endif
