#include "GameManager.h"
//#define FRAME_W 1024
//#define FRAME_H 1024
//#define NUM_FRAMES 10
//#define ANIMATION_SPEED 0.1
#include <SFML/System/Vector2.hpp>

GameManager::GameManager()
{
    _eventHandler = new EventHandler();


}

GameManager::~GameManager()
{
	Shutdown();
}

void GameManager::Init()
{
    _eventHandler->onClick.Subscribe([this](sf::Vector2f clickPos)
        {
        });
}

void GameManager::Run()
{
    while (_window->isOpen()) {

        float deltaTime = _deltaClock.restart().asSeconds();

        HandleEvents();
        Update(deltaTime);
        Render();
    }
}

void GameManager::Shutdown()
{
    
}

void GameManager::Update(float deltaTime)
{
}

void GameManager::Render()
{
    _window->clear();
   
    _window->display();
}

void GameManager::HandleEvents()
{
    while (const std::optional event = _window->pollEvent())//si veo un evento, automaticamente se consume (un buffer)
    {
        _eventHandler->HandleEvent(*event, *_window);
        
    }
}

