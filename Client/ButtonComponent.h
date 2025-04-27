#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Event.h"
#include "EventHandler.h"

class ButtonComponent : public Component
{
public:
    ButtonComponent(sf::Vector2f position, sf::Vector2f size, const std::string& label, EventHandler* eventHandler);
    ~ButtonComponent();

    void Render(sf::RenderWindow& window);

    Event<> onClick; 

    const std::type_index GetType() override;

private:
    void OnGlobalClick(sf::Vector2f clickPos);

    sf::RectangleShape _shape;
    sf::Font _font;
    sf::Text _label;
};

