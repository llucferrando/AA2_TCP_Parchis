#include "GameManager.h"


GameManager::GameManager()
{
    _window = new Window();
    _client = new Client();
    _eventHandler = new EventHandler();
    _currentState = GameState::None;
    _loginMenu = nullptr;
    _splashMenu = nullptr;
    _matchmakingMenu = nullptr;
    _gameOverMenu = nullptr;
    UpdateState(GameState::SplashScreen);
    _splashTime = 0;

    std::srand(std::time(nullptr));
}

GameManager::~GameManager()
{
	Shutdown();
}

void GameManager::Init()
{
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

    switch (_currentState) 
    {
        case GameState::SplashScreen:
            delete _splashMenu;
            _splashMenu = nullptr;
            break;

        case GameState::LoginMenu:
            delete _loginMenu;
            _loginMenu = nullptr;
            break;

        case GameState::MatchmakingMenu:
            delete _matchmakingMenu;
            _matchmakingMenu = nullptr;
            break;

        case GameState::GameOver:
            delete _matchmakingMenu;
            _matchmakingMenu = nullptr;
            break;
        
        default: break;
    }

    switch (newState) 
    {
        case GameState::SplashScreen:
            std::cout << "[Client] Opening Splashscreen" << std::endl;
            _splashMenu = new SplashScreenMenu();
            break;
        case GameState::LoginMenu:
            std::cout << "[Client] Opening Login Menu" << std::endl;
            _loginMenu = new LoginMenu(_eventHandler, _client);
            _loginMenu->onLoginSucces.Subscribe([this]() {     _nextState = GameState::MatchmakingMenu;
            _shouldChangeState = true; });
            _loginMenu->onRegisterSucces.Subscribe([this]() { std::cout << "Registered succesfully..." << std::endl; });
            break;

        case GameState::MatchmakingMenu:
            std::cout << "[Client] Opening Matchmaking Menu" << std::endl;
            _matchmakingMenu = new MatchmakingMenu(_eventHandler, _client);
            break;

        case GameState::GameOver:
            std::cout << "[Client] Opening GameOver Menu" << std::endl;
            //_gameOverMenu = new GameOverMenu(_eventHandler, _client);

            break;

        default: break;
    }

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

    if (_shouldChangeState) {
        UpdateState(_nextState);
        _shouldChangeState = false;
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


