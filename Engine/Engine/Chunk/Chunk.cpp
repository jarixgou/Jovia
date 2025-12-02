#include "Chunk.hpp"

#include <iostream>

#include "../Transform/Transform.hpp"

#include "../Camera/Camera.hpp"
#include "../Math/Math.hpp"

namespace Engine

{
	Chunk::Chunk(const sf::Vector2i& _chunkPos) :
		m_chunkPos(_chunkPos), m_tiles(chunkSize* chunkSize, { 0,0 }), m_groundVertices(sf::Quads),
		m_objectVertices(sf::Quads), m_isDirty(true), m_isVisible(true)
	{

	}

	const sf::Vector2i& Chunk::GetChunkPos() const
	{
		return m_chunkPos;
	}

	const sf::VertexArray& Chunk::GetGroundVertices() const
	{
		return m_groundVertices;
	}

	const sf::VertexArray& Chunk::GetObjectVertices() const
	{
		return m_objectVertices;
	}

	int Chunk::GetTile(const sf::Vector2i& _pos) const
	{
		return m_tiles[_pos.y * chunkSize + _pos.x].tileId;
	}

	bool Chunk::GetIsVisible() const
	{
		return m_isVisible;
	}

	bool Chunk::GetIsDirty() const
	{
		return m_isDirty;
	}

	void Chunk::SetTile(const sf::Vector2i& _pos, const uint8_t& _tileId, const float& _tileHeight)
	{
		if (_pos.x >= 0 && _pos.x < chunkSize &&
			_pos.y >= 0 && _pos.y < chunkSize)
		{
			m_tiles[_pos.y * chunkSize + _pos.x].tileId = _tileId;
			m_tiles[_pos.y * chunkSize + _pos.x].height = _tileHeight;
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

	void Chunk::Build(const std::vector<sf::IntRect>& _textureRect, const Camera* _cam)
	{
		m_groundVertices.clear();
		m_objectVertices.clear();

		const sf::Vector3f camPos = _cam->GetPos();
		const sf::Vector3f camAngle = _cam->GetAngle();

		for (int y = 0; y < chunkSize; ++y)
		{
			for (int x = 0; x < chunkSize; ++x)
			{
				TileData tileData = m_tiles[y * chunkSize + x];
				sf::IntRect rect = _textureRect[tileData.tileId];

				float worldX = static_cast<float>(m_chunkPos.x * chunkSize + x);
				float worldY = static_cast<float>(m_chunkPos.y * chunkSize + y);

				Transform tempTransform;
				tempTransform.position = {worldX, worldY, tileData.height};
				tempTransform.size = {static_cast<float>(rect.width), static_cast<float>(rect.height), 0.f};
				tempTransform.scale = {1.f ,1.f};
				tempTransform.angle = 0.f;
				sf::Vector2f screenPos = _cam->WorldToScreen(tempTransform);

				const float scale = 1.0f / (tileData.height - camPos.z);
				const float scaledWidth = static_cast<float>(rect.width) * scale;
				const float scaledHeight = static_cast<float>(rect.height) * scale;

				sf::VertexArray& targetVertices = (tileData.height < 0.5f) ? m_groundVertices : m_objectVertices;

				Math::Mat3x3 r = Math::CreateRotationMatrix(camAngle.x, camAngle.y, camAngle.z);
				sf::Vector3f dirWorld = Math::MultiplyMat3x3Vector(r, sf::Vector3f{ 1.f, 0.f, 0.f });

				float angleRad = atan2f(dirWorld.y, dirWorld.x);
				float c = cosf(angleRad);
				float s = sinf(angleRad);

				auto rot = [&](sf::Vector2f p) {
					return sf::Vector2f{
						p.x * c - p.y * s,
						p.x * s + p.y * c
					};
					};

				// Offsets avant rotation
				sf::Vector2f p0 = { 0.f, 0.f };
				sf::Vector2f p1 = { scaledWidth, 0.f };
				sf::Vector2f p2 = { scaledWidth, scaledHeight };
				sf::Vector2f p3 = { 0.f, scaledHeight };

				// Ajout des 4 vertices
				targetVertices.append(sf::Vertex(screenPos + rot(p0), sf::Color::White,
					{ static_cast<float>(rect.left), static_cast<float>(rect.top) }));

				targetVertices.append(sf::Vertex(screenPos + rot(p1), sf::Color::White,
					{ static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top) }));

				targetVertices.append(sf::Vertex(screenPos + rot(p2), sf::Color::White,
					{ static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top + rect.height) }));

				targetVertices.append(sf::Vertex(screenPos + rot(p3), sf::Color::White,
					{ static_cast<float>(rect.left), static_cast<float>(rect.top + rect.height) }));
			}
		}
		m_isDirty = false;
	}

	void Chunk::Clear()
	{
		m_tiles.clear();
		m_groundVertices.clear();
		m_objectVertices.clear();
	}
}
