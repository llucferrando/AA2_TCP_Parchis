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

    _board->AddComponent<SpriteRenderer>("Assets/GameAssets/Parchis_2.png", sf::Color::White, false);

    _rollButton = new GameObject();

    _rollButton->AddComponent<ButtonComponent>(sf::Vector2f(360, 360), sf::Vector2f(150, 50), "Tirar Dado", eventHandler);

    auto* btn = _rollButton->GetComponent<ButtonComponent>();

    if (btn)
    {
        btn->onClick.Subscribe([this]()
            {
                if (_isMyTurn && !_hasRolled)
                {
                    RollDice();
                }
            });
    }
    else
    {
        std::cerr << "[Error] ButtonComponent no encontrado en _rollButton" << std::endl;
    }

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
                if (_isMyTurn && _hasRolled) { MoveFichaConNormas(i); }
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

Gameplay::~Gameplay()
{
    delete _board;
    delete _rollButton;

    for (auto* t : _myTokens) 
        delete t;
    _myTokens.clear();

    for (auto* t : _enemyFichas) 
        delete t;
    _enemyFichas.clear();
    
    _client->ClearPeers();
}

void Gameplay::Update(float deltaTime) 
{
    _client->UpdateP2PConnections();

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
    //_diceValue = rand() % 6 + 1;
    _diceValue = 5;
    _hasRolled = true;

    // -- Si todos los tokens estan out sacas 7

    if (AllTokensOut() && _diceValue == 6)
    {
        _diceValue = 7;
    }

    std::cout << "[Gameplay] Has sacado un " << _diceValue << std::endl;

    sf::Packet dicePacket;
    dicePacket << static_cast<int>(MessageType::DICE_RESULT) << _diceValue;

    _client->BroadcastToPeers(dicePacket);

    if (_diceValue != 5 && AllTokensInHome())
    {
        std::cout << "TODAS LAS FICHAS EN CASA" << std::endl;
        EndTurn();
    }
}

void Gameplay::EndTurn() 
{
    _hasRolled = false;
    _isMyTurn = false;
    _currentTime = 0;

    _currentTurnIndex = (_currentTurnIndex + 1) % _numPlayers;
    
    std::cout << "Next turn for: " + _currentTurnIndex << std::endl;

    sf::Packet turnPacket;
    turnPacket << static_cast<int>(MessageType::TURN_CHANGE) << _currentTurnIndex;

    _client->BroadcastToPeers(turnPacket);
}

