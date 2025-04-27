#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Transform.h"

class AnimatedSprite : public Component
{
public:

	AnimatedSprite(const std::string& texturePath, sf::Vector2i frameSize, int frameCount, float frameTime);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow* window, Transform* transform);
	const std::type_index GetType() override;

private:
    sf::Texture _texture;
    sf::Sprite _sprite;

    sf::Vector2i _frameSize;
    int _frameCount;
    float _frameTime; // En segundos
    float _currentTime = 0.f;
    int _currentFrame = 0;
};