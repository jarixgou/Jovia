#include "TileMap.hpp"

#include "../Camera/Camera.hpp"
#include "Chunk.hpp"
#include "../Asset/AssetsManager.hpp"
#include "../System/System.hpp"
#include "../TextureSlice/TextureSlice.hpp"

namespace Engine
{
	TileMap::TileMap(const sf::Vector2i& _worldSize, const float& _height, const sf::Vector2i& _cellSize, const char* _textureName)
	{
		m_worldSize = _worldSize;
		m_height = _height;

		sf::Texture const* texture = AssetsManager::Get<sf::Texture>(_textureName);
		m_renderStates = sf::RenderStates(texture);

		std::vector<TextureSliced> slicedTextures = SliceTexture(*texture, _cellSize);
		for (const auto& slicedTexture : slicedTextures)
		{
			m_textureRects.push_back(slicedTexture.rect);
		}
		slicedTextures.clear();

		Clear();
	}

	TileMap::~TileMap()
	{
		Clear();

		GameObject::~GameObject();
	}

	void TileMap::Update()
	{
		UpdateVisibleChunks();
		RebuildDirtyChunks();
	}

	Chunk* TileMap::GetOrCreateChunk(const sf::Vector2i& _chunkPos)
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

		const sf::IntRect rect = m_textureRects.at(0);
		const float tileWidth = static_cast<float>(rect.width);
		const float tileHeight = static_cast<float>(rect.height);

		Transform tempTransform;
		tempTransform.position = {
			static_cast<float>(_chunkPos.x * chunkSize),  
			static_cast<float>(_chunkPos.y * chunkSize),  
			m_height
		};
		tempTransform.rotation = { 0.0f, 0.0f, 0.0f };
		tempTransform.scale = { 1.0f, 1.0f };
		tempTransform.size = {
			static_cast<float>(chunkSize),  // Largeur du chunk en TILES
			static_cast<float>(chunkSize),  // Hauteur du chunk en TILES
			0.0f
		};

		auto newChunk = std::make_unique<Chunk>(tempTransform, m_renderStates);
		Chunk* chunkPtr = newChunk.get();
		m_chunks[_chunkPos] = std::move(newChunk);

		return chunkPtr;
	}

	Chunk* TileMap::GetChunk(const sf::Vector2i& _chunkPos) const
	{
		auto it = m_chunks.find(_chunkPos);
		if (it != m_chunks.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	void TileMap::UpdateVisibleChunks()
	{
		if (System::currentCamera == nullptr)
		{
			return;
		}

		if ((*System::currentCamera)->GetHasMoved())
		{
			const float zCamera = (*System::currentCamera)->GetTransform().position.z;

			const float zValue = m_height - zCamera;

			if (zValue >= 2.220446e-16 && zValue <= (*System::currentCamera)->GetRenderDistance())
			{
				m_visibleChunks.clear();

				const sf::IntRect rect = m_textureRects.at(0);
				const float tileWidth = static_cast<float>(rect.width);
				const float tileHeight = static_cast<float>(rect.height);

				const sf::FloatRect visibleArea = (*System::currentCamera)->GetVisibleArea({ tileWidth, tileHeight }, m_height);

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
		}
	}

	void TileMap::SetChunkDirty(const sf::Vector2i& _chunkPos) const
	{
		Chunk* chunk = GetChunk(_chunkPos);
		if (chunk)
		{
			chunk->SetDirty(true);
		}
	}

	int TileMap::GetTileAt(const sf::Vector2i& _worldPos) const
	{
		sf::Vector2i chunkPos = WorldToChunkPos(_worldPos);
		sf::Vector2i localPos = WorldToLocalPos(_worldPos);

		Chunk* chunk = GetChunk(chunkPos);
		if (chunk)
		{
			return chunk->GetTile(localPos);
		}
		return -1;
	}

	void TileMap::SetTileAt(const sf::Vector2i& _worldPos, const uint8_t& _tileId, const float& _tileHeight)
	{
		sf::Vector2i chunkPos = WorldToChunkPos(_worldPos);

		Chunk* chunk = GetOrCreateChunk(chunkPos);
		if (chunk)
		{
			sf::Vector2i localPos = WorldToLocalPos(_worldPos);

			chunk->SetTile(localPos, _tileId);
			chunk->SetDirty(true);
		}
	}

	void TileMap::RebuildDirtyChunks()
	{
		if (System::currentCamera == nullptr)
		{
			return;
		}

		for (auto & [pos, chunk] : m_chunks)
		{
			if (chunk->GetIsVisible() && chunk->GetIsDirty())
			{
				chunk->Build(m_textureRects);
			}
		}
	}

	void TileMap::Clear()
	{
		m_chunks.clear();
	}

	void TileMap::Display()
	{
		for (auto & chunk : m_visibleChunks)
		{
			chunk->Display();
		}
	}

	const std::vector<Chunk*> TileMap::GetChunks() const
	{
		return m_visibleChunks;
	}

	sf::Vector2i TileMap::WorldToChunkPos(const sf::Vector2i& _worldPos) const
	{
		return { _worldPos.x / chunkSize, _worldPos.y / chunkSize };
	}

	sf::Vector2i TileMap::WorldToLocalPos(const sf::Vector2i& _worldPos) const
	{
		return { _worldPos.x % chunkSize, _worldPos.y % chunkSize };
	}
}
