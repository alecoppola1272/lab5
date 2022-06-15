#include"chain.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

auto evolve(Chain& chain, int steps_per_evolution, sf::Time delta_t) 
{
  double const dt{delta_t.asSeconds()};

  for (int i{0}; i != steps_per_evolution; ++i) {
    chain.evolve(dt);
  }

  return chain.state();
}

int main()
{
  // Molla con:
  // k = 1 N/m
  // l = 1 cm
  Hooke hooke{1.0, 0.01};

  // Catena con 10 punti materiali:
  // m: 50 g per ogni punto
  // x: un punto ogni 1.1 cm (partendo da 0.0)
  // v: tutti i punti hanno v = 0 m/s
  Chain chain(hooke);

  double const p_m{0.05};

  double p_x{0.0};
  double delta_x{0.011};

  for (auto const p_v : {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}) {
    chain.push_back({p_m, p_x, p_v});
    p_x += delta_x;
  };

  auto const delta_t{sf::milliseconds(1)};
  int const fps = 30;
  int const steps_per_evolution{1000 / fps};

  unsigned const display_width = .9 * sf::VideoMode::getDesktopMode().width;
  unsigned const display_height{300};

  auto const min_x{-0.025};
  auto const max_x{0.125};
  auto const scale_x = display_width / (max_x - min_x);

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Hooke Chain Evolution"); //titolo
  window.setFramerateLimit(fps); //frame rate limit

  sf::CircleShape circ{18.0f}; //scelta oggetto e dimensione
  circ.setFillColor(sf::Color::Blue); //colore oggetto

 //game loop, dipende dagli fps
  while (window.isOpen()) {  //iterazione, processa gli eventi
    sf::Event event;
    while (window.pollEvent(event)) {            //ricerca un evento
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::White); //pulizia scena precedente

    auto const state = evolve(chain, steps_per_evolution, delta_t);  //nuovo stato dopo l'evoluzione

    for (auto& pps : state) {     // per ogni punto e per ogni posizione disegna il punto
      circ.setPosition((pps.x - min_x) * scale_x, display_height * 0.5); //il punto è in posizione pps.x-min_x
      window.draw(circ); //disegna
    }

    window.display(); //visulizzazione
  }
}