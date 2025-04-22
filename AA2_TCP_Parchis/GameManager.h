#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObject.h"
#include "AnimatedSprite.h"
#include "SpriteRenderer.h"
#include "Window.h"
#include "EventHandler.h"

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
    void SplashScreen();

    sf::RenderWindow* _window;
    EventHandler* _eventHandler;
    sf::Clock _deltaClock;

    GameObject* _ficha;
    GameObject* _splashScreen;
};

