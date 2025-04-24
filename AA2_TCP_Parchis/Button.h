#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
public:
    Button(sf::Vector2f position, sf::Vector2f size, const std::string& label);

    void HandleEvent(const sf::Event& event);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window) const;

    void SetCallback(std::function<void()> callback);
    bool IsClicked() const;

private:
    sf::RectangleShape _shape;
    sf::Font _font;
    std::string _label;

    bool _hovered;
    bool _clicked;

    std::function<void()> _callback;
};
