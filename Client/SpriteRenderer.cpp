#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const std::string & texturePath, sf::Color color) : _texture(), _sprite(_texture)
{
    _texture.loadFromFile(texturePath);
    _sprite.setTexture(_texture, true);
	_sprite.setColor(color);
}

void SpriteRenderer::Draw(sf::RenderWindow* window, Transform* transform)
{
    _sprite.setPosition(transform->position);
    _sprite.setRotation(transform->rotation);
    _sprite.setScale(transform->scale);
    window->draw(_sprite);
}

sf::Sprite SpriteRenderer::GetSprite()
{
    return _sprite;
}

const std::type_index SpriteRenderer::GetType()
{
    return typeid(SpriteRenderer);
}