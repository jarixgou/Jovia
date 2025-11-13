#include "Chunk.hpp"

namespace Engine

{
	Chunk::Chunk(const sf::Vector2i& _chunkPos) :
		m_chunkPos(_chunkPos), m_tiles(chunkSize* chunkSize, { 0,0 }),
	{

	}

	const sf::Vector2i& Chunk::GetChunkPos()
	{

	}

	const sf::VertexArray& Chunk::GetVertices()
	{

	}

	int Chunk::GetTile(const sf::Vector2i& _pos)
	{

	}

	bool Chunk::GetIsVisible()
	{

	}

	void Chunk::SetTile(const sf::Vector2i& _pos, int tileId)
	{
	}

	void Chunk::SetDirty(bool _dirty)
	{
	}

	void Chunk::SetVisible(bool _visible)
	{
	}

	void Chunk::Build(const std::vector<sf::IntRect>& _textureRect, const Camera* _cam)
	{
	}

	void Chunk::Clear()
	{
	}
}