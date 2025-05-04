#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Enums.hpp"

extern int GetEntryToGoalIndex(PlayerColor color);
extern std::vector<sf::Vector2f> mainPathPositions; 
extern std::map<PlayerColor, sf::Vector2f> startPositions; 
extern std::map<PlayerColor, std::vector<sf::Vector2f>> metaPositions; 
extern std::map<PlayerColor, std::vector<sf::Vector2f>> homePositions; 