#include "GameManager.h"


GameManager::GameManager()
{
    _window = new Window();
    _eventHandler = new EventHandler();

    _currentState = GameState::SplashScreen;
    _splashTime = 0;

    //_backgroundGO = new GameObject();
    //_backgroundGO->AddComponent<SpriteRenderer>("Assets/GameAssets/Parchis.png", 10);


    //_matchManager = new MatchManager();
    //_boardManager = new BoardManager();
    //_gameRules = new GameRules(_boardManager);

    std::srand(std::time(nullptr));

}

GameManager::~GameManager()
{
	Shutdown();
}

void GameManager::Init()
{

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

    delete _eventHandler;
    delete _window;
}

void GameManager::UpdateState(GameState newState)
{
    if (_currentState == newState) return;

    _currentState = newState;
}

void GameManager::Update(float deltaTime)
{
    switch (_currentState) {
        case GameState::SplashScreen:
            _splashTime += deltaTime;
            if (_splashTime > 2) {
                _currentState = GameState::LoginMenu;
            }
            break;

        case GameState::LoginMenu:
            //_loginMenu->Update(deltaTime);
            break;

        case GameState::MatchmakingMenu:
            //_matchmakingMenu->Update(deltaTime);
            break;

        case GameState::Gameplay:
            //UpdateGameplay(deltaTime);
            break;
    }
}

void GameManager::Render()
{
    _window->Clear();



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

