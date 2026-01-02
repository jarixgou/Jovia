#include "Player.hpp"

#include "Engine/Asset/AssetsManager.hpp"

Player::Player()
{
	sf::Sprite* sprite = new sf::Sprite(*Engine::AssetsManager::Get<sf::Texture>("Monstre"));

	Engine::Object* obj = new Engine::Object();
	obj->shape = sprite;

	Engine::Transform transform;
	transform.position = { 0.f, 0.f, -2.f };
	transform.rotation = { 0.f, 0.f, 0.f };
	transform.size = { 32.f, 32.f, 0.f };
	transform.scale = { 1.f, 1.f };

	SetObject(&obj);
	SetTransform(transform);
	SetTag("Player");
}

void Player::Update()
{

}

void Player::Display()
{
	GameObject::Display();
}