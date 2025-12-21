#include <SFML/Graphics.hpp>
#include <iostream>

#include "Components/Title.hpp"
#include "Managers/LayoutManager.hpp"
#include "Managers/PagesManager.hpp"

class LobbyPage : public Page {
  private:
  Title* title;

  public:
  LobbyPage();

  void start(sf::RenderWindow& window);
  void end(sf::RenderWindow& window);

  void handleEvent(sf::RenderWindow& window, const sf::Event& event);
  void render(sf::RenderWindow& window);
};