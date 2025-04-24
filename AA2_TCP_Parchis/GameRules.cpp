#include "GameRules.h"

GameRules::GameRules(BoardManager* board) : _board(board) {}

bool GameRules::CanMove(TokenComponent* ficha, int diceValue)
{
    if (ficha->GetTokenState() == TokenState::IN_HOME)
        return diceValue == 5;

    if (ficha->GetTokenState() == TokenState::IN_GAME)
        return true;

    return false;
}

int GameRules::CalculateDestination(TokenComponent* ficha, int diceValue, const std::vector<TokenComponent*>& allTokens)
{
    int start = ficha->GetBoardPosition();
    int maxMove = diceValue;

    for (int i = 1; i <= maxMove; ++i)
    {
        int next = start + i;
        if (_board->IsBarrier(next, ficha->GetPlayerID()))
        {
            return start + i - 1; // it stops before the barrier
        }
    }

    return start + maxMove;
}

bool GameRules::HasEatenOpponent(int destPos, int fichaPlayerId, const std::vector<TokenComponent*>& allTokens)
{
    for (auto token : allTokens)
    {
        if (token->GetPlayerID() != fichaPlayerId &&
            token->GetBoardPosition() == destPos &&
            token->GetTokenState() == TokenState::IN_GAME)
        {
            return true;
        }
    }
    return false;
}

bool GameRules::IsBarreraBloqueante(int from, int to, int fichaPlayerId, const std::vector<TokenComponent*>& allTokens)
{
    for (int i = from + 1; i <= to; ++i)
    {
        if (_board->IsBarrier(i, fichaPlayerId))
            return true;
    }
    return false;
}

bool GameRules::MustBreakBarrier(TokenComponent* ficha, const std::vector<TokenComponent*>& allTokens)
{
    // Si hay barrera propia en casilla actual
    int pos = ficha->GetBoardPosition();
    int sameCount = 0;

    for (auto token : allTokens)
    {
        if (token->GetPlayerID() == ficha->GetPlayerID() && token->GetBoardPosition() == pos)
        {
            sameCount++;
        }
    }

    return sameCount >= 2;
}
