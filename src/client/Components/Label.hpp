
#include <SFML/Graphics.hpp>
#include "Managers/LayoutManager.hpp"

#pragma once

class Label: public sf::Text {
 public:
  Label(
    const std::string& text = "",
    const sf::Font& font = *LayoutManager::fontRoboto, 
    unsigned int characterSize = 20
  ) {
    sf::Text::setString(text);
    sf::Text::setFont(font);
    sf::Text::setCharacterSize(characterSize);
    sf::Text::setFillColor(sf::Color::White);
  }
};