#include "GameManager.h"


GameManager::GameManager()
{
    _window = new Window();
    _eventHandler = new EventHandler();
    _currentState = GameState::SplashScreen;
    _loginMenu = new LoginMenu(_eventHandler);
    _matchmakingMenu = new MatchmakingMenu(_eventHandler);
    _splashMenu = new SplashScreenMenu();
    _client = new Client();


    _loginMenu->GetLoginButton()->onClick.Subscribe([this]() 
        {
            //Si login es correcto con BBDD UPdate to matchmaking sino nada
            std::string username = _loginMenu->GetUsernameText();
            std::string password = _loginMenu->GetPasswordText();
            std::cout << "SendLogin  " + username + "   " + password << std::endl;
            if (_client->SendLogin(username, password))
            {
                sf::Packet response;
                if (_client->ReceivePacket(response))
                {
                    std::string reply;
                    response >> reply;

                    if (reply == "LOGIN_OK")
                    {
                        UpdateState(GameState::MatchmakingMenu);
                        
                    }
                    else
                    {
                        std::cout << "Login fallido: " << reply << std::endl;                        
                    }
                }
                else
                {
                    std::cout << "Error recibiendo respuesta del servidor." << std::endl;
                }
            }
            else
            {
                std::cout << "Error enviando login." << std::endl;
            }
        });

    _loginMenu->GetRegisterButton()->onClick.Subscribe([this]()
        {
            //Si register es correcto con BBDD UPdate to matchmaking sino nada
            std::string username = _loginMenu->GetUsernameText();
            std::string password = _loginMenu->GetPasswordText();
            std::cout << "Send Register  " + username + "   " + password << std::endl;
            if (_client->SendRegister(username, password))
            {
                sf::Packet response;
                if (_client->ReceivePacket(response))
                {
                    std::string reply;
                    response >> reply;

                    if (reply == "REGISTER_OK")
                    {
                        _window->Clear();
                        UpdateState(GameState::MatchmakingMenu);
                    }
                    else
                    {
                        std::cout << "Register fallido: " << reply << std::endl;
                    }
                }
                else
                {
                    std::cout << "Error recibiendo respuesta del servidor." << std::endl;
                }
            }
            else
            {
                std::cout << "Error enviando login." << std::endl;
            }
        });

    std::srand(std::time(nullptr));

}

GameManager::~GameManager()
{
	Shutdown();
}

void GameManager::Init()
{
    _splashTime = 0;
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


