
#include <SFML/Graphics.hpp>
#include <format>
#include <string>

#include "Components/EmptyButton.hpp"
#include "Components/HasEvents.hpp"
#include "Components/Label.hpp"
#include "Managers/LayoutManager.hpp"

#pragma once

class RoomCard : public sf::RectangleShape, public HasEvents {
  private:
  Label nameText;
  Label playersCountText;
  Label statusText;

  EmptyButton playButton;
  EmptyButton spectateButton;

  public:
  RoomCard() {
    // Card appearance
    sf::RectangleShape::setFillColor(sf::Color(50, 50, 50));

    this->setRoomName("?");
    this->setPlayersCount(0, 0);
    this->setStatusText("Waiting");

    // Play button
    // playButton.loadImageFromFile("play_button.png");
    playButton.setBackgroundColor(sf::Color::Green);
    playButton.setSize(sf::Vector2f(30.f, 30.f));

    // Spectate button
    // spectateButton.loadImageFromFile("spectate_button.png");
    spectateButton.setBackgroundColor(sf::Color::Blue);
    spectateButton.setSize(sf::Vector2f(30.f, 30.f));

    // Position elements
    this->setPosition(sf::Vector2f(0.f, 0.f));
    this->setSize(sf::Vector2f(300.f, 120.f));

    // Button callbacks
    playButton.onMousePressed([this]() {
      std::cout << "Play button pressed for room: " << nameText.getString().toAnsiString() << std::endl;
    });
  }

  void setPosition(const sf::Vector2f& position) {
    sf::RectangleShape::setPosition(position);
    sf::Vector2f cardSize = sf::RectangleShape::getSize();
    sf::Vector2f buttonSize = playButton.getSize();
    const sf::Vector2f padding = sf::Vector2f(10.f, 10.f);

    nameText.setPosition(position + padding);
    playersCountText.setPosition(position + padding + sf::Vector2f(0, 30));
    statusText.setPosition(position + padding + sf::Vector2f(0, 60));

    playButton.setPosition(position + cardSize - buttonSize - padding - sf::Vector2f(40.f, 0));
    spectateButton.setPosition(position + cardSize - buttonSize - padding);
  }

  void setSize(const sf::Vector2f& cardSize) {
    sf::Vector2f position = sf::RectangleShape::getPosition();
    sf::Vector2f padding = sf::Vector2f(10.f, 10.f);
    sf::Vector2f buttonSize = playButton.getSize();

    sf::RectangleShape::setSize(cardSize);
    playButton.setPosition(position + cardSize - buttonSize - padding - sf::Vector2f(40.f, 0));
    spectateButton.setPosition(position + cardSize - buttonSize - padding);
  }

  void setRoomName(const std::string& name) {
    nameText.setString(std::format("Room {}", name));
  }

  void setPlayersCount(int current, int max) {
    playersCountText.setString(std::format("Players: {}/{}", current, max));
  }

  void setStatusText(const std::string& status) {
    statusText.setString(std::format("Status: {}", status));
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw((sf::RectangleShape) * this, states);
    target.draw(nameText, states);
    target.draw(playersCountText, states);
    target.draw(statusText, states);
    target.draw(playButton, states);
    target.draw(spectateButton, states);
  }

  void handleEvent(const sf::Event& event) override {
    playButton.handleEvent(event);
    spectateButton.handleEvent(event);
  }
};