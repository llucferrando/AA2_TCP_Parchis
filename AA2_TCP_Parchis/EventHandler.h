#pragma once
#include <SFML/Window/Event.hpp>
#include "Window.h"

class EventHandler
{
public:
	void HandleEvent(const sf::Event& event, sf::RenderWindow& window);
};

