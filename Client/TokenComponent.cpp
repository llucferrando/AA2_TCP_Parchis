#include "TokenComponent.h"

TokenComponent::TokenComponent(PlayerColor color, int id)
    : _color(color), _tokenID(id), _boardPosition(-1), _state(TokenState::IN_HOME) {}

void TokenComponent::SetBoardPosition(int pos) {
    _boardPosition = pos;
}

int TokenComponent::GetBoardPosition()
{
    return _boardPosition;
}

TokenState TokenComponent::GetTokenState()
{
    return _state;
}

void TokenComponent::SetTokenState(TokenState state)
{
    _state = state;
}

int TokenComponent::GetTokenID() const
{
    return _tokenID;
}

void TokenComponent::SetTokenID(int tokenID)
{
    _tokenID = tokenID;
}


PlayerColor TokenComponent::GetColor() const {
    return _color;
}



const std::type_index TokenComponent::GetType() {
    return typeid(TokenComponent);
}
