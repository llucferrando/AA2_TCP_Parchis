#pragma once
#include <SFML/Graphics.hpp>
#include "MenuState.h"
#include "GameObject.h"
#include "LoginMenu.h"


class MenuManager {
public:
    MenuManager();

    void Init();
    void Update(float deltaTime);
    void Render(sf::RenderWindow* window);
    void NextMenu(); 
    MenuState GetCurrentState() const;

private:
    MenuState _currentState;
    sf::Clock _menuTimer;

    GameObject* _splashScreen;
    LoginMenu* _loginMenu;
    GameObject* _matchmakingMenu;

    void InitSplash();
    void InitLogin();
    void InitMatchmaking();
};
