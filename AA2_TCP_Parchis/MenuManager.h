#pragma once
#include <SFML/Graphics.hpp>
#include "Enums.hpp"
#include "GameObject.h"
#include "LoginMenu.h"
#include "EventHandler.h"
#include "Window.h"



class MenuManager {
public:
    MenuManager();

    void Init();
    void Run();
    void NextMenu(); 
    MenuState GetCurrentState() const;

    ~MenuManager();

private:
    MenuState _currentState;
    sf::Clock _menuTimer;
    sf::Clock _deltaClock;


    GameObject* _splashScreen;
    LoginMenu* _loginMenu;
    GameObject* _matchmakingMenu;

    sf::RenderWindow* _window;
    EventHandler* _eventHandler;

    void Update(float deltaTime);
    void Render();
    void UpdateState(MenuState state);
    void HandleEvents();


    void InitSplash();
    void InitLogin();
    void InitMatchmaking();
};
