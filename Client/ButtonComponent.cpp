#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(sf::Vector2f position, sf::Vector2f size, const std::string& label, EventHandler* eventHandler) : _label(FontManager::GetMainFont(), "", 24)
{
    _shape.setPosition(position);
    _shape.setSize(size);
    _shape.setFillColor(sf::Color(150, 150, 250));

    _label.setFont(FontManager::GetMainFont());
    _label.setString(label);
    _label.setCharacterSize(24);
    _label.setFillColor(sf::Color::White);
    _label.setPosition(sf::Vector2f(position.x + 10, position.y + 10));

    eventHandler->onClick.Subscribe([this](sf::Vector2f clickPos) {
        OnGlobalClick(clickPos);
        });
}

ButtonComponent::~ButtonComponent()
{
    //onClick.UnsubscribeAll();
}


void ButtonComponent::Render(sf::RenderWindow* window)
{
    window->draw(_shape);
    window->draw(_label);
}

const std::type_index ButtonComponent::GetType() { return typeid(ButtonComponent); }

void ButtonComponent::OnGlobalClick(sf::Vector2f clickPos)
{
    //If we are in the button shape we start onclick button subcribers to active
    if (_shape.getGlobalBounds().contains(clickPos)) {
        onClick.Invoke();
    }
}
