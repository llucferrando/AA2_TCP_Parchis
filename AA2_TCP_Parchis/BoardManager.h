#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

class BoardManager
{
public:
	BoardManager();

    sf::Vector2f GetWorldPosition(int logicalPosition);
    bool IsBarrier(int position, int playerID); // Checks for a barrier
    bool IsHomePosition(int position); // final positions
    int GetStartingPosition(int playerID); // starting square

private:
	std::vector<sf::Vector2f> _square;
    std::vector<std::vector<int>> _tokenMap; // to know how many tokens are per square
};

