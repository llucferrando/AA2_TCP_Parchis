#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Enums.hpp"

extern int GetEntryToGoalIndex(PlayerColor color);
extern std::vector<sf::Vector2f> mainPathPositions; // Casillas 0 a 67
extern std::map<PlayerColor, sf::Vector2f> startPositions; // Posiciones de salida
extern std::map<PlayerColor, std::vector<sf::Vector2f>> metaPositions; // 6 casillas de meta por color
extern std::map<PlayerColor, std::vector<sf::Vector2f>> homePositions; // 4 fichas en casa