#pragma once
#include "Component.h"
#include "Vector2.h"

class Transform : public Component
{
public:
	Transform();

	Vector2 position;
	float rotation;
	Vector2 scale;

	const std::type_index GetType() override;

	void Translate(const Vector2& offset) {
		position += offset;
	}

	void Rotate(float angle) {
		rotation += angle;
	}
};

