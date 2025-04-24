#include "TokenComponent.h"
#include "BoardManager.h"
#include <vector>

class GameRules {
public:
    GameRules(BoardManager* board);

    bool CanMove(TokenComponent* ficha, int diceValue);
    int CalculateDestination(TokenComponent* ficha, int diceValue, const std::vector<TokenComponent*>& allTokens);
    bool HasEatenOpponent(int destPos, int fichaPlayerId, const std::vector<TokenComponent*>& allTokens);
    bool IsBarreraBloqueante(int from, int to, int fichaPlayerId, const std::vector<TokenComponent*>& allTokens);
    bool MustBreakBarrier(TokenComponent* ficha, const std::vector<TokenComponent*>& allTokens);

private:
    BoardManager* _board;

};