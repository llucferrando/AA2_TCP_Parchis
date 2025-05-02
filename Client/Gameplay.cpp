#include "Gameplay.h"
#include "TokenComponent.h"
#include "SpriteRenderer.h"
#include "ButtonComponent.h"
#include <iostream>

Gameplay::Gameplay(Client* client, int playerIndex, int numPlayers, EventHandler* eventHandler) : _client(client), _playerIndex(playerIndex), _currentTurnIndex(0), _diceValue(0), _isMyTurn(false), _hasRolled(false), _numPlayers(numPlayers)
{
    _timeToEndTurn = 20.f;
    _currentTime = 0.f;
    _myColor = GetEnumColorFromIndex(playerIndex);

    _board = new GameObject();

    _board->AddComponent<SpriteRenderer>("Assets/GameAssets/Parchis_2.png", sf::Color::White);

    _rollButton = new GameObject();

    _rollButton->AddComponent<ButtonComponent>(sf::Vector2f(360, 360), sf::Vector2f(150, 50), "Tirar Dado", eventHandler);

    _rollButton->GetComponent<ButtonComponent>()->onClick.Subscribe([this]() 
    {
        if (_isMyTurn && !_hasRolled) 
        {
            RollDice();
        }
    });

    // Player tokens creation

    for (int i = 0; i < 4; ++i) 
    {
        Token* token = new Token(playerIndex, i, homePositions[_myColor][i], eventHandler);

        _myTokens.push_back(token);
    }

    // Subscription to click events on tokens

    for (int i = 0; i < _myTokens.size(); ++i)
    {
        _myTokens[i]->GetComponent<ClickableComponent>()->onClick.Subscribe([this, i]() 
            {
                if (_isMyTurn && _hasRolled) {
                    MoveFichaConNormas(i);
                    EndTurn();
                }
            });
    }

    // Other Player tokens creation

    for (int i = 0; i < _numPlayers; ++i) {

        if (i == playerIndex) continue;

        PlayerColor color = GetEnumColorFromIndex(i);

        for (int j = 0; j < 4; ++j) 
        {
            Token* token = new Token(i, j, homePositions[color][j], eventHandler);
            _enemyFichas.push_back(token);
        }
    }

    _isMyTurn = (_playerIndex == 0);
}

void Gameplay::Update(float deltaTime) 
{
    if (_isMyTurn && !_hasRolled)
    {
        // IF its my turn and i didnt roll dice, and time reaches to 20s ends turn
        _currentTime += deltaTime;

        if (_currentTime >= _timeToEndTurn)
        {
            EndTurn();
        }
    }

    HandleNetwork();
}

void Gameplay::Render(sf::RenderWindow* window) 
{
    // Render board
    
    _board->GetComponent<SpriteRenderer>()->Draw(window, _board->GetComponent<Transform>());

    // Render roll button

    if (_isMyTurn && !_hasRolled && _rollButton)
    {
        _rollButton->GetComponent<ButtonComponent>()->Render(window);
    }
    
    // Render player tokens

    for (auto* token : _myTokens) 
    {
        token->Render(window);
    }

    // Render other player tokens

    for (auto* ficha : _enemyFichas) 
    {
        ficha->GetComponent<SpriteRenderer>()->Draw(window, ficha->GetComponent<Transform>());
    }
}

void Gameplay::RollDice()
{
    _diceValue = rand() % 6 + 1;
    _hasRolled = true;

    std::cout << "[Gameplay] Has sacado un " << _diceValue << std::endl;

    sf::Packet dicePacket;
    dicePacket << static_cast<int>(MessageType::DICE_RESULT) << _diceValue;
    _client->BroadcastToPeers(dicePacket);
}


void Gameplay::EndTurn() 
{
    _hasRolled = false;
    _isMyTurn = false;

    _currentTurnIndex = (_currentTurnIndex + 1) % _numPlayers;

    sf::Packet turnPacket;
    turnPacket << static_cast<int>(MessageType::TURN_CHANGE)
        << _currentTurnIndex;
    _client->BroadcastToPeers(turnPacket);
}

