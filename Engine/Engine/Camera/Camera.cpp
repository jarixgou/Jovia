#include "Camera.hpp"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../GameObject/GameObject.hpp"
#include "../Transform/Transform.hpp"

#include "../Logger/Logger.hpp"
#include "../Math/Math.hpp"
#include "../Render/RenderAPI.hpp"

namespace Engine
{
	void Camera::Update(float _dt)
	{
		if (m_free)
		{
			float speed = 20.f * _dt;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			{
				m_pos.z += 1.f * _dt;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			{
				m_pos.z += -1.f * _dt;
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

		if (m_angle.x > 359.f)
		{
			m_angle.x = 0.f;
		}
		else if (m_angle.x < 0.f)
		{
			m_angle.x = 359.f;
		}

		if (m_angle.y > 359.f)
		{
			m_angle.y = 0.f;
		}
		else if (m_angle.y < 0.f)
		{
			m_angle.y = 359.f;
		}

		if (m_angle.z > 359.f)
		{
			m_angle.z = 0.f;
		}
		else if (m_angle.z < 0.f)
		{
			m_angle.z = 359.f;
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

	void Camera::SetAngle(const sf::Vector3f& _angle)
	{
		m_angle = _angle;
	}

	void Camera::SetType(CameraType _type)
	{
		m_type = _type;
	}

	sf::FloatRect Camera::GetVisibleArea(sf::Vector2f _tileSize) const
	{
		const float scale = 1.0f / -m_pos.z;

		if (m_type == CameraType::ORTHOGRAPHIC)
		{
			const float w = m_size.x / (scale * _tileSize.x);
			const float h = m_size.y / (scale * _tileSize.y);

			return { m_pos.x - w / 2.0f, m_pos.y - h / 2.0f, w, h };
		}
		else if (m_type == CameraType::ISOMETRIC)
		{
			// Transformation inverse isométrique pour trouver les bounds monde
			const float halfWidth = m_size.x * 0.5f;
			const float halfHeight = m_size.y * 0.5f;

			// Les 4 coins de l'écran en coordonnées écran (relatif au centre)
			const sf::Vector2f screenCorners[4] = {
				{ -halfWidth, -halfHeight },  // Haut-gauche
				{  halfWidth, -halfHeight },  // Haut-droite
				{ -halfWidth,  halfHeight },  // Bas-gauche
				{  halfWidth,  halfHeight }   // Bas-droite
			};

			float minX = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::lowest();
			float minY = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::lowest();

			// Récupérer la matrice de rotation et calculer son inverse (transposée pour une rotation orthonormée)
			Math::Mat3x3 r = Math::CreateRotationMatrix(m_angle.x, m_angle.y, m_angle.z);
			Math::Mat3x3 rInv;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					rInv[i][j] = r[j][i];
				}
			}

			// Pour chaque coin de l'écran, calculer la position monde correspondante
			for (const auto& screenPos : screenCorners)
			{
				// Transformation inverse isométrique (sur le plan z=0)
				// Dans WorldToScreen :
				// iso.x = (rotX - rotY) * ((_tileSize.x * scale) * 0.5f);
				// iso.y = (rotX + rotY - rotZ) * ((_tileSize.y * scale) * 0.25f);
				//
				// On considère rotZ = 0 pour la limite du sol (tiles), puis on résout rotX/rotY :
				const float a = screenPos.x / (_tileSize.x * scale * 0.5f);  // rotX - rotY
				const float b = screenPos.y / (_tileSize.y * scale * 0.25f); // rotX + rotY

				const float rotX = (a + b) * 0.5f;
				const float rotY = (b - a) * 0.5f;

				// Appliquer l'inverse de la rotation pour récupérer les coordonnées monde
				sf::Vector3f rotated = { rotX, rotY, 0.0f };
				sf::Vector3f world = Math::MultiplyMat3x3Vector(rInv, rotated);

				minX = std::min(minX, world.x);
				maxX = std::max(maxX, world.x);
				minY = std::min(minY, world.y);
				maxY = std::max(maxY, world.y);
			}

			// Ajouter la position de la caméra et une marge
			constexpr float margin = 1.7f;
			return {
				m_pos.x + minX - margin,
				m_pos.y + minY - margin,
				(maxX - minX) + margin * 2.0f,
				(maxY - minY) + margin * 2.0f
			};
		}

		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	sf::Vector3f Camera::GetPos() const
	{
		return m_pos;
	}

	sf::Vector2f Camera::GetSize() const
	{
		return m_size;
	}

	bool Camera::GetFree() const
	{
		return m_free;
	}

	const sf::Vector3f& Camera::GetAngle() const
	{
		return m_angle;
	}

	CameraType Camera::GetType() const
	{
		return m_type;
	}

	void Camera::DrawObject(GameObject* _gameObject, sf::RenderTarget& _renderTarget) const
	{
		Object* object = _gameObject->GetShape();

		if (object != nullptr && object->shape != nullptr)
		{
			TransformObject(object->shape, _gameObject->GetTransform());

			_renderTarget.draw(*object->shape, object->renderStates);
		}
	}

	sf::Vector2f Camera::WorldToScreen(const Transform& _transform) const
	{
		sf::Vector2f screenPos = { 0,0 };

		const sf::Vector2f cameraMiddlePoint = { m_size.x * 0.5f, m_size.y * 0.5f };
		const sf::Vector3f relativePos = {
			_transform.position.x - m_pos.x,
			_transform.position.y - m_pos.y,
			_transform.position.z - m_pos.z
		};

		const float scale = (1.0f / (_transform.position.z - m_pos.z));
		const sf::Vector2f finalScale = {_transform.scale.x * scale, _transform.scale.y * scale };
		sf::Vector2f objectScaled = sf::Vector2f(_transform.size.x * finalScale.x, _transform.size.y * finalScale.y);

		if (m_type == CameraType::ORTHOGRAPHIC)
		{
			Math::Mat3x3 r = Math::CreateRotationMatrix(m_angle.x, m_angle.y, m_angle.z);
			sf::Vector3f rotatedPos = Math::MultiplyMat3x3Vector(r, relativePos);

			const sf::Vector2f orthoPos = {
				(rotatedPos.x * objectScaled.x),
				(rotatedPos.y * objectScaled.y)
			};

			screenPos = {
				orthoPos.x + cameraMiddlePoint.x - finalScale.x,
				orthoPos.y + cameraMiddlePoint.y - finalScale.y
			};
		}
		else if (m_type == CameraType::ISOMETRIC)
		{
			Math::Mat3x3 r = Math::CreateRotationMatrix(m_angle.x, m_angle.y, m_angle.z);
		
			Math::Mat3x3 isoMatrix = Math::CreateIsoMatrix(objectScaled);
			Math::Mat3x3 finalMatrix = Math::MultiplyMat3x3(r, isoMatrix);
			sf::Vector3f pos = Math::MultiplyMat3x3Vector(finalMatrix, relativePos);

			screenPos = {
				pos.x + cameraMiddlePoint.x,
				pos.y + cameraMiddlePoint.y
			};
		}

		return screenPos;
	}

	void Camera::TransformObject(sf::Drawable* _object, const Transform& _transform) const
	{
		sf::Vector2f pos = WorldToScreen(_transform);

		const float scale = 1.f / (_transform.position.z - m_pos.z);
		const sf::Vector2f finalScale = {_transform.scale.x * scale, _transform.scale.y * scale };

		Math::Mat3x3 r = Math::CreateRotationMatrix(m_angle.x, m_angle.y, m_angle.z);
		sf::Vector3f dirWorld = Math::MultiplyMat3x3Vector(r, sf::Vector3f{ 1.f, 0.f, 0.f });

		const float rotation = Math::RadToDeg(atan2f(dirWorld.y, dirWorld.x));

		auto sprite = dynamic_cast<sf::Sprite*>(_object);
		auto shape = dynamic_cast<sf::Shape*>(_object);
		auto vertexArray = dynamic_cast<sf::VertexArray*>(_object);
		if (sprite != nullptr)
		{
			sprite->setScale(finalScale);
			sprite->setPosition(pos);
			sprite->setRotation(rotation);
		}
		else if (shape != nullptr)
		{
			shape->setScale(finalScale);
			shape->setPosition(pos);
			shape->setRotation(rotation);
		}
		else if (vertexArray != nullptr)
		{
			// TODO: need to support vertex array for transform
		}
	}
}
