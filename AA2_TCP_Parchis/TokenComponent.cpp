#include "TokenComponent.h"

TokenComponent::TokenComponent(int playerID, int tokenID) : _playerID(playerID), _tokenID(tokenID)
{
	_boardPosition = -1;
	_state = FichaState::IN_HOME;
}

int TokenComponent::GetPlayerID()
{
	return _playerID;
}

FichaState TokenComponent::GetTokenState()
{
	return _state;
}

int TokenComponent::GetTokenID()
{
	return _tokenID;
}

int TokenComponent::GetBoardPosition()
{
	return _boardPosition;
}

void TokenComponent::SetPlayerID(int playerID)
{
	if (_playerID == playerID) return;

	_playerID = playerID;
}

void TokenComponent::SetTokenID(int tokenID)
{
	if (_tokenID == tokenID) return;

	_tokenID = tokenID;
}

void TokenComponent::SetBoardPosition(int position)
{
	if (_boardPosition == position) return;

	_boardPosition = position;
}

void TokenComponent::SetTokenState(FichaState state)
{
	if (_state == state) return;

	_state = state;
}
