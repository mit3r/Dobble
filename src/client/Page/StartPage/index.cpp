#include "./index.hpp"

void StartPage::start(sf::RenderWindow& window) {
  sf::Vector2f winSize = sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));

  Grid grid = Grid();

  title = new Label();
  title->setString("Dobble");
  title->setCharacterSize(48);
  title->setFillColor(sf::Color::White);

  grid.setLayoutSize(sf::Vector2f(winSize.x, 100.f));
  grid.setRows(1);
  grid.setColumns(1);
  grid.setCellSize(title->getLocalBounds().getSize());
  grid.setPadding(sf::Vector2f(10.f, 10.f));
  grid.calculateLayout();
  title->setPosition(grid.getCellPosition(0));

  const sf::Vector2f cardSize = sf::Vector2f(300.f, 120.f);
  grid.setLayoutSize(winSize);
  grid.setRows(3);
  grid.setColumns(4);
  grid.setCellSize(cardSize);
  grid.setPadding(sf::Vector2f(10.f, 10.f));
  grid.setGap(sf::Vector2f(10.f, 10.f));
  grid.calculateLayout();

  for (int i = 0; i < 12; i++) {
    roomCards[i] = new RoomCard();
    roomCards[i]->setSize(cardSize);
    roomCards[i]->setRoomName(std::to_string(i + 1));
    roomCards[i]->setPlayersCount((i + 1) % 4, 4);
    roomCards[i]->setStatusText((i % 2 == 0) ? "Playing" : "Waiting");
    roomCards[i]->setPosition(grid.getCellPosition(i));
  }

  this->navigation = new Navigation(sf::Vector2f(0.f, 600.f), sf::Vector2f(winSize.x, 50.f));
}

void StartPage::end(sf::RenderWindow& window) {
  (void)window;
  delete title;
  delete navigation;
  for (const auto& roomCard : roomCards) delete roomCard;
}

void StartPage::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
  (void)window;
  for (const auto& roomCard : roomCards) roomCard->handleEvent(event);
  navigation->handleEvent(event);
}

void StartPage::render(sf::RenderWindow& window) {
  window.draw(*title);
  window.draw(*navigation);
  for (const auto& roomCard : roomCards) window.draw(*roomCard);
}
