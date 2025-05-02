#pragma once
#include "Menu.h"
#include "ButtonComponent.h"
#include "TextFieldComponent.h"
#include "SpriteRenderer.h"
#include "EventHandler.h"
#include "Client.h"

class GameOverMenu : public Menu
{
public:
	GameOverMenu(EventHandler* eventHandler, Client* client);
    void Update(float deltaTime);
    void Render(sf::RenderWindow* window);
    ButtonComponent* GetExitToRoomButton();
    ButtonComponent* GetExitGameButton();
    Event<> onExitGame;
    Event<> onReturnMenu;

private:
    void SubcribeToButtons();
    ButtonComponent* _exitRoomMenuButton;
    ButtonComponent* _exitGameButton;
    Client* _client;
};

