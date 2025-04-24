#include "GameManager.h"
// --Change after where belong
#define FRAME_W 96
#define FRAME_H 101
#define NUM_FRAMES 10
#define ANIMATION_SPEED 0.1
#include <SFML/System/Vector2.hpp>

GameManager::GameManager()
{
    _window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "nuget");
    _eventHandler = new EventHandler();

    _ficha = new GameObject();
    _splashScreen = new GameObject();

}

GameManager::~GameManager()
{
	Shutdown();
}

void GameManager::Init()
{
    _ficha->AddComponent<AnimatedSprite>(
        "Assets/Spritesheets/S_Link.png",
        sf::Vector2i(FRAME_W, FRAME_H),
        NUM_FRAMES,
        ANIMATION_SPEED
    );
    _splashScreen->AddComponent<SpriteRenderer>("Assets/Splashscreen/splash.png");
    _splashScreen->GetComponent<Transform>()->scale = sf::Vector2f(0.7f, .7f);

    _eventHandler->onClick.Subscribe([this](sf::Vector2f clickPos)
        {
            _splashScreen->GetComponent<Transform>()->scale = sf::Vector2f(5.f, 5.f);
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
    //Clear fichas y vaynas
}

void GameManager::Update(float deltaTime)
{
    //Update fichas o lo k sea
    _ficha->GetComponent<AnimatedSprite>()->Update(deltaTime);
}

void GameManager::Render()
{
    _window->clear();
    //-- Draw things
    _ficha->GetComponent<AnimatedSprite>()->Draw(_window, _ficha->GetComponent<Transform>());
    _splashScreen->GetComponent<SpriteRenderer>()->Draw(_window, _splashScreen->GetComponent<Transform>());

    //--
    _window->display();
}

void GameManager::HandleEvents()
{
    while (const std::optional event = _window->pollEvent())//si veo un evento, automaticamente se consume (un buffer)
    {
        _eventHandler->HandleEvent(*event, *_window);
    }
}

void GameManager::SplashScreen()
{
    //GameObject splashGO;
    //splashGO.AddComponent<SpriteRenderer>("Assets/Splashscreen/splash.png");

    //const float splashDuration = 3.0f;
    //sf::Clock clock;

    //while (_window->IsOpen() && clock.getElapsedTime().asSeconds() < splashDuration)
    //{
    //    _window->Clear();

    //   

    //    _window->Display();
    //}


}
