#include "GameManager.h"


GameManager::GameManager()
{
    _window = new Window();
    _client = new Client();
    _eventHandler = new EventHandler();
    _currentState = GameState::SplashScreen;
    _loginMenu = new LoginMenu(_eventHandler, _client);
    _matchmakingMenu = new MatchmakingMenu(_eventHandler, _client);
    _splashMenu = new SplashScreenMenu();
    _splashTime = 0;

    std::srand(std::time(nullptr));
}

GameManager::~GameManager()
{
	Shutdown();
}

void GameManager::Init()
{
    _loginMenu->onLoginSucces.Subscribe([this]() { UpdateState(GameState::MatchmakingMenu); });
    _loginMenu->onRegisterSucces.Subscribe([this]() { std::cout << "Registered succesfully..." << std::endl; });
    _client->ConnectToBootstrapServer("127.0.0.1", 50000);
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
    delete _loginMenu;
    delete _matchmakingMenu;
    delete _splashMenu;
    delete _client;
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

            if (_splashTime > 2) 
            {
                UpdateState(GameState::LoginMenu);
                _splashTime = 0;
            }
            break;

        case GameState::LoginMenu:
            _loginMenu->Update(deltaTime);
            break;

        case GameState::MatchmakingMenu:
            _matchmakingMenu->Update(deltaTime);
            break;

        case GameState::Gameplay:
            //UpdateGameplay(deltaTime);
            break;
    }
}

void GameManager::Render()
{
    _window->Clear();

    switch (_currentState) {

        case GameState::SplashScreen:
            _splashMenu->Render(_window->GetWindow());
            break;

        case GameState::LoginMenu:
            _loginMenu->Render(_window->GetWindow());
            break;

        case GameState::MatchmakingMenu:
           _matchmakingMenu->Render(_window->GetWindow());
            break;

        case GameState::Gameplay:
            break;
        }

    _window->Display();
}

void GameManager::HandleEvents()
{
    while (const std::optional event = _window->GetWindow()->pollEvent())
    {
        _eventHandler->HandleEvent(*event, *_window);
    }
}

int GameManager::RollDice()
{
    return rand() % 6 + 1;
}


