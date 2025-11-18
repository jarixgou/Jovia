#ifndef CHUNK_MANAGER__HPP
#define CHUNK_MANAGER__HPP
#include <functional>
#include <memory>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace Engine
{
	class Camera;
	class Chunk;

	struct Vector2iHash
	{
		std::size_t operator()(const sf::Vector2i& v) const noexcept
		{
			return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
		}
	};

	class ChunkManager
	{
	private:
		std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash> m_chunks;
		std::vector<Chunk*> m_visibleChunks;

		sf::Vector2i m_worldSize;

		std::vector<sf::IntRect> m_textureRects;
	public:
		ChunkManager();
		~ChunkManager();

		void Init(const sf::Vector2i& _worldSize, const std::vector<sf::IntRect>& _textureRects);

		Chunk* GetOrCreateChunk(const sf::Vector2i& _chunkPos);

		Chunk* GetChunk(const sf::Vector2i& _chunkPos) const;

		void UpdateVisibleChunks(const Camera* _camera);

		void SetChunkDirty(const sf::Vector2i& _chunkPos) const;

		int GetTileAt(const sf::Vector2i& _worldPos) const;

		void SetTileAt(const sf::Vector2i& _worldPos, const uint8_t& _tileId, const float& _tileHeight);

		void RebuildDirtyChunks(const Camera* _camera);

		void Clear();

		const std::vector<Chunk*> GetChunks() const;

	private:
		sf::Vector2i WorldToChunkPos(const sf::Vector2i& _worldPos) const;
		sf::Vector2i WorldToLocalPos(const sf::Vector2i& _worldPos) const;
	};
}

#endif