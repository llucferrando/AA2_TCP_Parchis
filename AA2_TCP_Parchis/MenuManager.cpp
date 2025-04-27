#include "MenuManager.h"
#include "SpriteRenderer.h"
#include "LoginMenu.h"

MenuManager::MenuManager() : _currentState(MenuState::SplashScreen)
{
    _window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Parchis Online");
    _splashScreen = new GameObject();
    _loginMenu = new LoginMenu();
    //_matchmakingMenu = nullptr; 
    //_eventHandler = new EventHandler();
    
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
    switch (_currentState) {
    case MenuState::LoginMenu:
        _loginMenu->Update(deltaTime);
    case MenuState::MatchmakingMenu:
        //_loginMenu->Update(deltaTime);
    case MenuState::InGame:
            break;
    }
    

}

void MenuManager::Run()
{
    while (_window->isOpen()) {

        float deltaTime = _deltaClock.restart().asSeconds();

        //HandleEvents();
        Update(deltaTime);
        Render();
    }

}

void MenuManager::Render()
{
    _window->clear();

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

    _window->display();
    
}


void MenuManager::UpdateState(MenuState newState)
{
    if (_currentState == newState) return;
    _currentState = newState;
}

void MenuManager::HandleEvents()
{
    while (const std::optional event = _window->pollEvent())
    {
        _eventHandler->HandleEvent(*event, *_window);
    }
   

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
