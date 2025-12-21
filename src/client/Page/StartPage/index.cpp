#include "./index.hpp"

void StartPage::start(sf::RenderWindow& window) {
  sf::Vector2f winSize = sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));

  this->title = new Title(window);
  this->roomCards = new RoomCards(window);
  this->navigation = new Navigation(sf::Vector2f(0.f, 600.f), sf::Vector2f(winSize.x, 50.f));
}

void StartPage::end(sf::RenderWindow& window) {
  (void)window;
  delete title;
  delete navigation;
  delete roomCards;
}

void StartPage::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
  (void)window;
  roomCards->handleEvent(event);
  navigation->handleEvent(event);
}

void StartPage::render(sf::RenderWindow& window) {
  window.draw(*title);
  window.draw(*navigation);
  window.draw(*roomCards);
}
