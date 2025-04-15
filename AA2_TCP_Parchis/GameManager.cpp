#include "GameManager.h"
// --Change after where belong
#define FRAME_W 96
#define FRAME_H 101
#define NUM_FRAMES 10
#define ANIMATION_SPEED 0.1

GameManager::GameManager()
{
    _window = new Window();
    _eventHandler = new EventHandler();

    _ficha = new GameObject();
    _ficha->AddComponent<AnimatedSprite>(
        "Assets/Spritesheets/S_Link.png",
        sf::Vector2i(FRAME_W, FRAME_H),
        NUM_FRAMES,
        ANIMATION_SPEED
        );
}

GameManager::~GameManager()
{
	Shutdown();
}

void GameManager::Init()
{
}

void GameManager::Run()
{
    while (_window->IsOpen()) {

        float deltaTime = _deltaClock.restart().asSeconds();

        HandleEvents();
        Update(deltaTime);
        Render();
    }
}

void GameManager::Shutdown()
{
    //Clear fichas y vaynas
}

void GameManager::Update(float deltaTime)
{
    //Update fichas o lo k sea
    _ficha->GetComponent<AnimatedSprite>()->Update(deltaTime);
}

void GameManager::Render()
{
    _window->Clear();
    //-- Draw things
    _ficha->GetComponent<AnimatedSprite>()->Draw(_window->GetWindow(), _ficha->GetComponent<Transform>());
    //--
    _window->Display();
}

void GameManager::HandleEvents()
{
    while (const std::optional event = _window->GetWindow()->pollEvent())//si veo un evento, automaticamente se consume (un buffer)
    {
        _eventHandler->HandleEvent(*event, *_window);
    }
}
