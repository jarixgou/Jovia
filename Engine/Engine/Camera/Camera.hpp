#ifndef CAMERA__HPP
#define CAMERA__HPP
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Engine
{
	enum class CameraType
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	class Camera
	{
	private:
		sf::Vector3f pos = { 0,0,0 };
		float angle = 0.f;
	public:
		void SetCameraPos(sf::Vector3f _pos);

		void DrawObjectInCamera(sf::Sprite& _object, sf::Vector3f _pos, sf::RenderWindow& _window);
		void DrawObjectInCamera(sf::RectangleShape& _object, sf::Vector3f _pos, sf::RenderWindow& _window);
		void DrawObjectInCamera(sf::CircleShape& _object, sf::Vector3f _pos, sf::RenderWindow& _window);
	};
}

#endif