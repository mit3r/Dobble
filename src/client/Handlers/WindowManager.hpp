#include <SFML/Graphics.hpp>
#include <iostream>

class WindowManager
{
private:
    const sf::Vector2u minSize{640, 480};
    const sf::Vector2u defaultSize{1280, 720};
    float scale = 1.0f;

public:
    sf::RenderWindow window;

    WindowManager();
    void handleResize(const sf::Event& event);
};
