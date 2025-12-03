#include "Player.hpp"

#include "Engine/Asset/AssetsManager.hpp"

void Player::Init()
{
	sf::Sprite sprite(*Engine::AssetsManager::Get<sf::Texture>("Monstre"));

	m_object = new Engine::Object();
	m_object->shape = new sf::Sprite(sprite);
	m_transform = Engine::Transform();
	m_transform.size = { 32,32, 0 };
}

void Player::Update(const float& _dt)
{

}

void Player::Display(const Engine::Camera* _camera)
{

	GameObject::Display(_camera);
}
