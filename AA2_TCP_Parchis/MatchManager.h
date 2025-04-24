#pragma once
#include "GameObject.h"
#include "Enums.hpp"
#include "PlayerComponent.h"
#include "TokenComponent.h"
#include "SpriteRenderer.h"
#include <vector>

class MatchManager
{
public:
	MatchManager();
	void CreatePlayers(int num);
	void CreateTokens(int tokensPerPlayer);


	GameObject* GetToken(int playerID, int tokenID);
	int GetCurrentPlayerTurn();
	void SetTurn(int playerID);
	void NextTurn();
	std::vector<TokenComponent*> GetAllTokenComponents() const;
	const std::vector<GameObject*>& GetAllFichas() const;


private:
	sf::Color GetColorFromPlayer(int playerIndex);
	std::vector<GameObject*> _players;
	std::vector<GameObject*> _fichas;
	int _currentTurn;
};

