#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720
#define FRAME_W 96
#define FRAME_H 101
#define NUM_FRAMES 10
#define ANIMATION_SPEED 0.1


void UpdateSprite(sf::Sprite& animatedSprite, int& currentFrame, float& deltaTimeAnimation) {

	if (deltaTimeAnimation >= ANIMATION_SPEED) {
		currentFrame = (currentFrame + 1) % NUM_FRAMES; //Si paso de largo, vuelvo al principio
		animatedSprite.setTextureRect(sf::IntRect({ currentFrame * FRAME_W, 0 }, { FRAME_W, FRAME_H }));
	}
}

sf::Texture LoadSpriteSheet(const std::string& path) {
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cerr << "aaaaaa" << std::endl;
		return texture;
	}
	return texture;
}

void Render(sf::RenderWindow& window,/*sf::RectangleShape& square*/ sf::Sprite& animatedSprite) {

	window.clear(sf::Color(0x000000FF));
	//window.draw(square);
	window.draw(animatedSprite);
	window.display();
}

void HandleEvent(const sf::Event& event, sf::RenderWindow& window) {

	//Evita que crashee --> Si cierro la window, hago que se cierre el programa por el while
	if (event.is<sf::Event::Closed>()) { //.is Permite comparar todos los eventos
		window.close();
	}


	//Teclas de Teclado
	if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>()) { //Si se cumple, me lo almacenas en la variable
		switch (keyPressed->code)
		{
		case sf::Keyboard::Key::Escape:
			window.close();
			break;
		default:
			break;
		}
	}

	//Mouse
	if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
		switch (mousePressed->button)
		{
		case sf::Mouse::Button::Left:
			std::cout << "Left mouse button pressed at: " << mousePressed->position.x << ", " << mousePressed->position.y << std::endl;
			break;
		default:
			break;
		}
	}

}

int main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "nuget");

	//Asignamos un clock a cada cosa
	sf::Clock deltaTimeClock;
	float deltaTimeAnimation = 0.f;
	int currentFrame = 0;


	sf::RectangleShape square;
	square.setSize({ 100.f, 100.f });
	square.setFillColor(sf::Color::Magenta);
	square.setPosition({ WIDTH * 0.5f - square.getSize().x * 0.5f, HEIGHT * 0.5f - square.getSize().x * 0.5f });

	sf::Texture spriteSheetTexture = LoadSpriteSheet("Assets/Spritesheets/S_Link.png");
	sf::Sprite animatedSprite = sf::Sprite(spriteSheetTexture);

	sf::SoundBuffer buffer("Assets/Sounds/FX_Quack.wav");
	sf::Sound sound(buffer);
	sound.play();

	while (window->isOpen())
	{
		float deltaTime = deltaTimeClock.restart().asSeconds();
		deltaTimeAnimation += deltaTime;

		while (const std::optional event = window->pollEvent())//si veo un evento, automaticamente se consume (un buffer)
		{
			HandleEvent(*event, *window);
		}
		UpdateSprite(animatedSprite, currentFrame, deltaTimeAnimation);
		Render(*window, /*square*/animatedSprite);
	}
	delete window;
}