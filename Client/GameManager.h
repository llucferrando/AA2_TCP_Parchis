#pragma once
#include <SFML/Graphics.hpp>
#include "EventHandler.h"
#include "Enums.hpp"
#include "LoginMenu.h"
#include "SplashScreenMenu.h"
#include <SFML/Network.hpp>
#include "Client.h"
#include "MatchmakingMenu.h"

class GameManager
{
public:
    GameManager();
    ~GameManager();
    void Init();      // Load resources, init fichas, int all game things
    void Run();       // Main loop
    void Shutdown();  // Clear resources or whatever
    void UpdateState(GameState newState);

private:
    void Update(float deltaTime);
    void Render();
    void HandleEvents();
    void MatchmakingMenuLogic();
    int RollDice();
    //void SendLogin(const std::string& username, const std::string& password);

    GameState _currentState;
    sf::TcpSocket _socket;
    Window* _window;
    EventHandler* _eventHandler;
    sf::Clock _deltaClock;

    Client* _client;

    // -- Menus -- //
    LoginMenu* _loginMenu;
    MatchmakingMenu* _matchmakingMenu;
    SplashScreenMenu* _splashMenu;


    bool waitingSelection = false;
    int diceValue = 0;
    int _currentPlayer = 0;

    float _splashTime;
    
};

