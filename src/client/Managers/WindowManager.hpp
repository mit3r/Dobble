#include <SFML/Graphics.hpp>
#include <iostream>

class WindowManager
{
private:
    const sf::Vector2u minSize{640, 480};
    const sf::Vector2u defaultSize{1280, 720};
    float scale = 1.0f;

    void handleResizeEvent(const sf::Event& event);

public:
    sf::RenderWindow window;

    WindowManager();
    ~WindowManager();

    void handleEvent(const sf::Event& event);
};
