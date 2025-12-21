#include "PagesManager.hpp"

void PagesManager::changeTo(const std::string& name, sf::RenderWindow& window) {
  if (currentPage)
    currentPage->end(window);

  auto it = pages.find(name);
  if (it != pages.end()) {
    currentPage = it->second.get();
    currentPage->start(window);
  }
}

void PagesManager::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
  if (currentPage) currentPage->handleEvent(window, event);
}

void PagesManager::render(sf::RenderWindow& window) {
  window.clear();
  if (currentPage) currentPage->render(window);
  window.display();
}