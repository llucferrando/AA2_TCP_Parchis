#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const std::string & texturePath, int colorID) : _texture(), _sprite(_texture), _colorID(colorID)
{
    _texture.loadFromFile(texturePath);
    _sprite.setTexture(_texture, true);
    _sprite.setColor(GetColorFromPlayer(colorID));
}

void SpriteRenderer::Draw(sf::RenderWindow* window, Transform* transform)
{
    _sprite.setPosition(transform->position);
    _sprite.setRotation(transform->rotation);
    _sprite.setScale(transform->scale);
    window->draw(_sprite);
}

sf::Color SpriteRenderer::GetColorFromPlayer(int index)
{
	switch (static_cast<PlayerColor>(index))
	{
	case PlayerColor::RED:    return sf::Color::Red;
	case PlayerColor::BLUE:   return sf::Color::Blue;
	case PlayerColor::GREEN:  return sf::Color::Green;
	case PlayerColor::YELLOW: return sf::Color::Yellow;

	default:                  return sf::Color::White;
	}
}

const std::type_index SpriteRenderer::GetType()
{
    return typeid(SpriteRenderer);
}