#pragma once
#include "Menu.h"
#include "ButtonComponent.h"
#include "TextFieldComponent.h"
#include "SpriteRenderer.h"
#include "EventHandler.h"
#include "Client.h"

class GameOverMenu : public Menu
{
    GameOverMenu(EventHandler* eventHandler, Client* client);
    ~GameOverMenu();
    void Update(float deltaTime);
    void Render(sf::RenderWindow* window);
    ButtonComponent* GetExitToRoomButton();
    ButtonComponent* GetExitGameButton();

private:
    void SubcribeToButtons();
    ButtonComponent* _exitRoomMenuButton;
    ButtonComponent* _exitGameButton;
    Client* _client;

};

