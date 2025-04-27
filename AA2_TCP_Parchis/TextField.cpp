#include "TextField.h"
#include <iostream>

TextField::TextField(sf::Vector2f position, sf::Vector2f size, const std::string& placeholder, bool isPassword)
    : _placeholder(placeholder), _isPassword(isPassword), _focused(false),
    _cursorBlinkTime(0.f), _cursorVisible(true)
{
    _textBox.setSize(size);
    _textBox.setPosition(position);
    _textBox.setFillColor(sf::Color(220, 220, 220));
    _textBox.setOutlineColor(sf::Color::Black);
    _textBox.setOutlineThickness(2);

    if (!_font.openFromFile("Assets/Fonts/Poppins-Bold.ttf")) {
        std::cerr << "[TextField] Failed to load font.\n";
    }
}

void TextField::HandleEvent(const sf::Event& event) {
    
}

void TextField::Update(float deltaTime) {
    if (_focused) {
        _cursorBlinkTime += deltaTime;
        if (_cursorBlinkTime > 0.5f) {
            _cursorBlinkTime = 0;
            _cursorVisible = !_cursorVisible;
        }
    }
}

void TextField::Draw(sf::RenderWindow& window) {
    window.draw(_textBox);
    DrawText(window);
}

std::string TextField::GetText() const {
    return _input;
}

void TextField::DrawText(sf::RenderWindow& window) const {
    std::string display = _input.empty() ? _placeholder : (_isPassword ? std::string(_input.size(), '*') : _input);

    sf::Text text (_font);
    text.setString(display);
    text.setCharacterSize(24);
    text.setPosition(_textBox.getPosition() + sf::Vector2f(10.f, 10.f));
    text.setFillColor(_input.empty() ? sf::Color(150, 150, 150) : sf::Color::Black);

    window.draw(text);
}