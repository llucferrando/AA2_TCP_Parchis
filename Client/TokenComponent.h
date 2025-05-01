#pragma once
#include "Component.h"
#include "Enums.hpp"

class TokenComponent : public Component
{
public:
	TokenComponent(PlayerColor color, int id);

	void SetBoardPosition(int position);
	int GetBoardPosition();


	TokenState GetTokenState();
	void SetTokenState(TokenState state);
	
	int GetTokenID() const;	
	void SetTokenID(int tokenID);

	PlayerColor GetColor() const;

	const std::type_index GetType() override;

private:
	PlayerColor _color;
	int _tokenID;
	int _boardPosition; // -1 if it's home

	TokenState _state;
};

