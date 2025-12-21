#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "Managers/LayoutManager.hpp"
#include "Managers/PagesManager.hpp"
#include "utils/Grid.hpp"
//
#include "./Components/Navigation.hpp"
#include "./Components/RoomCard.hpp"
#include "./Components/RoomCards.hpp"
#include "./Components/Title.hpp"

class StartPage : public Page {
  private:
      Title* title;
      RoomCards* roomCards;
      Navigation* navigation;

  public:
  void start(sf::RenderWindow& window);
  void end(sf::RenderWindow& window);

  void handleEvent(sf::RenderWindow& window, const sf::Event& event);
  void render(sf::RenderWindow& window);
};