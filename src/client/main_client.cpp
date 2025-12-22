#include <SFML/Graphics.hpp>
#include <iostream>

#include "Managers/LayoutManager.hpp"
#include "Managers/PagesManager.hpp"
#include "Managers/WindowManager.hpp"
#include "Objects/Cards.hpp"
#include "Page/EndPage/index.hpp"
#include "Page/GamePage/index.hpp"
#include "Page/LobbyPage/index.hpp"
#include "Page/StartPage/index.hpp"

unsigned int frames = 0;

int main(int, char**) {
  sf::RenderWindow& window = windowManager.window;

  LayoutManager::loadFonts();

  pagesManager.bindWindow(&window);
  pagesManager.add<StartPage>("start");
  pagesManager.add<LobbyPage>("lobby");
  pagesManager.add<GamePage>("game");
  pagesManager.add<EndPage>("end");

  pagesManager.changeTo("start");

  Cards cards("assets/cards.json");

  while (window.isOpen()) {
    pagesManager.render();

    sf::Event event;
    while (window.pollEvent(event)) {
      pagesManager.handleEvent(event);
      windowManager.handleEvent(event);
    }

    // Network related events
    // * To be implemented *
  }

  std::cout << "Exiting application.\n";
  LayoutManager::cleanup();

  return 0;
}
