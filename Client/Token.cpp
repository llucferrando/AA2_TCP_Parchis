#include "Token.h"

Token::Token(int playerIndex, int tokenID, sf::Vector2f position, EventHandler* eventHandler)
{
	_color = GetColorFromIndex(playerIndex);
	_spriteRenderer = AddComponent<SpriteRenderer>("Assets/GameAssets/Token.png", _color, true);
	_token = AddComponent<TokenComponent>(GetEnumColorFromIndex(playerIndex), tokenID);
	GetComponent<Transform>()->position = position;
	_clickable = AddComponent<ClickableComponent>(eventHandler, GetComponent<Transform>(), _spriteRenderer);
}

void Token::Render(sf::RenderWindow* window)
{
	_spriteRenderer->Draw(window, GetComponent<Transform>());
}

void Token::Update(float deltaTime)
{
}

TokenState Token::GetTokenState()
{
	return _token->GetTokenState();
}

void Token::SetTokenState(TokenState tokenState)
{
	_token->SetTokenState(tokenState);
}
