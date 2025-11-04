#include "Menu.hpp"

#include "Engine/Asset/AssetsManager.hpp"
#include "Engine/Interface/Assets/AssetsInterface.hpp"

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::Init()
{
	Engine::AssetsManager::Add<sf::Texture>("Assets/Cell.png");

	const sf::Texture* cellTexture = Engine::AssetsManager::Get<sf::Texture>("Cell");
}

void Menu::PollEvents(sf::RenderWindow& _window, sf::Event& _event)
{
}

void Menu::Update(sf::RenderWindow& _renderWindow, float _dt)
{

}

void Menu::Display(sf::RenderWindow& _window)
{

}

void Menu::Cleanup()
{
	Engine::AssetsManager::Clear();
}