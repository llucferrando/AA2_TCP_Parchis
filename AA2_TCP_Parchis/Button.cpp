#include "Button.h"
#include <iostream>

Button::Button(sf::Vector2f buttonPosition, sf::Vector2f buttonSize, const std::string& text, float textSize)
    : _hovered(false), _clicked(false),_label(text),_textSize(textSize)
{
    _shape.setPosition(buttonPosition);
    _shape.setSize(buttonSize);
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
    text.setCharacterSize(_textSize);
    text.setFillColor(sf::Color::White);


    sf::Vector2f center = _shape.getPosition() + _shape.getSize() / 2.f;
    text.setOrigin(text.getGlobalBounds().getCenter());  
    text.setPosition(center);

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
