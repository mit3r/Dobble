#include "WindowManager.hpp"

WindowManager::WindowManager() {
  const auto vm = sf::VideoMode::getDesktopMode();
  this->scale = static_cast<float>(vm.width) / static_cast<float>(defaultSize.x) * .5f;

  this->window.create(sf::VideoMode({static_cast<unsigned int>(defaultSize.x * scale), static_cast<unsigned int>(defaultSize.y * scale)}), "SFML works!");
}

void WindowManager::handleResize(const sf::Event& event)
{
    sf::Vector2u newSize{
        std::max(event.size.width, this->minSize.x),
        std::max(event.size.height, this->minSize.y)
    };
    
    if (newSize.x == this->window.getSize().x && newSize.y == this->window.getSize().y) return;
    
    this->window.setSize(newSize);

    sf::FloatRect visibleArea(0, 0, static_cast<float>(newSize.x), static_cast<float>(newSize.y));
    this->window.setView(sf::View(visibleArea));
}