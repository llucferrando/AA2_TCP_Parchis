#include "GameManager.h"


GameManager::GameManager()
{
    //_window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "nuget");
    _backgroundGO = new GameObject();
    _backgroundGO->AddComponent<SpriteRenderer>("Assets/GameAssets/Parchis.png", 10);

    _window = new Window();
    _eventHandler = new EventHandler();

    _matchManager = new MatchManager();
    _boardManager = new BoardManager();
    _gameRules = new GameRules(_boardManager);

    std::srand(std::time(nullptr));

}

GameManager::~GameManager()
{
	Shutdown();
}

void GameManager::Init()
{
    _matchManager->CreatePlayers(2);
    _matchManager->CreateTokens(4);

    _eventHandler->onClick.Subscribe([this](sf::Vector2f clickPos)
        {
            if (!waitingSelection) return;

            for (auto ficha : _matchManager->GetAllFichas()) {

                auto transform = ficha->GetComponent<Transform>();
                if (!transform) continue;

                sf::FloatRect fichaRect(transform->position, { 40, 40 }); // Dimension of the token 
                auto token = ficha->GetComponent<TokenComponent>();

                if (token && token->GetPlayerID() == _currentPlayer && fichaRect.contains(clickPos)) {

                    if (_gameRules->MustBreakBarrier(token, _matchManager->GetAllTokenComponents()) && diceValue == 6) {
                        // se permite romper barrera
                    }

                    int destino = _gameRules->CalculateDestination(token, diceValue, _matchManager->GetAllTokenComponents());

                    token->SetBoardPosition(destino);

                    token->SetTokenState(TokenState::IN_GAME);

                    transform->position = _boardManager->GetWorldPosition(destino);

                    waitingSelection = false;

                    if (_gameRules->HasEatenOpponent(destino, _currentPlayer, _matchManager->GetAllTokenComponents())) {
                        // TODO: lógica de comer + avanzar 20
                    }

                    if (diceValue == 6) {
                        diceValue = RollDice();
                        waitingSelection = true;
                    }
                    else {
                        _matchManager->NextTurn();
                    }
                    break;
                }
            }
        });
}

void GameManager::Run()
{
    while (_window->IsOpen()) {

        float deltaTime = _deltaClock.restart().asSeconds();

        HandleEvents();
        Update(deltaTime);
        Render();
    }
}

void GameManager::Shutdown()
{    
    delete _matchManager;
    delete _boardManager;
    delete _gameRules;
    delete _eventHandler;
    delete _window;
}

void GameManager::Update(float deltaTime)
{
    if (!waitingSelection) {

        diceValue = RollDice();
        
        _currentPlayer = _matchManager->GetCurrentPlayerTurn();

        bool canMove = false;

        for (auto ficha : _matchManager->GetAllFichas()) {

            auto token = ficha->GetComponent<TokenComponent>();
            
            if (token && token->GetPlayerID() == _currentPlayer && _gameRules->CanMove(token, diceValue)) {
                canMove = true;
                break;
            }
        }

        if (!canMove) {
            _matchManager->NextTurn();
        }
        else {
            waitingSelection = true;
        }
    }
}

void GameManager::Render()
{
    _window->Clear();

    _backgroundGO->GetComponent<SpriteRenderer>()->Draw(_window->GetWindow(), _backgroundGO->GetComponent<Transform>());

    for (auto ficha : _matchManager->GetAllFichas()) {
        auto transform = ficha->GetComponent<Transform>();
        if (transform) {
            sf::CircleShape shape(20);
            shape.setPosition(transform->position);
            shape.setFillColor(sf::Color::Red);
            _window->GetWindow()->draw(shape);
        }
    }

    _window->Display();
}

void GameManager::HandleEvents()
{
    while (const std::optional event = _window->GetWindow()->pollEvent())//si veo un evento, automaticamente se consume (un buffer)
    {
        _eventHandler->HandleEvent(*event, *_window);
    }
}

int GameManager::RollDice()
{
    return rand() % 6 + 1;
}

