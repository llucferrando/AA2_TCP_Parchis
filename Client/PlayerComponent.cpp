#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(int playerID, PlayerColor playerColor) : _playerID(playerID), _playerColor(playerColor)
{
}

int PlayerComponent::GetPlayerID()
{
	return _playerID;
}

PlayerColor PlayerComponent::GetPlayerColor()
{
	return _playerColor;
}

void PlayerComponent::SetPlayerColor(PlayerColor color)
{
	if (_playerColor == color) return;
	_playerColor = color;
}

void PlayerComponent::SetPlayerID(int playerID)
{
	if (_playerID == playerID) return;
	_playerID = playerID;
}

const std::type_index PlayerComponent::GetType()
{
	return typeid(PlayerComponent);
}
