
#include <SFML/Graphics.hpp>

#include "Components/Label.hpp"
#include "utils/Grid.hpp"

#pragma once

class Title : public Label {
  private:
  Label title = Label();

  public:
  Title(sf::RenderWindow& window) {
    sf::Vector2f winSize = sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    Grid grid = Grid();

    title.setString("Dobble");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::White);

    grid.setLayoutSize(sf::Vector2f(winSize.x, 100.f));
    grid.setRows(1);
    grid.setColumns(1);
    grid.setCellSize(title.getLocalBounds().getSize());
    grid.setPadding(sf::Vector2f(10.f, 10.f));
    grid.calculateLayout();
    title.setPosition(grid.getCellPosition(0));
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(title, states);
  }
};