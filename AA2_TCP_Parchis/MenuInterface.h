#pragma once
#include <SFML/Graphics.hpp>
class MenuInterface
{
public:
	virtual void Init() = 0;
	virtual void HandleEvent(const sf::Event& event) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow* window) = 0;
	virtual ~MenuInterface() = default;
};