void Gameplay::HandleNetwork() 
{
    sf::Packet packet;

    if (_client->ReceivePacketFromPeers(packet)) 
    {
        int msgTypeInt;

        packet >> msgTypeInt;

        MessageType msgType = static_cast<MessageType>(msgTypeInt);

        switch (msgType) {

            case MessageType::MOVE_REQUEST: 
            {
                int colorInt, fichaId, pos;
                packet >> colorInt >> fichaId >> pos;
                PlayerColor color = static_cast<PlayerColor>(colorInt);

                for (auto* ficha : _enemyFichas) {

                    auto* token = ficha->GetComponent<TokenComponent>();

                    if (token->GetColor() == color && token->GetTokenID() == fichaId) 
                    {
                        token->SetBoardPosition(pos);
                        ficha->GetComponent<Transform>()->position = GetWorldPosFromBoardIndex(pos, static_cast<int>(color));
                        std::cout << "[Gameplay] Jugador " << static_cast<int>(color)
                            << " movió ficha " << fichaId << " a pos " << pos << std::endl;
                        break;
                    }
                }
                break;
            }

            case MessageType::TURN_CHANGE: 
            {
                int newTurn;
                packet >> newTurn;
                _isMyTurn = (_playerIndex == newTurn);
                std::cout << "[Gameplay] Nuevo turno: " << newTurn << (_isMyTurn ? " (mi turno)" : "") << std::endl;
                break;
            }

            case MessageType::DICE_RESULT: {
                int dice;
                packet >> dice;
                std::cout << "[Gameplay] Otro jugador ha sacado un " << dice << std::endl;
                break;
            }

            default:
                break;
        }
    }
}

bool Gameplay::HasTokenInHome() {
    for (auto* token : _myTokens) 
    {
        auto* comp = token->GetComponent<TokenComponent>();
        if (comp->GetTokenState() == TokenState::IN_HOME)
            return true;
    }
    return false;
}

bool Gameplay::AllTokensOut() 
{
    for (auto* token : _myTokens) 
    {
        auto* comp = token->GetComponent<TokenComponent>();
        if (comp->GetTokenState() == TokenState::IN_HOME)
            return false;
    }
    return true;
}

int GetStartingIndexForColor(PlayerColor color) 
{
    switch (color) 
    {
        case PlayerColor::YELLOW:
            return 4;
        case PlayerColor::BLUE: 
            return 19;
        case PlayerColor::RED: 
            return 34;
        case PlayerColor::GREEN:
            return 49;
        default: return 0;
    }
}

void Gameplay::BroadcastMove(int fichaId, int pos) 
{
    sf::Packet movePacket;
    movePacket << static_cast<int>(MessageType::MOVE_REQUEST)
        << static_cast<int>(_myColor)
        << fichaId << pos;
    _client->BroadcastToPeers(movePacket);
}

void Gameplay::MoveFichaConNormas(int fichaIndex) 
{
    Token* ficha = _myTokens[fichaIndex];
    TokenComponent* comp = ficha->GetComponent<TokenComponent>();

    int currentPos = comp->GetBoardPosition();
    int targetPos = currentPos + _diceValue;

    for (auto* rival : _enemyFichas) 
    {
        auto* rivalComp = rival->GetComponent<TokenComponent>();
        if (rivalComp->GetBoardPosition() == targetPos && rivalComp->GetTokenState() == TokenState::IN_GAME) 
        {
            rivalComp->SetTokenState(TokenState::IN_HOME);
            rivalComp->SetBoardPosition(-1);
            rival->GetComponent<Transform>()->position = homePositions[rivalComp->GetColor()][rivalComp->GetTokenID()];
            std::cout << "[Gameplay] ¡Captura! +20\n";
            targetPos += 20;
            break;
        }
    }

    int entradaMeta = GetEntryToGoalIndex(_myColor); // ej: 67 para rojo
    if (currentPos <= entradaMeta && targetPos > entradaMeta) {
        int pasosEnMeta = targetPos - entradaMeta - 1;
        if (pasosEnMeta >= 6) {
            comp->SetTokenState(TokenState::IN_END);
            std::cout << "[Gameplay] Ficha llegó a la meta.\n";
            ficha->GetComponent<Transform>()->position = metaPositions[_myColor][5];
        }
        else {
            comp->SetTokenState(TokenState::IN_END);
            ficha->GetComponent<Transform>()->position = metaPositions[_myColor][pasosEnMeta];
            comp->SetBoardPosition(100 + pasosEnMeta); // puedes usar un offset artificial como 100+
        }
    }
    else {
        comp->SetBoardPosition(targetPos);
        ficha->GetComponent<Transform>()->position = mainPathPositions[targetPos];
    }

    BroadcastMove(fichaIndex, targetPos);
}