#ifndef CHUNK__HPP
#define CHUNK__HPP
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

namespace Engine
{
	class Camera;

	constexpr int chunkSize = 16;

	struct TileData
	{
		uint8_t tileId;
		float height;
	};

	class Chunk
	{
	private:
		sf::Vector2i m_chunkPos;
		std::vector<TileData> m_tiles;
		sf::VertexArray m_groundVertices;
		sf::VertexArray m_objectVertices;
		bool m_isDirty;
		bool m_isVisible;
	public:
		Chunk(const sf::Vector2i& _chunkPos);
		~Chunk() = default;

		const sf::Vector2i& GetChunkPos() const;
		const sf::VertexArray& GetGroundVertices() const;
		const sf::VertexArray& GetObjectVertices() const;
		int GetTile(const sf::Vector2i& _pos) const;
		bool GetIsVisible() const;
		bool GetIsDirty() const;

		void SetTile(const sf::Vector2i& _pos, const uint8_t& _tileId, const float& _tileHeight);
		void SetDirty(const bool& _dirty);
		void SetVisible(const bool& _visible);

		void Build(const std::vector<sf::IntRect>& _textureRect, const Camera* _cam);
		void Clear();
	};
}

#endif