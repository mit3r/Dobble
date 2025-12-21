#include "PagesManager.hpp"

PagesManager pagesManager;

void PagesManager::changeTo(const std::string& name) {
  if (!window) throw std::runtime_error("PagesManager: No window bound. Call bindWindow() before changing pages.");

  if (currentPage)
    currentPage->end(*window);

  auto it = pages.find(name);
  if (it != pages.end()) {
    currentPage = it->second.get();
    currentPage->start(*window);
  }
}

void PagesManager::handleEvent(const sf::Event& event) {
  if (currentPage) currentPage->handleEvent(*window, event);
}

void PagesManager::render() {
  window->clear();
  if (currentPage) currentPage->render(*window);
  window->display();
}