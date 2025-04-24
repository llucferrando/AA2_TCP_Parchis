#pragma once
#include <SFML/Graphics.hpp>
#include "EventHandler.h"
#include "MenuManager.h"
#include "MatchManager.h"
#include "BoardManager.h"
#include "GameRules.h"
#include "Enums.hpp"

class GameManager
{
public:
    GameManager();
    ~GameManager();
    void Init();      // Load resources, init fichas, int all game things
    void Run();       // Main loop
    void Shutdown();  // Clear resources or whatever

private:
    void Update(float deltaTime);
    void Render();
    void HandleEvents();
    int RollDice();
    

    Window* _window;
    EventHandler* _eventHandler;
    sf::Clock _deltaClock;

    MenuManager * _menuManager;
    BoardManager* _boardManager;
    MatchManager* _matchManager;
    GameRules* _gameRules;

    GameObject* _backgroundGO;

    bool waitingSelection = false;
    int diceValue = 0;
    int _currentPlayer = 0;
    
};

