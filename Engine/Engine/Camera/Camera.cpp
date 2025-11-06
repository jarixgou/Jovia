#include "Camera.hpp"

namespace Engine
{


	void Camera::SetPos(sf::Vector3f _pos)
	{
		m_pos = _pos;
	}

	void Camera::SetSize(sf::Vector2f _size)
	{
		m_size = _size;
	}

	void Camera::SetZoom(float _zoom)
	{
		m_zoom = _zoom;
	}

	void Camera::SetAngle(float _angle)
	{
		m_angle = _angle;
	}

	void Camera::SetType(CameraType _type)
	{
		type = _type;
	}

	sf::FloatRect Camera::GetVisibleArea()
	{
		float w = m_size.x / m_zoom;
		float h = m_size.y / m_zoom;

		return { m_pos.x - w / 2.0f, m_pos.y - h / 2.0f,
			w, h };
	}

	void Camera::DrawObject(sf::Sprite& _object, sf::Vector3f _pos, sf::Vector2f _size, sf::RenderWindow& _window)
	{
		float scale = _size.x * m_zoom;
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		_object.setScale(scale, scale);
		_object.setPosition(screenPos);
		_window.draw(_object);
	}

	void Camera::DrawObject(sf::RectangleShape& _object, sf::Vector3f _pos, sf::Vector2f _size, sf::RenderWindow& _window)
	{
		float scale = _size.x * m_zoom;
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		_object.setScale(scale, scale);
		_object.setPosition(screenPos);
		_window.draw(_object);
	}

	void Camera::DrawObject(sf::CircleShape& _object, sf::Vector3f _pos, sf::Vector2f _size, sf::RenderWindow& _window)
	{
		float scale = _size.x * m_zoom;
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		_object.setScale(scale, scale);
		_object.setPosition(screenPos);
		_window.draw(_object);
	}

	sf::Vector2f Camera::WorldToScreen(sf::Vector3f _objectPos, sf::Vector2f _objectSize)
	{
		sf::Vector2f screenPos = { 0,0 };

		sf::Vector2f cameraMiddlePoint = { m_size.x * 0.5f, m_size.y * 0.5f };
		sf::Vector3f relativePos = { _objectPos.x * (_objectSize.x * m_zoom) - m_pos.x,
									 _objectPos.y * (_objectSize.y * m_zoom) - m_pos.y,
									 _objectPos.z - m_pos.z };


		if (type == CameraType::ORTHOGRAPHIC)
		{
			screenPos = {
				relativePos.x / relativePos.z + cameraMiddlePoint.x,
				relativePos.y / relativePos.z + cameraMiddlePoint.y
			};
		}
		else if (type == CameraType::ISOMETRIC)
		{
			sf::Vector2f iso = { 0,0 };
			iso.x = (relativePos.x - relativePos.y) * (_objectSize.x * 0.5f);
			iso.y = (relativePos.x + relativePos.y - relativePos.z) * (_objectSize.y * 0.25f);

			screenPos = {iso.x + cameraMiddlePoint.x, iso.y + cameraMiddlePoint.y};
		}

		return screenPos;
	}
}
