#include  "Game.hpp"

#include <Engine/Asset/AssetsManager.hpp>
#include <Engine/Scene/ScenesManager.hpp>

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init()
{
	Engine::AssetsManager::Add<sf::Texture>("Assets/spritesheet.png");

	const sf::Texture* spritesheetTexture = Engine::AssetsManager::Get<sf::Texture>("spritesheet");
	tileSheet.setTexture(*spritesheetTexture);

	textureSliced = Engine::SliceTexture(*spritesheetTexture, { 32,32 });
	std::string message = "Sliced texture into " + std::to_string(textureSliced.size()) + " sprites.";
	LOG_DEBUG(message.c_str());

	for (int y = 0; y < 50; ++y)
	{
		for (int x = 0; x < 50; ++x)
		{
			map[y][x] = rand() % textureSliced.size() + 1;
		}
	}
}

void Game::PollEvents(sf::RenderWindow& _window, sf::Event& _event)
{

}

void Game::Update(sf::RenderWindow& _renderWindow, float _dt)
{
	timer += _dt;

	if (timer >= 0.5f)
	{
		if (searchID < textureSliced.size())
		{
			for (auto& sliced : textureSliced)
			{
				if (sliced.id == searchID)
				{
					tileSheet.setTextureRect(sliced.rect);
					timer = 0.f;
				}
			}
			searchID++;
		}
		else
		{
			searchID = 0; // Optionnel : boucle l'animation
		}
	}
}

void Game::Display(sf::RenderWindow& _window)
{
	_window.draw(tileSheet);
}

void Game::Cleanup()
{
	Engine::AssetsManager::Clear();
}
