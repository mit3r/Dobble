
#include <SFML/Window.hpp>

#pragma once

class HasEvents {
  public:
  virtual void handleEvent(const sf::Event& event) = 0;
};