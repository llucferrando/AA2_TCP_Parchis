#include "BoardManager.h"

BoardManager::BoardManager()
{

	_tokenMap.resize(68);
}

sf::Vector2f BoardManager::GetWorldPosition(int logicalPosition)
{
	if (logicalPosition < 0 || logicalPosition >= _square.size())
		return sf::Vector2f(-1, -1);

	return _square[logicalPosition];
}

bool BoardManager::IsBarrier(int position, int playerID)
{
	if (position < 0 || position >= _tokenMap.size())
		return false;

	int count = 0;
	for (int tokenOwner : _tokenMap[position]) {
		if (tokenOwner == playerID) count++;
	}
	return count >= 2;
}

bool BoardManager::IsHomePosition(int position)
{
	return position >= 68;
}

int BoardManager::GetStartingPosition(int playerID)
{
	switch (playerID) {
		case 0: return 0;
		case 1: return 17;
		case 2: return 34;
		case 3: return 51;
		default: return -1;
	}
}
