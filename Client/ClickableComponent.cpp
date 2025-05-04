#include "ClickableComponent.h"

ClickableComponent::ClickableComponent(EventHandler* handler, Transform* transform, SpriteRenderer* spriteRenderer) : _spriteRenderer(spriteRenderer), _transform(transform)
{
    handler->onClick.Subscribe([this](sf::Vector2f pos) 
        {
        if (!_transform || !_spriteRenderer) return;

        sf::FloatRect bounds = _spriteRenderer->GetSprite().getGlobalBounds();

        bounds.position = {
            _transform->position.x - bounds.size.x / 2.f,
            _transform->position.y - bounds.size.y / 2.f
        };

        //float padding = 10.f;
        //bounds.position.x -= padding;
        //bounds.position.y -= padding;
        //bounds.size.x += padding * 2;
        //bounds.size.y += padding * 2;

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

void ClickableComponent::RenderTest(sf::RenderWindow* window)
{
    // Debug DRAW
    //sf::FloatRect bounds = _spriteRenderer->GetSprite().getGlobalBounds();

    //bounds.position = {
    //    _transform->position.x - bounds.size.x / 2.f,
    //    _transform->position.y - bounds.size.y / 2.f
    //};

    //float padding = 10.f;
    //bounds.position.x -= padding;
    //bounds.position.y -= padding;
    //bounds.size.x += padding * 2;
    //bounds.size.y += padding * 2;

    //sf::RectangleShape debugRect;
    //debugRect.setPosition(bounds.position);
    //debugRect.setSize(bounds.size);
    //debugRect.setFillColor(sf::Color(0, 255, 0, 50)); // Verde transparente

    //window->draw(debugRect);
}

void ClickableComponent::OnGlobalClick(sf::Vector2f pos)
{
}
