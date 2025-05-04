#include "Utils.h"

sf::Color GetColorFromIndex(int index)
{
    switch (index) {
        case 0: 
            return sf::Color::Red;
        case 1: 
            return sf::Color::Blue;
        case 2: 
            return sf::Color::Green;
        case 3: 
            return sf::Color::Yellow;
        default: 
            return  sf::Color::White;
    }
}

PlayerColor GetEnumColorFromIndex(int index) 
{
    switch (index) 
    {
        case 0: 
            return PlayerColor::RED;
        case 1: 
            return PlayerColor::BLUE;
        case 2: 
            return PlayerColor::GREEN;
        case 3: 
            return PlayerColor::YELLOW;
        default:
            return PlayerColor::RED;
    }
}

sf::Vector2f GetWorldPosFromBoardIndex(int index, int playerOffset) 
{
    return sf::Vector2f(100 + index * 10, 200 + playerOffset * 50);
}