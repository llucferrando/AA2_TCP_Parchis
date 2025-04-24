#include "Button.h"
#include <iostream>

Button::Button(sf::Vector2f position, sf::Vector2f size, const std::string& text)
    : _hovered(false), _clicked(false)
{
    _shape.setPosition(position);
    _shape.setSize(size);
    _shape.setFillColor(sf::Color(100, 100, 250));
    _shape.setOutlineColor(sf::Color::Black);
    _shape.setOutlineThickness(2);

    if (!_font.openFromFile("Assets/Fonts/Poppins-Bold.ttf")) {
        std::cerr << "[Button] Error loading font" << std::endl;
    }

}

void Button::HandleEvent(const sf::Event& event)
{
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x), static_cast<float>(mousePressed->position.y));
            if (_shape.getGlobalBounds().contains(mousePos)) {
                _clicked = true;
                if (_callback) _callback();
            }
        }
    }
}

void Button::Update(float)
{
    
}


void Button::Draw(sf::RenderWindow& window) const
{
    window.draw(_shape);

    sf::Text text (_font);
    text.setString(_label);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    // Center text
    auto bounds = text.getLocalBounds();
   
    text.setPosition(_shape.getPosition() + _shape.getSize() / 2.f);

    window.draw(text);
}

void Button::SetCallback(std::function<void()> callback)
{
    _callback = std::move(callback);
}

bool Button::IsClicked() const
{
    return _clicked;
}
