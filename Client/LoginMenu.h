#pragma once
#include "Menu.h"
#include "ButtonComponent.h"
#include "TextFieldComponent.h"
#include "SpriteRenderer.h"
#include "EventHandler.h"

// -- Acts like a unity prefab?

class LoginMenu : public Menu
{
public:
    LoginMenu(EventHandler* eventHandler);

    void Update(float deltaTime);
    void Render(sf::RenderWindow* window);
    ButtonComponent* GetLoginButton();
    std::string GetUsernameText();
    std::string GetPaswwordText();

private:
    TextFieldComponent* _usernameField;
    TextFieldComponent* _passwordField;
    ButtonComponent* _loginButton;
    ButtonComponent* _registerButton;

};

