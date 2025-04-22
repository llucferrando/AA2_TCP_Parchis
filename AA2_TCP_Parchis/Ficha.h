#pragma once
#include "GameObject.h"

class Ficha : GameObject
{
public:
	Ficha();
	~Ficha();

	void Update(float deltaTime);
	void Draw();

private:
	void Move(int numPos);
	void CheckNextPosition();
};

