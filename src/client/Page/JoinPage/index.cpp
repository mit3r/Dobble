#include "./index.hpp"

void JoinPage::start(sf::RenderWindow& window) {
  (void)window;
  this->titleText.setFont(*LayoutManager::fontRoboto);
  this->titleText.setString("Join Game");
  this->titleText.setCharacterSize(48);
  this->titleText.setFillColor(sf::Color::White);
  this->titleText.setPosition(100.f, 50.f);
}

void JoinPage::end(sf::RenderWindow& window) {
  (void)window;
}

void JoinPage::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
  (void)window;
  if (event.type == sf::Event::MouseButtonPressed) {
    std::cout << "JoinPage: Mouse Button Pressed at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
  }
}

void JoinPage::render(sf::RenderWindow& window) {
  window.draw(this->titleText);
}
