#pragma once
#include <SFML/Window/Event.hpp>
#include "Window.h"
#include "Event.h"

class EventHandler
{
public:
	void HandleEvent(const sf::Event& event, sf::RenderWindow& window);

	Event<sf::Vector2f> onClick;
};

