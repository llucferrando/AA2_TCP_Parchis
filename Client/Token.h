#pragma once
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "TokenComponent.h"
#include "ClickableComponent.h"
#include "Enums.hpp"
#include "Utils.h"

class Token : public GameObject
{
public:
	Token(int playerIndex, int tokenID, sf::Vector2f position, EventHandler* eventHandler);

	void Render(sf::RenderWindow* window);
	void Update(float deltaTime);
	TokenState GetTokenState();
	void SetTokenState(TokenState tokenState);
private:
	SpriteRenderer* _spriteRenderer;
	TokenComponent* _token;
	Transform* _transform;
	ClickableComponent* _clickable;
	sf::Color _color;
	TokenState _state;
};

