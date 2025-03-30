#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Transform.h"

class SpriteRenderer : public Component 
{
public:
	SpriteRenderer(const std::string& texturePath);

	void Draw(sf::RenderWindow& window, Transform* transform);

	const std::type_index GetType() override;

private:
	sf::Texture _texture;
	sf::Sprite _sprite;
};