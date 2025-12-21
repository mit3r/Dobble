#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "Managers/LayoutManager.hpp"
#include "Managers/PagesManager.hpp"
#include "utils/Grid.hpp"
//
#include "./Components/Navigation.hpp"
#include "./Components/RoomCard.hpp"

class StartPage : public Page {
  private:
  Label* title;
  RoomCard* roomCards[12];
  Navigation* navigation;

  public:
  void start(sf::RenderWindow& window);
  void end(sf::RenderWindow& window);

  void handleEvent(sf::RenderWindow& window, const sf::Event& event);
  void render(sf::RenderWindow& window);
};