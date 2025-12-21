
#include <SFML/Graphics.hpp>

#include "./RoomCard.hpp"
#include "Components/HasEvents.hpp"
#include "utils/Grid.hpp"

#pragma once

class RoomCards : public sf::Drawable, public HasEvents {
  private:
  RoomCard* roomCards[12];

  public:
  RoomCards(sf::RenderWindow& window) {
    sf::Vector2f winSize = sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    Grid grid = Grid();

    const sf::Vector2f cardSize = sf::Vector2f(300.f, 120.f);
    grid.setLayoutSize(winSize);
    grid.setRows(3);
    grid.setColumns(4);
    grid.setCellSize(cardSize);
    grid.setPadding(sf::Vector2f(10.f, 10.f));
    grid.setGap(sf::Vector2f(10.f, 10.f));
    grid.calculateLayout();

    for (int i = 0; i < 12; i++) {
      roomCards[i] = new RoomCard();
      roomCards[i]->setSize(sf::Vector2f(300.f, 120.f));
      roomCards[i]->setRoomName(std::to_string(i + 1));
      roomCards[i]->setPlayersCount((i + 1) % 4, 4);
      roomCards[i]->setStatusText((i % 2 == 0) ? "Playing" : "Waiting");
      roomCards[i]->setPosition(grid.getCellPosition(i));
    }
  }

  ~RoomCards() {
    for (const auto& roomCard : roomCards) delete roomCard;
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    for (const auto& roomCard : roomCards) target.draw(*roomCard, states);
  }

  void handleEvent(const sf::Event& event) override {
    for (const auto& roomCard : roomCards) roomCard->handleEvent(event);
  }
};