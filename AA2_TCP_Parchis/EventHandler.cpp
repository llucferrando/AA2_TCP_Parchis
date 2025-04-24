#include "EventHandler.h"
#include <iostream>

void EventHandler::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.is<sf::Event::Closed>())
    {
        window.close();
    }

    //Keyboard
    if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
        case sf::Keyboard::Key::Escape:
            window.close();
            break;
        default:
            break;
        }
    }

    //Mouse
    if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        switch (mousePressed->button)
        {
        case sf::Mouse::Button::Left:
            std::cout << "Left mouse button pressed at: " << mousePressed->position.x << ", " << mousePressed->position.y << std::endl;
            onClick.Invoke(sf::Vector2f(mousePressed->position.x, mousePressed->position.y));
            break;
        default:
            break;
        }
    }
}
