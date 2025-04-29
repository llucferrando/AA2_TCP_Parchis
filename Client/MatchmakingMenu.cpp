#include "MatchmakingMenu.h"
#include <iostream>

MatchmakingMenu::MatchmakingMenu(EventHandler* eventHandler)
{
	AddComponent<SpriteRenderer>("Assets/Splashscreen/matchmaking.png", 10);

	_idCreateRoomField = AddComponent<TextFieldComponent>(sf::Vector2f(160, 300), sf::Vector2f(400, 50), "ID create room", eventHandler);

	_idJoinRoomField = AddComponent<TextFieldComponent>(sf::Vector2f(160, 450), sf::Vector2f(400, 50), "ID join room", eventHandler);

	_createButton = AddComponent<ButtonComponent>(sf::Vector2f(170, 370), sf::Vector2f(180, 50), "Create", eventHandler);

	_joinButton = AddComponent<ButtonComponent>(sf::Vector2f(170, 520), sf::Vector2f(180, 50), "Join", eventHandler);

	_createButton->onClick.Subscribe([this]() {
		std::cout << "Create Room Button pressed\n" << std::endl;
		});

	_joinButton->onClick.Subscribe([this]() {
		std::cout << "Join Room Button pressed\n" << std::endl;
		});
}

void MatchmakingMenu::Update(float deltaTime)
{
	_idCreateRoomField->Update(deltaTime);
	_idJoinRoomField->Update(deltaTime);
}

void MatchmakingMenu::Render(sf::RenderWindow* window)
{
	GetComponent<SpriteRenderer>()->Draw(window, GetComponent<Transform>());
	_idCreateRoomField->Render(window);
	_idJoinRoomField->Render(window); 
	_createButton->Render(window);
	_joinButton->Render(window);
}

ButtonComponent* MatchmakingMenu::GetCreateRoomButton()
{
	return _createButton;
}

ButtonComponent* MatchmakingMenu::GetJoinRoomButton()
{
	return _joinButton;
}

std::string MatchmakingMenu::GetJoinIDText()
{
	return _idJoinRoomField->GetText();
}

std::string MatchmakingMenu::GetCreateIDText()
{
	return _idCreateRoomField->GetText();
}
