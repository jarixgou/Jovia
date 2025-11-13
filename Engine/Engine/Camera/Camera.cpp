#include "Camera.hpp"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../DrawableObject/DrawableObject.hpp"

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

		if (m_angle > 359.f)
		{
			m_angle = 0.f;
		}
		else if (m_angle < 0.f)
		{
			m_angle = 359.f;
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

	sf::FloatRect Camera::GetVisibleArea(sf::Vector2f _tileSize)
	{
		if (m_type == CameraType::ORTHOGRAPHIC)
		{
			float w = m_size.x / (m_zoom * _tileSize.x);
			float h = m_size.y / (m_zoom * _tileSize.y);

			return { m_pos.x - w / 2.0f, m_pos.y - h / 2.0f, w, h };
		}
		else if (m_type == CameraType::ISOMETRIC)
		{
			// Transformation inverse isométrique pour trouver les bounds monde
			float halfWidth = m_size.x * 0.5f;
			float halfHeight = m_size.y * 0.5f;

			// Les 4 coins de l'écran en coordonnées écran (relatif au centre)
			sf::Vector2f screenCorners[4] = {
				{ -halfWidth, -halfHeight },  // Haut-gauche
				{  halfWidth, -halfHeight },  // Haut-droite
				{ -halfWidth,  halfHeight },  // Bas-gauche
				{  halfWidth,  halfHeight }   // Bas-droite
			};

			float minX = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::lowest();
			float minY = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::lowest();

			// Pour chaque coin de l'écran, calculer la position monde correspondante
			for (const auto& screenPos : screenCorners)
			{
				// Transformation inverse isométrique
				// Dans WorldToScreen :
				// screenX = (worldX - worldY) * (_tileSize.x * zoom) * 0.5
				// screenY = (worldX + worldY) * (_tileSize.y * zoom) * 0.25
				//
				// Inverse :
				// worldX - worldY = screenX / (_tileSize.x * zoom * 0.5)
				// worldX + worldY = screenY / (_tileSize.y * zoom * 0.25)
				//
				// Résolution :
				// worldX = [(screenX / (_tileSize.x * zoom * 0.5)) + (screenY / (_tileSize.y * zoom * 0.25))] / 2
				// worldY = [(screenY / (_tileSize.y * zoom * 0.25)) - (screenX / (_tileSize.x * zoom * 0.5))] / 2

				float a = screenPos.x / (_tileSize.x * m_zoom * 0.5f);  // worldX - worldY
				float b = screenPos.y / (_tileSize.y * m_zoom * 0.25f); // worldX + worldY

				float worldX = (a + b) * 0.5f;
				float worldY = (b - a) * 0.5f;

				minX = std::min(minX, worldX);
				maxX = std::max(maxX, worldX);
				minY = std::min(minY, worldY);
				maxY = std::max(maxY, worldY);
			}

			// Ajouter la position de la caméra et une marge
			float margin = 1.7f;
			return {
				m_pos.x + minX - margin,
				m_pos.y + minY - margin,
				(maxX - minX) + margin * 2.0f,
				(maxY - minY) + margin * 2.0f
			};
		}

		return { 0.0f, 0.0f, 0.0f, 0.0f };
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

	void Camera::DrawObject(sf::Sprite& _object, const sf::Vector3f& _pos, const sf::Vector2f& _size, sf::RenderWindow& _window)
	{
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		sf::Vector2f scale = { (_size.x * m_zoom) / _size.x,(_size.y * m_zoom) / _size.y };

		_object.setScale(scale.x, scale.y);
		_object.setPosition(screenPos);
		_object.setRotation(m_angle);
		_window.draw(_object);
	}

	void Camera::DrawObject(sf::RectangleShape& _object, const sf::Vector3f& _pos, const sf::Vector2f& _size, sf::RenderWindow& _window)
	{
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		sf::Vector2f scale = { (_size.x * m_zoom) / _size.x,(_size.y * m_zoom) / _size.y };

		_object.setScale(scale.x, scale.y);
		_object.setPosition(screenPos);
		_object.setRotation(m_angle);
		_window.draw(_object);
	}

	void Camera::DrawObject(sf::CircleShape& _object, const sf::Vector3f& _pos, const sf::Vector2f& _size, sf::RenderWindow& _window)
	{
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		sf::Vector2f scale = { (_size.x * m_zoom) / _size.x,(_size.y * m_zoom) / _size.y };

		_object.setScale(scale.x, scale.y);
		_object.setPosition(screenPos);
		_object.setRotation(m_angle);
		_window.draw(_object);
	}

	void Camera::DrawObject(DrawableObject& _object, const sf::Vector3f& _pos, const sf::Vector2f& _size, sf::RenderWindow& _window)
	{
		sf::Vector2f screenPos = WorldToScreen(_pos, _size);

		sf::Vector2f scale = { (_size.x * m_zoom) / _size.x,(_size.y * m_zoom) / _size.y };

		switch (_object.type)
		{
		case DrawableType::SPRITE:
			_object.sprite.setScale(scale.x, scale.y);
			_object.sprite.setPosition(screenPos);
			_object.sprite.setRotation(m_angle);

			_window.draw(_object.sprite, _object.states);
			break;
		case DrawableType::RECTANGLE:
			_object.rectangle.setScale(scale.x, scale.y);
			_object.rectangle.setPosition(screenPos);
			_object.rectangle.setRotation(m_angle);

			_window.draw(_object.sprite, _object.states);
			break;
		case DrawableType::CIRCLE:
			_object.circle.setScale(scale.x, scale.y);
			_object.circle.setPosition(screenPos);
			_object.circle.setRotation(m_angle);

			_window.draw(_object.sprite, _object.states);
			break;
		case DrawableType::SHAPE:
			_window.draw(_object.shape, _object.states);
			break;
		}
	}

	sf::Vector2f Camera::WorldToScreen(const sf::Vector3f& _objectPos, const sf::Vector2f& _objectSize)
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
			sf::Vector2f orthoPos = {
				(relativePos.x * m_zoom) / relativePos.z,
				(relativePos.y * m_zoom) / relativePos.z
			};

			sf::Vector2f rotatedPos = { 0,0 };
			float angleRad = m_angle * (3.14159265f / 180.f);
			rotatedPos.x = orthoPos.x * cos(angleRad) - orthoPos.y * sin(angleRad);
			rotatedPos.y = orthoPos.x * sin(angleRad) + orthoPos.y * cos(angleRad);

			screenPos = {
				rotatedPos.x + cameraMiddlePoint.x,
				rotatedPos.y + cameraMiddlePoint.y
			};
		}
		else if (m_type == CameraType::ISOMETRIC)
		{
			sf::Vector2f iso = { 0,0 };
			iso.x = (relativePos.x - relativePos.y) * ((_objectSize.x * m_zoom) * 0.5f);
			iso.y = (relativePos.x + relativePos.y - relativePos.z) * ((_objectSize.y * m_zoom) * 0.25f);

			sf::Vector2f rotatedPos = { 0,0 };
			float angleRad = m_angle * (3.14159265f / 180.f);
			rotatedPos.x = iso.x * cos(angleRad) - iso.y * sin(angleRad);
			rotatedPos.y = iso.x * sin(angleRad) + iso.y * cos(angleRad);

			screenPos = {
				rotatedPos.x + cameraMiddlePoint.x,
				rotatedPos.y + cameraMiddlePoint.y
			};
		}

		return screenPos;
	}
}
