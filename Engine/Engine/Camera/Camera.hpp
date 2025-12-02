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
	class GameObject;
	struct Transform;

	enum class CameraType : uint8_t
	{
		ORTHOGRAPHIC,
		ISOMETRIC
	};

	class Camera
	{
	private:
		sf::Vector3f m_pos = { 0,0,0 };
		sf::Vector2f m_size = { 1920,1080 };
		sf::Vector3f m_angle = { 0,0,0 };
		bool m_free = false;

		sf::Vector3f m_target = { 0,0,0 };

		CameraType m_type = CameraType::ORTHOGRAPHIC;
	public:
		Camera() = default;
		~Camera() = default;

		void Update(float _dt);

		void SetFree(bool _free = true);
		void SetFollow(sf::Vector3f _target);
		void SetPos(sf::Vector3f _pos);
		void SetSize(sf::Vector2f _size);
		void SetAngle(const sf::Vector3f& _angle);
		void SetType(CameraType _type);

		sf::FloatRect GetVisibleArea(sf::Vector2f _tileSize) const;
		sf::Vector3f GetPos() const;
		sf::Vector2f GetSize() const;
		bool GetFree() const;
		const sf::Vector3f& GetAngle() const;
		CameraType GetType() const;

		void DrawObject(GameObject* _gameObject, sf::RenderTarget& _renderTarget) const;

		sf::Vector2f WorldToScreen(const Transform& _transform) const;
	private: // Private function
		void TransformObject(sf::Drawable* _object, const Transform& _transform) const;
	};
}

#endif