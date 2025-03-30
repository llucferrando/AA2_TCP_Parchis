#include "Transform.h"

Transform::Transform()
{

}

void Transform::Translate(const sf::Vector2f& offset)
{
    position += offset;
}
void Transform::Rotate(sf::Angle angle)
{
    rotation += angle;
}

const std::type_index Transform::GetType()
{
    return typeid(Transform);
}