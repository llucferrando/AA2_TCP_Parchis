#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class TextField {
public:
    TextField(sf::Vector2f position, sf::Vector2f size, const std::string& placeholder, bool isPassword = false);

    void HandleEvent(const sf::Event& event);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

    std::string GetText() const;

private:
    sf::RectangleShape _textBox;
    sf::Font _font;

    std::string _input;
    std::string _placeholder;
    bool _isPassword;
    bool _focused;

    float _cursorBlinkTime;
    bool _cursorVisible;

    void DrawText(sf::RenderWindow& window) const;
    void UpdateText(sf::RenderWindow& window) const;
};
