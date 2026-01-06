#include "Camera.hpp"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../GameObject/GameObject.hpp"
#include "../Transform/Transform.hpp"

#include "../Logger/Logger.hpp"
#include "../Render/RenderAPI.hpp"
#include "../System/System.hpp"
#include "../Light/Light.hpp"

namespace Engine
{
	Camera::Camera(const Transform& _transform, CameraType _type, float _renderDistance) : 
		GameObject(_transform), m_free(false), m_target({ 0,0,0 }), m_type(_type), m_hasMoved(false), m_renderDistance(_renderDistance)
	{
		m_lastTransform = new Transform();

		m_rotationMatrix = Math::CreateRotationMatrix(_transform.rotation);

		SetTag("Camera");
	}

	Camera::~Camera()
	{
		delete m_lastTransform;
	}

	void Camera::Update()
	{
		Transform currentTransform = GetTransform();
		sf::Vector3f& currentPos = currentTransform.position;
		sf::Vector3f& currentRot = currentTransform.rotation;

		if (m_free)
		{
			float speed = 20.f * System::time.GetDeltaTime();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			{
				currentPos.z += 1.f * System::time.GetDeltaTime();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			{
				currentPos.z += -1.f * System::time.GetDeltaTime();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
			{
				if (m_type == CameraType::ORTHOGONAL)
				{
					currentPos.y += -speed;
				}
				else if (m_type == CameraType::ISOMETRIC)
				{
					currentPos.x -= speed;
					currentPos.y -= speed;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				if (m_type == CameraType::ORTHOGONAL)
				{
					currentPos.y += speed;
				}
				if (m_type == CameraType::ISOMETRIC)
				{
					currentPos.x += speed;
					currentPos.y += speed;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
			{
				if (m_type == CameraType::ORTHOGONAL)
				{
					currentPos.x += -speed;
				}
				if (m_type == CameraType::ISOMETRIC)
				{
					currentPos.x -= speed;
					currentPos.y += speed;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				if (m_type == CameraType::ORTHOGONAL)
				{
					currentPos.x += speed;
				}
				if (m_type == CameraType::ISOMETRIC)
				{
					currentPos.x += speed;
					currentPos.y -= speed;
				}
			}
		}

		if (currentRot.x > 359.f)
		{
			currentRot.x = 0.f;
		}
		else if (currentRot.x < 0.f)
		{
			currentRot.x = 359.f;
		}

		if (currentRot.y > 359.f)
		{
			currentRot.y = 0.f;
		}
		else if (currentRot.y < 0.f)
		{
			currentRot.y = 359.f;
		}

		if (currentRot.z > 359.f)
		{
			currentRot.z = 0.f;
		}
		else if (currentRot.z < 0.f)
		{
			currentRot.z = 359.f;
		}

		if (currentTransform != *m_lastTransform)
		{
			if (currentTransform.rotation != m_lastTransform->rotation)
			{
				m_rotationMatrix = Math::CreateRotationMatrix(currentTransform.rotation);
			}

			m_hasMoved = true;

			*m_lastTransform = currentTransform;
		}
		else
		{
			m_hasMoved = false;
		}

		SetTransform(currentTransform);
	}

	void Camera::SetFree(bool _free)
	{
		m_free = _free;
	}

	void Camera::SetFollow(sf::Vector3f _target)
	{
		m_target = _target;
	}

	void Camera::SetType(CameraType _type)
	{
		m_type = _type;
	}

	void Camera::SetRenderDistance(const float& _renderDistance)
	{
		m_renderDistance = _renderDistance;
	}

	const float& Camera::GetRenderDistance() const
	{
		return m_renderDistance;
	}

	const bool& Camera::GetHasMoved() const
	{
		return m_hasMoved;
	}

	sf::FloatRect Camera::GetVisibleArea(const sf::Vector2f& _tileSize, const float& _height) const
	{
		Transform currentTransform = GetTransform();
		sf::Vector3f& currentPos = currentTransform.position;
		sf::Vector3f& currentSize = currentTransform.size;

		const float scale = (1.0f / (_height - currentPos.z));

		if (m_type == CameraType::ORTHOGONAL)
		{
			const float worldWidth = currentSize.x / (_tileSize.x * scale);
			const float worldHeight = currentSize.y / (_tileSize.y * scale);

			return {
				currentPos.x - worldWidth * 0.5f,
				currentPos.y - worldHeight * 0.5f,
				worldWidth,
				worldHeight
			};
		}
		else if (m_type == CameraType::ISOMETRIC)
		{
			// Transformation inverse isométrique pour trouver les bounds monde
			const float halfWidth = currentSize.x * 0.5f;
			const float halfHeight = currentSize.y * 0.5f;

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
			Math::Mat3x3 rInv = Math::InvertMatrix(m_rotationMatrix);

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
				sf::Vector3f world = Math::MultiplyMatrixVector(rInv, rotated);

				minX = std::min(minX, world.x);
				maxX = std::max(maxX, world.x);
				minY = std::min(minY, world.y);
				maxY = std::max(maxY, world.y);
			}

			// Ajouter la position de la caméra et une marge
			constexpr float margin = 1.7f;
			return {
				currentPos.x + minX - margin,
				currentPos.y + minY - margin,
				(maxX - minX) + margin * 2.0f,
				(maxY - minY) + margin * 2.0f
			};
		}

		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	bool Camera::GetFree() const
	{
		return m_free;
	}

	CameraType Camera::GetType() const
	{
		return m_type;
	}

	void Camera::DrawObject(GameObject* _gameObject, sf::RenderTarget& _renderTarget) const
	{
		float zValue = _gameObject->GetTransform().position.z - GetTransform().position.z;

		if (zValue >= 2.220446e-16 && zValue <= m_renderDistance)
		{
			System::drawCall += 1;
			Object* object = _gameObject->GetShape();

			if (object != nullptr && object->shape != nullptr)
			{
				TransformObject(object->shape, _gameObject->GetTransform());

				_renderTarget.draw(*object->shape, object->renderStates);
			}
		}
	}

	void Camera::DrawLight(GameObject* _gameObject, sf::RenderTarget& _renderTarget) const
	{
		Light* light = dynamic_cast<Light*>(_gameObject);
		if (light == nullptr)
		{
			LOG_WARNING("Change the tag of the object it's not actually a light", true);
			return;
		}


	}

	std::pair<sf::Vector2f, sf::Vector2f> Camera::WorldToScreen(const Transform& _transform) const
	{
		Transform currentTransform = GetTransform();
		sf::Vector3f& currentPos = currentTransform.position;
		sf::Vector3f& currentSize = currentTransform.size;

		sf::Vector2f screenPos = { 0,0 };

		const sf::Vector2f cameraMiddlePoint = { currentSize.x * 0.5f, currentSize.y * 0.5f };
		const sf::Vector3f relativePos = {
			_transform.position.x - currentPos.x,
			_transform.position.y - currentPos.y,
			_transform.position.z - currentPos.z
		};

		const float scale = (1.0f / (_transform.position.z - currentPos.z));
		const sf::Vector2f finalScale = { _transform.scale.x * scale, _transform.scale.y * scale };
		sf::Vector2f objectScaled = sf::Vector2f(_transform.size.x * finalScale.x, _transform.size.y * finalScale.y);

		if (m_type == CameraType::ORTHOGONAL)
		{
			sf::Vector3f rotatedPos = Math::MultiplyMatrixVector(m_rotationMatrix, relativePos);

			const sf::Vector2f orthoPos = {
				(rotatedPos.x * objectScaled.x),
				(rotatedPos.y * objectScaled.y)
			};

			screenPos = {
				orthoPos.x + cameraMiddlePoint.x,
				orthoPos.y + cameraMiddlePoint.y
			};
		}
		else if (m_type == CameraType::ISOMETRIC)
		{
			Math::Mat3x3 isoMatrix = Math::CreateIsoMatrix(objectScaled);
			Math::Mat3x3 finalMatrix = Math::MultiplyMatrix(m_rotationMatrix, isoMatrix);
			sf::Vector3f pos = Math::MultiplyMatrixVector(finalMatrix, relativePos);

			screenPos = {
				pos.x + cameraMiddlePoint.x,
				pos.y + cameraMiddlePoint.y
			};
		}

		return { screenPos, finalScale };
	}

	void Camera::TransformObject(sf::Drawable* _object, const Transform& _transform) const
	{
		auto [pos, scale] = WorldToScreen(_transform);

		sf::Vector3f dirWorld = Math::MultiplyMatrixVector(m_rotationMatrix, sf::Vector3f{ 1.f, 0.f, 0.f });

		const float angle = Math::RadToDeg(std::atan2(dirWorld.y, dirWorld.x)) + _transform.rotation.z;

		float rotXRad = Math::DegToRad(_transform.rotation.x);
		float rotYRad = Math::DegToRad(_transform.rotation.y);

		float scaleFactorX = std::abs(std::cos(rotYRad)); 
		float scaleFactorY = std::abs(std::cos(rotXRad));

		scale = { scale.x * scaleFactorX, scale.y * scaleFactorY };

		auto sprite = dynamic_cast<sf::Sprite*>(_object);
		auto shape = dynamic_cast<sf::Shape*>(_object);
		auto vertexArray = dynamic_cast<sf::VertexArray*>(_object);

		if (sprite != nullptr)
		{
			System::verticeNb += 4;
			sprite->setScale(scale);
			sprite->setPosition(pos);
			sprite->setRotation(angle);
		}
		else if (shape != nullptr)
		{
			System::verticeNb += shape->getPointCount();
			shape->setScale(scale);
			shape->setPosition(pos);
			shape->setRotation(angle);
		}
		else if (vertexArray != nullptr)
		{
			System::verticeNb += vertexArray->getVertexCount();
			//const Math::Mat2x2 rotationMatrix = Math::CreateRotationMatrix(angle);
			//const size_t vertexCount = vertexArray->getVertexCount();

			///*#pragma omp parallel for schedule(static) if(vertexCount > 1000)*/
			//for (int i = 0; i < static_cast<int>(vertexCount); i++)
			//{
			//	sf::Vertex& vertex = (*vertexArray)[i];

			//	sf::Vector2f rotated = Math::MultiplyMatrixVector(rotationMatrix, vertex.position);

			//	rotated.x *= scale.x;
			//	rotated.y *= scale.y;

			//	vertex.position = pos + rotated;
			//}
		}
	}
}