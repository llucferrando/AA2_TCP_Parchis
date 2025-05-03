#include "MatchmakingMenu.h"
#include <iostream>

MatchmakingMenu::MatchmakingMenu(EventHandler* eventHandler, Client* client)
{
	AddComponent<SpriteRenderer>("Assets/Splashscreen/matchmaking.png", sf::Color::White);

	_idCreateRoomField = AddComponent<TextFieldComponent>(sf::Vector2f(160, 300), sf::Vector2f(400, 50), "ID create room", eventHandler);

	_idJoinRoomField = AddComponent<TextFieldComponent>(sf::Vector2f(160, 450), sf::Vector2f(400, 50), "ID join room", eventHandler);

	_createRoomButton = AddComponent<ButtonComponent>(sf::Vector2f(170, 370), sf::Vector2f(180, 50), "Create", eventHandler);

	_joinRoomButton = AddComponent<ButtonComponent>(sf::Vector2f(170, 520), sf::Vector2f(180, 50), "Join", eventHandler);

	_client = client;

	_createRoomButton->onClick.Subscribe([this]() {
		std::cout << "[Client] Creating a room..." << std::endl;

		if (_client->CreateRoom(_idCreateRoomField->GetText()))
		{
			std::cout << "[Client] Room created successfully, now listening for players..." << std::endl;
			_client->StartListeningForPeers();
		}
		else
		{
			std::cout << "[Client] Failed to create room." << std::endl;
		}
		});

	_joinRoomButton->onClick.Subscribe([this]() {
		std::cout << "[Client] Join room requested..." << std::endl;

		if (_client->JoinRoom(GetJoinIDText())) {
			_client->StartListeningForPeers();
			std::cout << "[Client] Join packet sent. Waiting for response..." << std::endl;
		}
		else {
			std::cout << "[Client] Failed to send join request." << std::endl;
		}
		});
}

void MatchmakingMenu::Update(float deltaTime)
{
	_idCreateRoomField->Update(deltaTime);
	_idJoinRoomField->Update(deltaTime);

	//Aqui tendria que hacer algo similar al servidor donde acepta y lee nuevos peers no?
	  // Escuchar nuevos peers entrantes (si este cliente es host)
	_client->UpdateP2PConnections();
	_client->HandleServerMessages(onStartMatch);
}

void MatchmakingMenu::Render(sf::RenderWindow* window)
{
	GetComponent<SpriteRenderer>()->Draw(window, GetComponent<Transform>());
	_idCreateRoomField->Render(window);
	_idJoinRoomField->Render(window); 
	_createRoomButton->Render(window);
	_joinRoomButton->Render(window);
}

ButtonComponent* MatchmakingMenu::GetCreateRoomButton() { return _createRoomButton; }

ButtonComponent* MatchmakingMenu::GetJoinRoomButton() { return _joinRoomButton; }

std::string MatchmakingMenu::GetJoinIDText() { return _idJoinRoomField->GetText(); }

std::string MatchmakingMenu::GetCreateIDText() { return _idCreateRoomField->GetText(); }
