#include <SFML/Graphics.hpp>
#include <functional>

#include "Components/HasEvents.hpp"

#pragma once

class EmptyButton : public sf::RectangleShape, public HasEvents {
  private:
  sf::IntRect rect;

  std::function<void()> mousePressedCallback;
  std::function<void()> mouseReleasedCallback;

  bool isHovered = false;
  std::function<void()> hoverStartCallback;
  std::function<void()> hoverEndCallback;

  public:
  EmptyButton() {
    this->setSize(sf::Vector2f(30.f, 30.f));
    this->setPosition(sf::Vector2f(0.f, 0.f));
    this->setBackgroundColor(sf::Color::White);
  }

  void setPosition(const sf::Vector2f& position) {
    this->rect.left = static_cast<int>(position.x);
    this->rect.top = static_cast<int>(position.y);
    RectangleShape::setPosition(position);
  }

  void setSize(const sf::Vector2f& size) {
    this->rect.width = static_cast<int>(size.x);
    this->rect.height = static_cast<int>(size.y);
    RectangleShape::setSize(size);
  }

  void setBackgroundColor(const sf::Color color) {
    RectangleShape::setFillColor(color);
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw((sf::RectangleShape) * this, states);
  }

  void onMousePressed(std::function<void()> callback) {
    this->mousePressedCallback = callback;
  }

  void onMouseReleased(std::function<void()> callback) {
    this->mouseReleasedCallback = callback;
  }

  void onMouseHoverStart(std::function<void()> callback) {
    this->hoverStartCallback = callback;
  }

  void onMouseHoverEnd(std::function<void()> callback) {
    this->hoverEndCallback = callback;
  }

  void handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && this->rect.contains(event.mouseButton.x, event.mouseButton.y) && this->mousePressedCallback) this->mousePressedCallback();

    if (event.type == sf::Event::MouseButtonReleased && this->rect.contains(event.mouseButton.x, event.mouseButton.y) && this->mouseReleasedCallback) {
      this->mouseReleasedCallback();
    }

    if (event.type == sf::Event::MouseMoved && this->rect.contains(event.mouseMove.x, event.mouseMove.y) && !this->isHovered && this->hoverStartCallback) {
      this->isHovered = true;
      if (this->hoverStartCallback) this->hoverStartCallback();
    }

    if (event.type == sf::Event::MouseMoved && !this->rect.contains(event.mouseMove.x, event.mouseMove.y) && this->isHovered && this->hoverEndCallback) {
      if (this->hoverEndCallback) this->hoverEndCallback();
      this->isHovered = false;
    }
  };
};