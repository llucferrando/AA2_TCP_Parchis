#include "LoginMenu.h"
#include <iostream>

LoginMenu::LoginMenu(EventHandler* eventHandler) {

    AddComponent<SpriteRenderer>("Assets/Splashscreen/login.png", 10);

    _usernameField = AddComponent<TextFieldComponent>(sf::Vector2f(160, 300), sf::Vector2f(400, 50), "Username", eventHandler);

    _passwordField = AddComponent<TextFieldComponent>(sf::Vector2f(160, 370), sf::Vector2f(400, 50), "Password", eventHandler);

    _loginButton = AddComponent<ButtonComponent>(sf::Vector2f(170, 450), sf::Vector2f(180, 50), "Login", eventHandler);

    _registerButton = AddComponent<ButtonComponent>(sf::Vector2f(370, 450), sf::Vector2f(180, 50), "Register", eventHandler);

    _loginButton->onClick.Subscribe([this]() {

        std::cout << "Login button pressed\n";
        // Send Package to server to login call
        });

    _registerButton->onClick.Subscribe([this]() {
        std::cout << "Register button pressed\n";
        //Send package to register
        });
}

void LoginMenu::Update(float deltaTime)
{
    _usernameField->Update(deltaTime);
    _passwordField->Update(deltaTime);
}

void LoginMenu::Render(sf::RenderWindow* window)
{
    GetComponent<SpriteRenderer>()->Draw(window, GetComponent<Transform>());
    _usernameField->Render(window);
    _passwordField->Render(window);
    _loginButton->Render(window);
    _registerButton->Render(window);
}

ButtonComponent* LoginMenu::GetLoginButton()
{
    return _loginButton;
}

ButtonComponent* LoginMenu::GetRegisterButton()
{
    return _registerButton;
}

std::string LoginMenu::GetUsernameText()
{
    return _usernameField->GetText();
}

std::string LoginMenu::GetPaswwordText()
{
    return _passwordField->GetText();
}
