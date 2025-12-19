#ifndef CHUNK__HPP
#define CHUNK__HPP
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

namespace Engine
{
	class Camera;

	constexpr int chunkSize = 16; ///< Number of tiles per chunk dimension (16x16)

	/**
	 * @struct TileData
	 * @brief Data for a single tile in a chunk
	 */
	struct TileData
	{
		uint8_t tileId; ///< Index to the tile texture in the tileset
		float height;   ///< Height/elevation of the tile for 3D rendering
	};

	/**
	 * @class Chunk
	 * @brief Manages a 16x16 grid of tiles for efficient rendering
	 *
	 * Chunks divide the game world into manageable sections, allowing for
	 * frustum culling, batched rendering, and efficient updates. Each chunk
	 * maintains separate vertex arrays for ground and object layers.
	 */
	class Chunk
	{
	private:
		sf::Vector2i m_chunkPos;              ///< Chunk position in chunk-space coordinates
		std::vector<TileData> m_tiles;        ///< Tile data array (chunkSize * chunkSize)
		sf::VertexArray m_groundVertices;     ///< Batched vertices for ground layer
		sf::VertexArray m_objectVertices;     ///< Batched vertices for object layer
		bool m_isDirty;                       ///< Flag indicating vertices need rebuilding
		bool m_isVisible;                     ///< Flag indicating chunk is in camera view
	public:
		/**
		 * @brief Constructs a chunk at the specified position
		 *
		 * @param _chunkPos Chunk coordinates (not world coordinates)
		 */
		Chunk(const sf::Vector2i& _chunkPos);

		/**
		 * @brief Default destructor
		 */
		~Chunk() = default;

		/**
		 * @brief Gets the chunk position
		 * @return Chunk coordinates
		 */
		const sf::Vector2i& GetChunkPos() const;

		/**
		 * @brief Gets the ground layer vertices
		 * @return Const reference to ground vertex array
		 */
		const sf::VertexArray& GetGroundVertices() const;

		/**
		 * @brief Gets the object layer vertices
		 * @return Const reference to object vertex array
		 */
		const sf::VertexArray& GetObjectVertices() const;

		/**
		 * @brief Gets a tile at the specified local position
		 *
		 * @param _pos Local position within chunk (0-15, 0-15)
		 * @return Tile ID at the position
		 */
		int GetTile(const sf::Vector2i& _pos) const;

		/**
		 * @brief Checks if the chunk is currently visible
		 * @return True if chunk is in camera frustum
		 */
		bool GetIsVisible() const;

		/**
		 * @brief Checks if the chunk needs rebuilding
		 * @return True if vertices need to be regenerated
		 */
		bool GetIsDirty() const;

		/**
		 * @brief Sets a tile at the specified local position
		 *
		 * Marks the chunk as dirty, requiring vertex regeneration.
		 *
		 * @param _pos Local position within chunk (0-15, 0-15)
		 * @param _tileId Tile texture index
		 * @param _tileHeight Elevation of the tile
		 */
		void SetTile(const sf::Vector2i& _pos, const uint8_t& _tileId, const float& _tileHeight);

		/**
		 * @brief Sets the dirty flag
		 *
		 * @param _dirty True if vertices need rebuilding
		 */
		void SetDirty(const bool& _dirty);

		/**
		 * @brief Sets the visibility flag
		 *
		 * @param _visible True if chunk is in camera view
		 */
		void SetVisible(const bool& _visible);

		/**
		 * @brief Rebuilds the chunk's vertex arrays
		 *
		 * Generates optimized vertex geometry for all tiles in the chunk,
		 * applying camera transformations and separating ground/object layers.
		 *
		 * @param _textureRect Array of texture rectangles from the tileset
		 * @param _cam Camera used for world-to-screen transformations
		 */
		void Build(const std::vector<sf::IntRect>& _textureRect, const Camera* _cam);

		/**
		 * @brief Clears all vertices in the chunk
		 */
		void Clear();
	};
}

#endif