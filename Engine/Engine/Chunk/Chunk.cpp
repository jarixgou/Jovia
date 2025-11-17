#include "Chunk.hpp"

#include "../Camera/Camera.hpp"

namespace Engine

{
	Chunk::Chunk(const sf::Vector2i& _chunkPos) :
		m_chunkPos(_chunkPos), m_tiles(chunkSize* chunkSize, { 0,0 }), m_groundVertices(sf::Quads),
		m_objectVertices(sf::Quads), m_isDirty(true), m_isVisible(true)
	{

	}

	const sf::Vector2i& Chunk::GetChunkPos()
	{
		return m_chunkPos;
	}

	const sf::VertexArray& Chunk::GetGroundVertices()
	{
		return m_groundVertices;
	}

	const sf::VertexArray& Chunk::GetObjectVertices()
	{
		return m_objectVertices;
	}

	int Chunk::GetTile(const sf::Vector2i& _pos)
	{
		return m_tiles[_pos.y * chunkSize + _pos.x].tileId;
	}

	bool Chunk::GetIsVisible()
	{
		return m_isVisible;
	}

	bool Chunk::GetIsDirty()
	{
		return m_isDirty;
	}

	void Chunk::SetTile(const sf::Vector2i& _pos, int _tileId, float _tileHeight)
	{
		if (_pos.x >= 0 && _pos.x < chunkSize &&
			_pos.y >= 0 && _pos.y < chunkSize)
		{
			m_tiles[_pos.y * chunkSize + _pos.x].tileId = _tileId;
			m_tiles[_pos.y * chunkSize + _pos.x].height = _tileHeight;
		}
	}

	void Chunk::SetDirty(bool _dirty)
	{
		m_isDirty = _dirty;
	}

	void Chunk::SetVisible(bool _visible)
	{
		m_isVisible = _visible;
	}

	void Chunk::Build(const std::vector<sf::IntRect>& _textureRect, Camera* _cam)
	{
		m_groundVertices.clear();
		m_objectVertices.clear();

		const sf::Vector3f camPos = _cam->GetPos();

		for (int y = 0; y < chunkSize; ++y)
		{
			for (int x = 0; x < chunkSize; ++x)
			{
				TileData tileData = m_tiles[y * chunkSize + x];
				sf::IntRect rect = _textureRect[tileData.tileId];

				float worldX = static_cast<float>(m_chunkPos.x * chunkSize + x);
				float worldY = static_cast<float>(m_chunkPos.y * chunkSize + y);

				sf::Vector2f screenPos = _cam->WorldToScreen(
					{ worldX, worldY, tileData.height },
					{ static_cast<float>(rect.width), static_cast<float>(rect.height) }
				);

				const float scale = 1.0f / (tileData.height - camPos.z);
				const float scaledWidth = static_cast<float>(rect.width) * scale;
				const float scaledHeight = static_cast<float>(rect.height) * scale;

				sf::VertexArray& targetVertices = (tileData.height < 0.5f) ? m_groundVertices : m_objectVertices;

				// Top - Left
				targetVertices.append(sf::Vertex(
					screenPos,
					sf::Color::White,
					{ static_cast<float>(rect.left), static_cast<float>(rect.top) }));

				// Top - Right
				targetVertices.append(sf::Vertex(
					{ screenPos.x + scaledWidth, screenPos.y },
					sf::Color::White,
					{ static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top) }));

				// Bottom - Right
				targetVertices.append(sf::Vertex(
					{ screenPos.x + scaledWidth, screenPos.y + scaledHeight },
					sf::Color::White,
					{ static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top + rect.height) }));

				// Bottom - Left
				targetVertices.append(sf::Vertex(
					{ screenPos.x, screenPos.y + scaledHeight },
					sf::Color::White,
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
