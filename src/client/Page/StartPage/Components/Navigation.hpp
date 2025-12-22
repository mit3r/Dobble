
#include <SFML/Graphics.hpp>
#include <format>
#include <iostream>
#include <string>

#include "Components/HasEvents.hpp"
#include "Components/LabelButton.hpp"
#include "Managers/LayoutManager.hpp"
#include "Managers/PagesManager.hpp"
#include "utils/Grid.hpp"

class Navigation : public sf::RectangleShape, public HasEvents {
  private:
  Grid grid = Grid();
  LabelButton prevButton = LabelButton();
  LabelButton nextButton = LabelButton();

  public:
  Navigation(
      sf::Vector2f position,
      sf::Vector2f size) {
    const sf::Vector2f buttonSize = sf::Vector2f(150.f, size.y);

    sf::RectangleShape::setPosition(position);
    sf::RectangleShape::setSize(size);
    sf::RectangleShape::setFillColor(sf::Color(0, 0, 0, 150));

    grid.setLayoutSize(size);
    grid.setCellSize(buttonSize);
    grid.setColumns(2);
    grid.setGap(sf::Vector2f(10.f, 0.f));
    grid.calculateLayout();

    prevButton.label.setString("Previous");
    prevButton.label.setFillColor(sf::Color::White);
    prevButton.setFillColor(sf::Color::Transparent);
    prevButton.setOutlineColor(sf::Color::White);
    prevButton.setOutlineThickness(2.f);
    prevButton.setSize(buttonSize);
    nextButton.setPosition(position + grid.getCellPosition(1));

    nextButton.label.setString("Next");
    nextButton.label.setFillColor(sf::Color::White);
    nextButton.setFillColor(sf::Color::Transparent);
    nextButton.setOutlineColor(sf::Color::White);
    nextButton.setOutlineThickness(2.f);
    nextButton.setSize(buttonSize);
    prevButton.setPosition(position + grid.getCellPosition(0));

    // Button callbacks
    prevButton.onMousePressed([this]() {
      std::cout << "Previous button pressed" << std::endl;
    });
    nextButton.onMousePressed([this]() {
      std::cout << "Next button pressed" << std::endl;
      pagesManager.changeTo("lobby");
    });
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw((sf::RectangleShape) * this, states);
    target.draw(this->prevButton, states);
    target.draw(this->nextButton, states);
  }

  void handleEvent(const sf::Event& event) override {
    prevButton.handleEvent(event);
    nextButton.handleEvent(event);
  }
};