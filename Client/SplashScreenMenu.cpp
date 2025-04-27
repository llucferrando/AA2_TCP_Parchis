#include "SplashScreenMenu.h"

SplashScreenMenu::SplashScreenMenu()
{
	AddComponent<SpriteRenderer>("Assets/Splashscreen/splash.png", 10);
}

void SplashScreenMenu::Render(sf::RenderWindow* window)
{
	GetComponent<SpriteRenderer>()->Draw(window, GetComponent<Transform>());
}
