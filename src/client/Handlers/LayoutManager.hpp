
#include <SFML/Graphics.hpp>

#ifndef LAYOUT_MANAGER_HPP
#define LAYOUT_MANAGER_HPP

class LayoutManager {
  public:
  static void loadFonts();
  static void cleanup();

  static sf::Font* fontRoboto;
};

#endif  // LAYOUT_MANAGER_HPP