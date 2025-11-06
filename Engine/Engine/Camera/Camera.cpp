#include "Camera.hpp"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace Engine
{
	void Camera::Update(float _dt)
	{
		if (m_free)
		{
			float speed = 20.f * _dt;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			{
				m_zoom += 1.f * _dt;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			{
				m_zoom += -1.f * _dt;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
			{
				if (m_type == CameraType::ORTHOGRAPHIC)
				{
					m_pos.y += -speed;
				}
				else if (m_type == CameraType::ISOMETRIC)
				{
					m_pos.x -= speed;
					m_pos.y -= speed;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				if (m_type == CameraType::ORTHOGRAPHIC)
				{
					m_pos.y += speed;
				}
				if (m_type == CameraType::ISOMETRIC)
				{
					m_pos.x += speed;
					m_pos.y += speed;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
			{
				if (m_type == CameraType::ORTHOGRAPHIC)
				{
					m_pos.x += -speed;
				}
				if (m_type == CameraType::ISOMETRIC)
				{
					m_pos.x -= speed;
					m_pos.y += speed;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				if (m_type == CameraType::ORTHOGRAPHIC)
				{
					m_pos.x += speed;
				}
				if (m_type == CameraType::ISOMETRIC)
				{
					m_pos.x += speed;
					m_pos.y -= speed;
				}
			}
		}

		if (m_zoom < 0)
		{
			m_zoom = 0;
		}
	}

	void Camera::SetFree(bool _free)
	{
		m_free = _free;
	}

	void Camera::SetFollow(sf::Vector3f _target)
	{
		m_target = _target;
	}

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
		m_type = _type;
	}

	sf::FloatRect Camera::GetVisibleArea()
	{
		float w = m_size.x / m_zoom;
		float h = m_size.y / m_zoom;

		return { m_pos.x - w / 2.0f, m_pos.y - h / 2.0f,
			w, h };
	}

	sf::Vector3f Camera::GetPos()
	{
		return m_pos;
	}

	sf::Vector2f Camera::GetSize()
	{
		return m_size;
	}

	bool Camera::GetFree()
	{
		return m_free;
	}

	float Camera::GetZoom()
	{
		return m_zoom;
	}

	float Camera::GetAngle()
	{
		return m_angle;
	}

	CameraType Camera::GetType()
	{
		return m_type;
	}

	void Camera::DrawObject(sf::Sprite& _object, sf::Vector3f _pos, sf::Vector2f _size, sf::RenderWindow& _window)
	{
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		float scaleX = (_size.x * m_zoom) / _size.x;
		float scaleY = (_size.y * m_zoom) / _size.y;

		_object.setScale(scaleX, scaleY);
		_object.setPosition(screenPos);
		_window.draw(_object);
	}

	void Camera::DrawObject(sf::RectangleShape& _object, sf::Vector3f _pos, sf::Vector2f _size, sf::RenderWindow& _window)
	{
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		float scaleX = (_size.x * m_zoom) / _size.x;
		float scaleY = (_size.y * m_zoom) / _size.y;

		_object.setScale(scaleX, scaleY);
		_object.setPosition(screenPos);
		_window.draw(_object);
	}

	void Camera::DrawObject(sf::CircleShape& _object, sf::Vector3f _pos, sf::Vector2f _size, sf::RenderWindow& _window)
	{
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		float scaleX = (_size.x * m_zoom) / _size.x;
		float scaleY = (_size.y * m_zoom) / _size.y;

		_object.setScale(scaleX, scaleY);
		_object.setPosition(screenPos);
		_window.draw(_object);
	}

	sf::Vector2f Camera::WorldToScreen(sf::Vector3f _objectPos, sf::Vector2f _objectSize)
	{
		sf::Vector2f screenPos = { 0,0 };

		sf::Vector2f cameraMiddlePoint = { m_size.x * 0.5f, m_size.y * 0.5f };
		sf::Vector3f relativePos = {
		_objectPos.x - m_pos.x,
		_objectPos.y - m_pos.y,
		_objectPos.z - m_pos.z
		};

		if (m_type == CameraType::ORTHOGRAPHIC)
		{
			screenPos = {
				(relativePos.x * m_zoom) / relativePos.z + cameraMiddlePoint.x,
				(relativePos.y * m_zoom) / relativePos.z + cameraMiddlePoint.y
			};
		}
		else if (m_type == CameraType::ISOMETRIC)
		{
			sf::Vector2f iso = { 0,0 };
			iso.x = (relativePos.x - relativePos.y) * ((_objectSize.x * m_zoom) * 0.5f);
			iso.y = (relativePos.x + relativePos.y - relativePos.z) * ((_objectSize.y * m_zoom) * 0.25f);

			screenPos = { iso.x + cameraMiddlePoint.x, iso.y + cameraMiddlePoint.y };
		}

		return screenPos;
	}
}
