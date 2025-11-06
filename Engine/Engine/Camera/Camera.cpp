#include "Camera.hpp"

namespace Engine
{


	void Camera::SetCameraPos(sf::Vector3f _pos)
	{
		pos = _pos;
	}

	void Camera::DrawObjectInCamera(sf::Sprite& _object, sf::Vector3f _pos, sf::RenderWindow& _window)
	{

	}

	void Camera::DrawObjectInCamera(sf::RectangleShape& _object, sf::Vector3f _pos, sf::RenderWindow& _window)
	{

	}

	void Camera::DrawObjectInCamera(sf::CircleShape& _object, sf::Vector3f _pos, sf::RenderWindow& _window)
	{

	}
}