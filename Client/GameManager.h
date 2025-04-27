#pragma once
#include <SFML/Graphics.hpp>
#include "EventHandler.h"

#include "Enums.hpp"

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
    int RollDice();
    
    GameState _currentState;

    Window* _window;
    EventHandler* _eventHandler;
    sf::Clock _deltaClock;



    bool waitingSelection = false;
    int diceValue = 0;
    int _currentPlayer = 0;

    float _splashTime;
    
};

