#pragma once
#include "Enums.hpp"
#include "Component.h"

class PlayerComponent : public Component
{
public:
	PlayerComponent(int playerID, PlayerColor playerColor);

	int GetPlayerID();
	PlayerColor GetPlayerColor();


	void SetPlayerColor(PlayerColor color);
	void SetPlayerID(int playerID);
	const std::type_index GetType() override;

private:
	int _playerID;
	PlayerColor _playerColor;
	
};


