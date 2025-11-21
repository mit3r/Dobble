#include <SFML/Graphics.hpp>
#include <iostream>

#include "Handlers/LayoutManager.hpp"
#include "Handlers/PagesManager.hpp"

class JoinPage : public Page {
  private:
  sf::Text titleText;
  sf::RectangleShape joinButton;

  public:
  // JoinPage();

  void start(sf::RenderWindow& window);
  void end(sf::RenderWindow& window);

  void handleEvent(sf::RenderWindow& window, const sf::Event& event);
  void render(sf::RenderWindow& window);
};