void Gameplay::HandleNetwork() 
{
    auto packetOpt = _client->WaitForPeerMessage(0.1f); 

    while (packetOpt.has_value())
    {
        sf::Packet packet = packetOpt.value();
        int msgTypeInt;
        packet >> msgTypeInt;

        MessageType msgType = static_cast<MessageType>(msgTypeInt);

        switch (msgType)
        {
            case MessageType::MOVE_REQUEST:
            {
                std::cout << "Move_Rquest" << std::endl;
                int colorInt, fichaId, pos;
                packet >> colorInt >> fichaId >> pos;
                PlayerColor color = static_cast<PlayerColor>(colorInt);

                for (auto* ficha : _enemyFichas)
                {
                    auto* token = ficha->GetComponent<TokenComponent>();

                    if (token->GetColor() == color && token->GetTokenID() == fichaId)
                    {
                        token->SetBoardPosition(pos);

                        if (pos >= 100)
                        {
                            ficha->GetComponent<Transform>()->position = metaPositions[color][pos - 100];
                        }
                        else
                        {
                            ficha->GetComponent<Transform>()->position = mainPathPositions[pos];
                        }

                        std::cout << "[Gameplay] Jugador " << static_cast<int>(color) << " movió ficha " << fichaId << " a pos " << pos << std::endl;
                        break;
                    }
                }
                break;
            }
            case MessageType::TURN_CHANGE:
            {
                int newTurn;
                packet >> newTurn;

                _currentTurnIndex = newTurn;
                _isMyTurn = (_playerIndex == newTurn);

                std::cout << "[Gameplay] Nuevo turno: " << newTurn << (_isMyTurn ? " (mi turno)" : "") << std::endl;
                break;
            }
            case MessageType::DICE_RESULT:
            {
                int dice;
                packet >> dice;
                std::cout << "[Gameplay] Otro jugador ha sacado un " << dice << std::endl;
                break;
            }
            case MessageType::TOKEN_CAPTURED:
            {
                int colorInt, fichaId;
                packet >> colorInt >> fichaId;
                PlayerColor color = static_cast<PlayerColor>(colorInt);

                std::cout << "[Gameplay] Jugador " << colorInt << " ha perdido la ficha " << fichaId << " (capturada)\n";

                if (color == _myColor)
                {
                    for (auto* ficha : _myTokens)
                    {
                        auto* token = ficha->GetComponent<TokenComponent>();
                        if (token && token->GetTokenID() == fichaId)
                        {
                            token->SetTokenState(TokenState::IN_HOME);
                            token->SetBoardPosition(-1);
                            ficha->GetComponent<Transform>()->position = homePositions[color][fichaId];
                            break;
                        }
                    }
                }
                else 
                {
                    for (auto* ficha : _enemyFichas)
                    {
                        auto* token = ficha->GetComponent<TokenComponent>();
                        if (token && token->GetColor() == color && token->GetTokenID() == fichaId)
                        {
                            token->SetTokenState(TokenState::IN_HOME);
                            token->SetBoardPosition(-1);
                            ficha->GetComponent<Transform>()->position = homePositions[color][fichaId];
                            break;
                        }
                    }
                }

                break;
            }
            case MessageType::GAME_OVER:
            {
                int winnerColor;
                packet >> winnerColor;

                if (winnerColor == static_cast<int>(_myColor))
                {
                    std::cout << "[Gameplay] _victory" << std::endl;
                    onWinMatch.Invoke();
                }
                    
                else
                {
                    std::cout << "[Gameplay] _defeat" << std::endl;
                    onLoseMatch.Invoke();
                }
                break;
            }
            default:
                std::cout << "[Gameplay] Mensaje no reconocido: " << msgTypeInt << std::endl;
                break;
        }

        // Siguiente paquete (si hay)
        packetOpt = _client->WaitForPeerMessage(0.f);
    }
}
bool Gameplay::AllTokensInGoal()
{
    for (auto* token : _myTokens)
    {
        if (token->GetTokenState() != TokenState::IN_END)
            return false;
    }
    return true;
}
bool Gameplay::AllTokensInHome()
{
    for (auto token : _myTokens)
    {
        std::cout << "Token: " << static_cast<int>(token->GetTokenState()) << std::endl;
        if (token->GetTokenState() == TokenState::IN_GAME)
        {
            return false;
        }
    }
    return true;
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
            return 18;
        case PlayerColor::RED: 
            return 33;
        case PlayerColor::GREEN:
            return 48;
        default: return 0;
    }
}

void Gameplay::BroadcastMove(int fichaId, int pos) 
{
    sf::Packet movePacket;
    movePacket << static_cast<int>(MessageType::MOVE_REQUEST) << static_cast<int>(_myColor) << fichaId << pos;
    _client->BroadcastToPeers(movePacket);
}

