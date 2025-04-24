#include "LoginMenu.h"
#include <iostream>

LoginMenu::LoginMenu()
    : _backgroundGO(nullptr), _nicknameField(nullptr), _passwordField(nullptr),
    _loginButton(nullptr), _registerButton(nullptr)
{
}

LoginMenu::~LoginMenu()
{
    delete _backgroundGO;
    delete _nicknameField;
    delete _passwordField;
    delete _loginButton;
    delete _registerButton;
}

void LoginMenu::Init()
{
    // Background GameObject con SpriteRenderer
    _backgroundGO = new GameObject();
    _backgroundGO->AddComponent<SpriteRenderer>("Assets/Splashscreen/login.png");
    _backgroundGO->GetComponent<Transform>()->position = sf::Vector2f(0.f, 0.f);
    _backgroundGO->GetComponent<Transform>()->scale = sf::Vector2f(1.f, 1.f);

    // Campos de texto
    _nicknameField = new TextField(sf::Vector2f(200, 300), sf::Vector2f(400, 50), "Nickname");
    _passwordField = new TextField(sf::Vector2f(200, 370), sf::Vector2f(400, 50), "Password", true);

    // Botones
    _loginButton = new Button(sf::Vector2f(200, 450), sf::Vector2f(180, 50), "Login");
    _registerButton = new Button(sf::Vector2f(420, 450), sf::Vector2f(180, 50), "Register");
}

void LoginMenu::HandleEvent(const sf::Event& event)
{
    _nicknameField->HandleEvent(event);
    _passwordField->HandleEvent(event);
    _loginButton->HandleEvent(event);
    _registerButton->HandleEvent(event);
}

void LoginMenu::Update(float deltaTime)
{
    _nicknameField->Update(deltaTime);
    _passwordField->Update(deltaTime);
    _loginButton->Update(deltaTime);
    _registerButton->Update(deltaTime);
}

void LoginMenu::Render(sf::RenderWindow* window)
{
    _backgroundGO->GetComponent<SpriteRenderer>()->Draw(window, _backgroundGO->GetComponent<Transform>());
    _nicknameField->Draw(*window);
    _passwordField->Draw(*window);
    _loginButton->Draw(*window);
    _registerButton->Draw(*window);
}
