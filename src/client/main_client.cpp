#include <SFML/Graphics.hpp>
#include <iostream>

#include "Managers/LayoutManager.hpp"
#include "Managers/PagesManager.hpp"
#include "Managers/WindowManager.hpp"
#include "Objects/Cards.hpp"
#include "Page/EndPage/index.hpp"
#include "Page/GamePage/index.hpp"
#include "Page/JoinPage/index.hpp"
#include "Page/StartPage/index.hpp"

unsigned int frames = 0;

int main(int, char**) {
  WindowManager windowManager;
  PagesManager pagesManager;

  sf::RenderWindow& window = windowManager.window;

  LayoutManager::loadFonts();

  pagesManager.add<JoinPage>("join");
  pagesManager.add<GamePage>("game");
  pagesManager.add<EndPage>("end");
  pagesManager.add<StartPage>("start");

  pagesManager.changeTo("start", window);

  // Cards cards("assets/cards.json");

  while (window.isOpen()) {
    pagesManager.render(window);

    sf::Event event;
    while (window.pollEvent(event)) {
      pagesManager.handleEvent(window, event);
      windowManager.handleEvent(event);
    }

    // Network related events
    // * To be implemented *
  }

  std::cout << "Exiting application.\n";
  LayoutManager::cleanup();

  return 0;
}
