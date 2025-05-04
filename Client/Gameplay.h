#pragma once
#include "GameObject.h"
#include "Client.h"
#include "Enums.hpp"
#include <vector>
#include "Utils.h"
#include "Token.h"
#include "EventHandler.h"
#include "ButtonComponent.h"
#include "BoardPositions.h"
#include "NormalTextComponent.h"

class Gameplay : public GameObject {
public:
    Gameplay(Client* client, int playerIndex, int numPlayers, EventHandler* eventHandler);
    ~Gameplay();
    void Update(float deltaTime);
    void Render(sf::RenderWindow* window);
    Event<> onWinMatch;
    Event<> onLoseMatch;
private:

    Client* _client;
    int _playerIndex;
    int _currentTurnIndex;
    int _diceValue;
    int _numPlayers;

    bool _isMyTurn;
    bool _hasRolled;
    bool _usernamesCreated = false;

    float _timeToEndTurn;
    float _currentTime;

    GameObject* _board;
    GameObject* _rollButton;
   

    PlayerColor _myColor;

    std::array<std::string,4> _playerUsernames;
    std::array< GameObject*,4> _usernameLabels;
    std::vector<Token*> _myTokens;
    std::vector<Token*> _enemyFichas;
    std::string _username;

    void HandleNetwork();
    void BroadcastMove(int fichaId, int pos);

    // -- Gameplay

    void RollDice();

    bool AllTokensInHome();
    bool AllTokensInGoal();
    bool AllTokensOut();
    bool HasTokenInHome();

    void MoveFichaConNormas(int fichaIndex);
    void EndTurn();
};