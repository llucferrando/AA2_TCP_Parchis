#include "ClickableComponent.h"

ClickableComponent::ClickableComponent(EventHandler* handler, Transform* transform, SpriteRenderer* spriteRenderer) : _spriteRenderer(spriteRenderer), _transform(transform)
{
    handler->onClick.Subscribe([this](sf::Vector2f pos) 
        {
        if (!_transform || !_spriteRenderer) return;

        sf::FloatRect bounds = _spriteRenderer->GetSprite().getGlobalBounds();

        bounds.position = _transform->position;

        if (bounds.contains(pos)) {
            onClick.Invoke();
        }
        });
}

ClickableComponent::~ClickableComponent()
{
	onClick.UnsubscribeAll();
}

const std::type_index ClickableComponent::GetType()
{
	return typeid(ClickableComponent);
}

void ClickableComponent::OnGlobalClick(sf::Vector2f pos)
{
}
