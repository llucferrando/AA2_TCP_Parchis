#include "GameManager.h"


GameManager::GameManager()
{
    _window = new Window();
    _eventHandler = new EventHandler();
    _currentState = GameState::SplashScreen;
    _loginMenu = new LoginMenu(_eventHandler);
    _splashMenu = new SplashScreenMenu();



    _loginMenu->GetLoginButton()->onClick.Subscribe([this]() 
        {
            //Si login es correcto con BBDD UPdate to matchmaking sino nada
            std::string username = _loginMenu->GetUsernameText();
            std::string password = _loginMenu->GetPaswwordText();
            std::cout << "SendLogin";
            SendLogin(username, password);
        });

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
    _splashTime = 0;
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

            if (_splashTime > 2) 
            {
                UpdateState(GameState::LoginMenu);
                _splashTime = 0;
            }
            break;

        case GameState::LoginMenu:
            _loginMenu->Update(deltaTime);
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

    switch (_currentState) {

        case GameState::SplashScreen:
            _splashMenu->Render(_window->GetWindow());
            break;

        case GameState::LoginMenu:
            _loginMenu->Render(_window->GetWindow());
            break;

        case GameState::MatchmakingMenu:
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

void GameManager::SendLogin(const std::string& username, const std::string& password) {

    sf::Packet packet;
    packet << "LOGIN";
    _socket.send(packet);

    sf::Packet credentials;
    credentials << username << password;
    _socket.send(credentials);

    sf::Packet response;
    if (_socket.receive(response) == sf::Socket::Status::Done) 
    {
        std::string reply;
        response >> reply;

        if (reply == "LOGIN_OK") 
        {
            UpdateState(GameState::MatchmakingMenu);
        }
        else 
        {
            std::cout << "Login failed" << std::endl;
            // Aquí podrías mostrar un mensaje de error en pantalla
        }
    }
}

