#pragma once
#include <SFML/Graphics.hpp>
#include "Enums.hpp"
#include <string>

sf::Color GetColorFromIndex(int index);
PlayerColor GetEnumColorFromIndex(int index);
sf::Vector2f GetWorldPosFromBoardIndex(int index, int playerOffset = 0);