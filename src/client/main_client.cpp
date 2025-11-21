#include <SFML/Graphics.hpp>
#include <iostream>

#include "Handlers/LayoutManager.hpp"
#include "Handlers/PagesManager.hpp"
#include "Handlers/WindowManager.hpp"
#include "Objects/Cards.hpp"
#include "Page/EndPage.hpp"
#include "Page/GamePage.hpp"
#include "Page/JoinPage.hpp"

int main(int, char**) {
  WindowManager windowManager;
  PagesManager pagesManager;

  sf::RenderWindow& window = windowManager.window;

  LayoutManager::loadFonts();

  pagesManager.add<JoinPage>("join");
  pagesManager.add<GamePage>("game");
  pagesManager.add<EndPage>("end");

  pagesManager.changeTo("join", window);

  Cards cards("assets/cards.json");

  while (window.isOpen()) {
    // Drawing
    pagesManager.render(window);

    // Window related events
    sf::Event event;
    while (window.pollEvent(event)) {
      pagesManager.handleEvent(window, event);
      windowManager.handleEvent(event);
    }

    std::cout << "Main loop iteration.\n";

    // Network related events
    // * To be implemented *
  }

  std::cout << "Exiting application.\n";
  LayoutManager::cleanup();

  return 0;
}
