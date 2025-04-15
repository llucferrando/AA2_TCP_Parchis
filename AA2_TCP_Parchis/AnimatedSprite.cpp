#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(const std::string& texturePath, sf::Vector2i frameSize, int frameCount, float frameTime)
	: _texture(), _sprite(_texture), _frameSize(frameSize), _frameCount(frameCount), _frameTime(frameTime)
{
	_texture.loadFromFile(texturePath);
    _sprite.setTextureRect(sf::IntRect(sf::Vector2i(_currentFrame * _frameSize.x, 0), _frameSize));
}

void AnimatedSprite::Update(float deltaTime)
{
    _currentTime += deltaTime;

    if (_currentTime >= _frameTime)
    {
        _currentTime = 0.f;
        _currentFrame = (_currentFrame + 1) % _frameCount;

        int left = _currentFrame * _frameSize.x;
        _sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), _frameSize));
    }
}

void AnimatedSprite::Draw(sf::RenderWindow* window, Transform* transform)
{
    _sprite.setPosition(transform->position);
    _sprite.setRotation(transform->rotation); 
    _sprite.setScale(transform->scale);
    window->draw(_sprite);
}

const std::type_index AnimatedSprite::GetType()
{
    return typeid(AnimatedSprite);
}