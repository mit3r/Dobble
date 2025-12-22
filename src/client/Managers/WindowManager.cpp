#include "WindowManager.hpp"

WindowManager windowManager;

WindowManager::WindowManager() {
  const auto vm = sf::VideoMode::getDesktopMode();
  this->scale = static_cast<float>(vm.width) / static_cast<float>(defaultSize.x) * .5f;

  this->window.create(sf::VideoMode({static_cast<unsigned int>(defaultSize.x * scale), static_cast<unsigned int>(defaultSize.y * scale)}), "SFML works!");
}

WindowManager::~WindowManager() {
  if (this->window.isOpen()) {
    this->window.close();
  }
}

void WindowManager::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::Closed) {
    this->window.close();
  }

  if (event.type == sf::Event::Resized) {
    this->handleResizeEvent(event);
  }
}

void WindowManager::handleResizeEvent(const sf::Event& event) {
  sf::Vector2u newSize{
      std::max(event.size.width, this->minSize.x),
      std::max(event.size.height, this->minSize.y)};

  if (newSize.x == this->window.getSize().x && newSize.y == this->window.getSize().y) return;

  this->window.setSize(newSize);

  sf::FloatRect visibleArea(0, 0, static_cast<float>(newSize.x), static_cast<float>(newSize.y));
  this->window.setView(sf::View(visibleArea));
}
