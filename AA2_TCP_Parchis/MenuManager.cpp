#include "MenuManager.h"
#include "SpriteRenderer.h"
#include "LoginMenu.h"

MenuManager::MenuManager() : _currentState(MenuState::SplashScreen)
{
    _splashScreen = new GameObject();
    _loginMenu = new LoginMenu();
    _matchmakingMenu = nullptr; 
    _window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Parchis Online");
    _eventHandler = new EventHandler();
    
}

MenuManager::~MenuManager()
{
    delete _splashScreen;
    delete _loginMenu;
    delete _matchmakingMenu;

}

void MenuManager::Init()
{
    InitSplash();
    _menuTimer.restart();
}


void MenuManager::Update(float deltaTime)
{
    if (_currentState == MenuState::SplashScreen && _menuTimer.getElapsedTime().asSeconds() > 3.0f)
    {
        UpdateState(MenuState::LoginMenu);
        InitLogin();
    }
    if(_currentState==MenuState::LoginMenu && _loginMenu)
    {
        _loginMenu->Update(deltaTime);
    }

}

void MenuManager::Render()
{
    switch (_currentState)
    {
    case MenuState::SplashScreen:
        _splashScreen->GetComponent<SpriteRenderer>()->Draw(_window, _splashScreen->GetComponent<Transform>());
        break;
    case MenuState::LoginMenu:
        if (_loginMenu) _loginMenu->Render(_window);
        break;
    case MenuState::MatchmakingMenu:
        break;
    case MenuState::InGame:
        break;
    }
}

void MenuManager::Run()
{
    while (_window->isOpen()) {

        float deltaTime = _deltaClock.restart().asSeconds();

        HandleEvents();
        Update(deltaTime);
        Render();
    }

}

void MenuManager::NextMenu()
{
    if (_currentState == MenuState::LoginMenu)
        UpdateState(MenuState::MatchmakingMenu);
    else if (_currentState == MenuState::MatchmakingMenu)
        UpdateState(MenuState::InGame);
}

void MenuManager::UpdateState(MenuState newState)
{
    if (_currentState == newState) return;
    _currentState = newState;
}

void MenuManager::HandleEvents()
{
}

MenuState MenuManager::GetCurrentState() const
{
    return _currentState;
}



void MenuManager::InitSplash()
{
    _splashScreen->AddComponent<SpriteRenderer>("Assets/Splashscreen/splash.png");
    auto* t = _splashScreen->GetComponent<Transform>();
    t->position = { 0.f, 0.f };
    t->scale = { 1.f, 1.f };
}

void MenuManager::InitLogin()
{
    _loginMenu->Init();
}

void MenuManager::InitMatchmaking()
{

}
