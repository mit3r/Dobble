#include "./index.hpp"

LobbyPage::LobbyPage() {
}

void LobbyPage::start(sf::RenderWindow& window) {
  this->title = new Title(window);
}

void LobbyPage::end(sf::RenderWindow& window) {
  (void)window;
}

void LobbyPage::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
  (void)window;
  (void)event;
}

void LobbyPage::render(sf::RenderWindow& window) {
  window.draw(*this->title);
}
