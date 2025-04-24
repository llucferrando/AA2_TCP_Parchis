#include "MatchManager.h"

MatchManager::MatchManager()
{
	_currentTurn = 0;
}

void MatchManager::CreatePlayers(int num)
{
	_players.clear();

	for(int i = 0; i < num; ++i)
	{
		GameObject* player = new GameObject();
		player->AddComponent<PlayerComponent>(i, static_cast<PlayerColor>(i));
		_players.push_back(player);
	}
}

void MatchManager::CreateTokens(int tokensPerPlayer)
{
	_fichas.clear();

	for (int i = 0; i < _players.size(); ++i)
	{
		for (int j = 0; j < tokensPerPlayer; ++j)
		{
			GameObject* ficha = new GameObject();
			ficha->AddComponent<TokenComponent>(i, j);
			ficha->AddComponent<SpriteRenderer>("Assets/GameAssets/Token.png", i);
			_fichas.push_back(ficha);
		}
	}
}



GameObject* MatchManager::GetToken(int playerID, int tokenID)
{
	for (auto ficha : _fichas)
	{
		TokenComponent* token = ficha->GetComponent<TokenComponent>();
		if (token && token->GetPlayerID() == playerID && token->GetTokenID() == tokenID)
			return ficha;
	}
	return nullptr;
}

int MatchManager::GetCurrentPlayerTurn()
{
	return _currentTurn;
}

void MatchManager::SetTurn(int playerID)
{
	_currentTurn = playerID;
}

void MatchManager::NextTurn()
{
	_currentTurn = (_currentTurn + 1) % _players.size();
}

std::vector<TokenComponent*> MatchManager::GetAllTokenComponents() const
{
	std::vector<TokenComponent*> tokens;

	for (auto ficha : _fichas) {
		TokenComponent* token = ficha->GetComponent<TokenComponent>();
		if (token)
			tokens.push_back(token);
	}

	return tokens;
}

const std::vector<GameObject*>& MatchManager::GetAllFichas() const
{
	return _fichas;
}
