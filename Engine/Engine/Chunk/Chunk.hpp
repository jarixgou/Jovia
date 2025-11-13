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
		int tileId;
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

		const sf::Vector2i& GetChunkPos();
		const sf::VertexArray& GetGroundVertices();
		const sf::VertexArray& GetObjectVertices();
		int GetTile(const sf::Vector2i& _pos);
		bool GetIsVisible();
		bool GetIsDirty();

		void SetTile(const sf::Vector2i& _pos, int _tileId, float _tileHeight);
		void SetDirty(bool _dirty);
		void SetVisible(bool _visible);

		void Build(const std::vector<sf::IntRect>& _textureRect, Camera* _cam);
		void Clear();
	};
}

#endif