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
		ISOMETRIC
	};

	class Camera
	{
	private:
		sf::Vector3f m_pos = { 0,0,0 };
		sf::Vector2f m_size = { 1920,1080 };
		float m_zoom = 0;
		float m_angle = 0.f;

		CameraType type = CameraType::ORTHOGRAPHIC;
	public:
		void SetPos(sf::Vector3f _pos);
		void SetSize(sf::Vector2f _size);
		void SetZoom(float _zoom);
		void SetAngle(float _angle);
		void SetType(CameraType _type);

		sf::FloatRect GetVisibleArea();

		void DrawObject(sf::Sprite& _object, sf::Vector3f _pos, sf::Vector2f _size, sf::RenderWindow& _window);
		void DrawObject(sf::RectangleShape& _object, sf::Vector3f _pos, sf::Vector2f _size, sf::RenderWindow& _window);
		void DrawObject(sf::CircleShape& _object, sf::Vector3f _pos, sf::Vector2f _size, sf::RenderWindow& _window);
	private:
		sf::Vector2f WorldToScreen(sf::Vector3f _objectPos, sf::Vector2f _objectSize);

	};
}

#endif