void Gameplay::MoveFichaConNormas(int fichaIndex) 
{
    Token* ficha = _myTokens[fichaIndex];
    TokenComponent* tokenToMove = ficha->GetComponent<TokenComponent>();

    if (tokenToMove->IsInGoal()) return;

    if (_diceValue == 5 && tokenToMove->GetTokenState() == TokenState::IN_HOME)
    {
        //Setear la ficha en startPos y end turn
        int salida = GetStartingIndexForColor(tokenToMove->GetColor());
        tokenToMove->SetTokenState(TokenState::IN_GAME);
        tokenToMove->SetBoardPosition(salida);
        ficha->GetComponent<Transform>()->position = mainPathPositions[salida];
        BroadcastMove(fichaIndex, salida);

        EndTurn();
        return;
    }

    // Si el dado no es 5 y la ficha que clico esta en casa a la verga

    if (tokenToMove->GetTokenState() == TokenState::IN_HOME) return;

    int currentPos = tokenToMove->GetBoardPosition();
    int targetPos = currentPos + _diceValue;

    // -- Mirar si al mover ficha hago captura

    for (auto* rival : _enemyFichas)
    {
        auto* rivalComp = rival->GetComponent<TokenComponent>();
        if (rivalComp->GetBoardPosition() == targetPos)
        {
            rivalComp->SetTokenState(TokenState::IN_HOME);
            rivalComp->SetBoardPosition(-1);
            rival->GetComponent<Transform>()->position = homePositions[rivalComp->GetColor()][rivalComp->GetTokenID()];

            //ENVIAR PAQUETE A LOS DEMAS DE CAPTURA

            std::cout << "[Gameplay] ¡Captura! +20\n";

            sf::Packet capturePacket;
            capturePacket << static_cast<int>(MessageType::TOKEN_CAPTURED)
                << static_cast<int>(rivalComp->GetColor())
                << rivalComp->GetTokenID();

            _client->BroadcastToPeers(capturePacket);

            targetPos += 20;
            break;
        }
    }

    // -- Mirar si al mover ficha entro en la parte final

    int entradaMeta = GetEntryToGoalIndex(_myColor);
    int totalPositions = mainPathPositions.size(); // 60
    int distanceToEntry = (entradaMeta - currentPos + totalPositions) % totalPositions;

    int stepsToMove = targetPos - currentPos;

    if (stepsToMove > distanceToEntry)
    {
        int pasosEnMeta = stepsToMove - distanceToEntry - 1;

        if (pasosEnMeta >= 6 || tokenToMove->GetStepsToGoal() <= stepsToMove)
        {
            tokenToMove->SetTokenState(TokenState::IN_END);
            tokenToMove->SetBoardPosition(100 + 6);
            ficha->GetComponent<Transform>()->position = metaPositions[_myColor][6];
            tokenToMove->AddSteps(stepsToMove);

            std::cout << "[Gameplay] ¡Ficha " << fichaIndex << " ha llegado a la meta!" << std::endl;

            // Seguridad extra
            if (tokenToMove->IsInGoal())
            {
                ficha->GetComponent<Transform>()->position = metaPositions[_myColor][6];
                tokenToMove->SetBoardPosition(100 + 6);
            }
        }
        else
        {
            //tokenToMove->SetTokenState(TokenState::IN_END);
            tokenToMove->SetBoardPosition(100 + pasosEnMeta);
            ficha->GetComponent<Transform>()->position = metaPositions[_myColor][pasosEnMeta];
            tokenToMove->AddSteps(stepsToMove);
        }
    }
    else
    {
        // Wrap_around
        targetPos = (currentPos + stepsToMove) % totalPositions;
        tokenToMove->SetBoardPosition(targetPos);
        ficha->GetComponent<Transform>()->position = mainPathPositions[targetPos];
        tokenToMove->AddSteps(stepsToMove);
    }

    BroadcastMove(fichaIndex, tokenToMove->GetBoardPosition());

    // -- Verificar condición de victoria
    if (AllTokensInGoal())
    {
        std::cout << "[Gameplay] _victory" << std::endl;

        sf::Packet winPacket;
        winPacket << static_cast<int>(MessageType::GAME_OVER) << static_cast<int>(_myColor);
        _client->BroadcastToPeers(winPacket);

        onWinMatch.Invoke();
        return;
    }

    // -- Si el dado es un 6 vuelve a tirar después de mover ficha

    if (_diceValue == 6 || _diceValue == 7)
    {
        _isMyTurn = true;
        _hasRolled = false;
    }
    else
    {
        EndTurn();
    }

}