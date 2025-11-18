#include <iostream>
#include <SFML/Graphics.hpp>
#include "Handlers/WindowManager.hpp"

int main(int, char**){
    
    
    WindowManager windowManager;
    sf::RenderWindow& window = windowManager.window;

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::Resized)
                windowManager.handleResize(event);
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}

