#include "Chunk.hpp"

#include "../Transform/Transform.hpp"

#include "../Camera/Camera.hpp"
#include "../Math/Math.hpp"
#include "../System/System.hpp"

namespace Engine
{
	Chunk::Chunk(const Transform& _transform, sf::RenderStates _renderStates) :
		GameObject(_transform), m_tileId(chunkSize* chunkSize, { 0 }), m_groundVertices(sf::Quads),
		m_isDirty(true), m_isVisible(true)
	{
		Object* chunkObject = new Object();
		chunkObject->renderStates = _renderStates;
		chunkObject->shape = &m_groundVertices;
		SetObject(&chunkObject);
		SetTag("Chunk");
	}

	void Chunk::Update()
	{

	}

	void Chunk::Display()
	{
		GameObject::Display();
	}

	const sf::VertexArray& Chunk::GetGroundVertices() const
	{
		return m_groundVertices;
	}

	int Chunk::GetTile(const sf::Vector2i& _pos) const
	{
		return m_tileId[_pos.y * chunkSize + _pos.x];
	}

	bool Chunk::GetIsVisible() const
	{
		return m_isVisible;
	}

	bool Chunk::GetIsDirty() const
	{
		return m_isDirty;
	}

	void Chunk::SetTile(const sf::Vector2i& _pos, const uint8_t& _tileId)
	{
		if (_pos.x >= 0 && _pos.x < chunkSize &&
			_pos.y >= 0 && _pos.y < chunkSize)
		{
			m_tileId[_pos.y * chunkSize + _pos.x] = _tileId;
		}
	}

	void Chunk::SetDirty(const bool& _dirty)
	{
		m_isDirty = _dirty;
	}

	void Chunk::SetVisible(const bool& _visible)
	{
		m_isVisible = _visible;
	}

	void Chunk::Build(const std::vector<sf::IntRect>& _textureRect)
	{
		m_groundVertices.clear();

		Transform camTransform = (*System::currentCamera)->GetTransform();
		Transform chunkTransform = GetTransform();

		for (int y = 0; y < chunkSize; ++y)
		{
			for (int x = 0; x < chunkSize; ++x)
			{
				uint8_t tileId = m_tileId[y * chunkSize + x];
				sf::IntRect rect = _textureRect[tileId];

				float worldX = static_cast<float>(chunkTransform.position.x + x);
				float worldY = static_cast<float>(chunkTransform.position.y + y);

				Transform tempTransform;
				tempTransform.position = { worldX, worldY, chunkTransform.position.z };
				tempTransform.size = { static_cast<float>(rect.width), static_cast<float>(rect.height), 0.f };
				tempTransform.scale = { 1.f ,1.f };
				tempTransform.rotation = { 0.f, 0.f, 0.f };
				auto [screenPos, scale] = (*System::currentCamera)->WorldToScreen(tempTransform);

				const float scaledWidth = static_cast<float>(rect.width) * scale.x;
				const float scaledHeight = static_cast<float>(rect.height) * scale.y;

				sf::VertexArray& targetVertices = m_groundVertices;

				Math::Mat3x3 r = Math::CreateRotationMatrix(camTransform.rotation);
				sf::Vector3f dirWorld = Math::MultiplyMatrixVector(r, sf::Vector3f{ 1.f, 0.f, 0.f });

				const float angle = Math::RadToDeg(atan2f(dirWorld.y, dirWorld.x));
				Math::Mat2x2 rMatrix = Math::CreateRotationMatrix(angle);

				sf::Vector2f p1 = { scaledWidth, 0.f };
				sf::Vector2f p2 = { scaledWidth, scaledHeight };
				sf::Vector2f p3 = { 0.f, scaledHeight };

				// Ajout des 4 vertices
				targetVertices.append(sf::Vertex(screenPos, sf::Color::White,
					{ static_cast<float>(rect.left), static_cast<float>(rect.top) }));

				targetVertices.append(sf::Vertex(screenPos + Math::MultiplyMatrixVector(rMatrix, p1), sf::Color::White,
					{ static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top) }));

				targetVertices.append(sf::Vertex(screenPos + Math::MultiplyMatrixVector(rMatrix, p2), sf::Color::White,
					{ static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top + rect.height) }));

				targetVertices.append(sf::Vertex(screenPos + Math::MultiplyMatrixVector(rMatrix, p3), sf::Color::White,
					{ static_cast<float>(rect.left), static_cast<float>(rect.top + rect.height) }));
			}
		}
		m_isDirty = false;
	}

	void Chunk::Cleanup()
	{
		m_tileId.clear();
		m_groundVertices.clear();
	}
}
