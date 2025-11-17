#include "ChunkManager.hpp"

#include "../Camera/Camera.hpp"
#include "Chunk.hpp"

namespace Engine
{
	ChunkManager::ChunkManager() : m_worldSize(0, 0)
	{
	}

	ChunkManager::~ChunkManager()
	{
		Clear();
	}

	void ChunkManager::Init(const sf::Vector2i& _worldSize, const std::vector<sf::IntRect>& _textureRects)
	{
		m_worldSize = _worldSize;
		m_textureRects = _textureRects;
		Clear();
	}

	Chunk* ChunkManager::GetOrCreateChunk(const sf::Vector2i& _chunkPos)
	{
		if (_chunkPos.x < 0 || _chunkPos.x >= m_worldSize.x ||
			_chunkPos.y < 0 || _chunkPos.y >= m_worldSize.y)
		{
			return nullptr;
		}

		// Try to find the chunk
		auto it = m_chunks.find(_chunkPos);
		if (it != m_chunks.end())
		{
			return it->second.get();
		}

		auto newChunk = std::make_unique<Chunk>(_chunkPos);
		Chunk* chunkPtr = newChunk.get();
		m_chunks[_chunkPos] = std::move(newChunk);

		return chunkPtr;
	}

	Chunk* ChunkManager::GetChunk(const sf::Vector2i& _chunkPos)
	{
		auto it = m_chunks.find(_chunkPos);
		if (it != m_chunks.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	void ChunkManager::UpdateVisibleChunks(Camera* _camera)
	{
		m_visibleChunks.clear();

		sf::IntRect rect = m_textureRects.at(0);
		sf::FloatRect visibleArea = _camera->GetVisibleArea({ static_cast<float>(rect.width), static_cast<float>(rect.height)});

		// Frustum culling for chunks
		const int startX = std::max(0, static_cast<int>(visibleArea.left / chunkSize));
		const int startY = std::max(0, static_cast<int>(visibleArea.top / chunkSize));
		const int endX = std::min(m_worldSize.x, static_cast<int>((visibleArea.left + visibleArea.width) / chunkSize) + 1);
		const int endY = std::min(m_worldSize.y, static_cast<int>((visibleArea.top + visibleArea.height) / chunkSize) + 1);

		m_visibleChunks.reserve((endX - startX) * (endY - startY));

		for (auto& [pos, chunk] : m_chunks)
		{
			chunk->SetVisible(false);
		}

		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{
				Chunk* chunk = GetOrCreateChunk({ x, y });
				if (chunk)
				{
					chunk->SetVisible(true);
					chunk->SetDirty(true);
					m_visibleChunks.emplace_back(chunk);
				}
			}
		}
	}

	void ChunkManager::SetChunkDirty(const sf::Vector2i& _chunkPos)
	{
		Chunk* chunk = GetChunk(_chunkPos);
		if (chunk)
		{
			chunk->SetDirty(true);
		}
	}

	int ChunkManager::GetTileAt(const sf::Vector2i& _worldPos)
	{
		sf::Vector2i chunkPos = WorldToChunkPos(_worldPos);
		sf::Vector2i localPos = WorldToLocalPos(_worldPos);

		Chunk* chunk = GetChunk(chunkPos);
		if (chunk)
		{
			return chunk->GetTile(localPos);
		}
	}

	void ChunkManager::SetTileAt(const sf::Vector2i& _worldPos, int _tileId, float _tileHeight)
	{
		sf::Vector2i chunkPos = WorldToChunkPos(_worldPos);
		sf::Vector2i localPos = WorldToLocalPos(_worldPos);

		Chunk* chunk = GetOrCreateChunk(chunkPos);
		if (chunk)
		{
			chunk->SetTile(localPos, _tileId, _tileHeight);
			chunk->SetDirty(true);
		}
	}

	void ChunkManager::RebuildDirtyChunks(Camera* _camera)
	{
		if (!_camera)
		{
			return;
		}

		for (auto & [pos, chunk] : m_chunks)
		{
			if (chunk->GetIsVisible() && chunk->GetIsDirty())
			{
				chunk->Build(m_textureRects, _camera);
			}
		}
	}

	void ChunkManager::Clear()
	{
		m_chunks.clear();
	}

	const std::vector<Chunk*> ChunkManager::GetChunks() const
	{
		return m_visibleChunks;
	}

	sf::Vector2i ChunkManager::WorldToChunkPos(const sf::Vector2i& _worldPos) const
	{
		return { _worldPos.x / chunkSize, _worldPos.y / chunkSize };
	}

	sf::Vector2i ChunkManager::WorldToLocalPos(const sf::Vector2i& _worldPos) const
	{
		return { _worldPos.x % chunkSize, _worldPos.y % chunkSize };
	}
}
