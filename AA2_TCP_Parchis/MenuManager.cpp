#include "MenuManager.h"
#include "SpriteRenderer.h"
#include "LoginMenu.h"

MenuManager::MenuManager() : _currentState(MenuState::SplashScreen)
{
    _splashScreen = new GameObject();
    _loginMenu = new LoginMenu();
    _matchmakingMenu = nullptr; // Cuando tengas MatchmakingMenu, inicialízalo igual
}

void MenuManager::Init()
{
    InitSplash();
    _menuTimer.restart();
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
    if (_loginMenu) {
        _loginMenu->Init();
    }
}

void MenuManager::InitMatchmaking()
{
    // Preparar matchmaking menu aquí
}

void MenuManager::Update(float deltaTime)
{
    if (_currentState == MenuState::SplashScreen && _menuTimer.getElapsedTime().asSeconds() > 3.0f)
    {
        UpdateState(MenuState::LoginMenu);
        InitLogin();
    }

    // Update dinámico según el estado
    switch (_currentState) {
    case MenuState::LoginMenu:
        if (_loginMenu) _loginMenu->Update(deltaTime);
        break;
    case MenuState::MatchmakingMenu:
        // matchmakingMenu->Update(deltaTime);
        break;
    default:
        break;
    }
}

void MenuManager::Render(sf::RenderWindow* window)
{
    switch (_currentState)
    {
    case MenuState::SplashScreen:
        _splashScreen->GetComponent<SpriteRenderer>()->Draw(window, _splashScreen->GetComponent<Transform>());
        break;
    case MenuState::LoginMenu:
        if (_loginMenu) _loginMenu->Render(window);
        break;
    case MenuState::MatchmakingMenu:
        // matchmakingMenu->Render(window);
        break;
    case MenuState::InGame:
        // gameInterface->Render(window);
        break;
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

MenuState MenuManager::GetCurrentState() const
{
    return _currentState;
}
