#include "LayoutManager.hpp"

sf::Font* LayoutManager::fontRoboto = new sf::Font();

void LayoutManager::loadFonts() {
  fontRoboto->loadFromFile("assets/fonts/Roboto.ttf");
}

void LayoutManager::cleanup() {
  delete fontRoboto;
}