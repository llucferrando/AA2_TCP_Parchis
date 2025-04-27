#include "EventHandler.h"
#include <iostream>

void EventHandler::HandleEvent(const sf::Event& event, Window& window)
{
    if (event.is<sf::Event::Closed>())
    {
        window.Close();
    }

    //Keyboard
    if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
        case sf::Keyboard::Key::Escape:
            window.Close();
            break;
        default:
            break;
        }
    }
    // On Text Entered invokes subscribers
    if (const sf::Event::TextEntered* textEntered = event.getIf<sf::Event::TextEntered>()) 
    {
        onTextEntered.Invoke(textEntered->unicode);
    }

    //Mouse
    if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) 
    {
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
