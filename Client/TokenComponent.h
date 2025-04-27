#pragma once
#include "Component.h"
#include "Enums.hpp"

class TokenComponent : public Component
{
public:
	TokenComponent(int player, int id);

	int GetPlayerID();
	TokenState GetTokenState();
	int GetTokenID();
	int GetBoardPosition();
	
	void SetPlayerID(int playerID);
	void SetTokenID(int tokenID);
	void SetBoardPosition(int position);
	void SetTokenState(TokenState state);
	const std::type_index GetType() override;

private:
	int _playerID;
	int _tokenID;
	int _boardPosition; // -1 if it's home
	TokenState _state;
};

