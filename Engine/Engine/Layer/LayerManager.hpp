#ifndef LAYER_MANAGER__HPP
#define LAYER_MANAGER__HPP
#include <future>
#include <variant>
#include <vector>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector3.hpp>

namespace Engine
{
	class Camera;
	enum class CameraType : uint8_t;
	class GameObject;

	/**
	 * @struct Layer
	 * @brief Associates a GameObject with a rendering order
	 */
	struct Layer
	{
		GameObject* object; ///< Pointer to the game object
		uint8_t order;      ///< Rendering order (lower values draw first)
	};

	/**
	 * @class LayerManager
	 * @brief Manages rendering order and depth sorting of game objects
	 *
	 * Provides a layer-based rendering system with automatic depth sorting.
	 * Uses double buffering and asynchronous sorting for performance.
	 * Objects are sorted based on their Y position for isometric projection
	 * or Z position for orthogonal projection.
	 */
	class LayerManager
	{
	private:
		static std::vector<Layer> m_layers;           ///< Active layer list
		static std::vector<Layer> m_layersBuffer;     ///< Buffer for double-buffering
		static std::future<void> m_sortTask;          ///< Async sorting task handle
		static std::atomic<bool> m_useBuffer;         ///< Flag for buffer swapping
	public:
		/**
		 * @brief Adds a GameObject to the rendering queue
		 *
		 * @param _object Pointer to the GameObject to render
		 * @param _order Base rendering order (0-255, lower draws first)
		 */
		static void Add(GameObject* _object, const uint8_t& _order);

		/**
		 * @brief Reserves memory for layers
		 *
		 * Pre-allocates memory to avoid reallocations during runtime.
		 *
		 * @param _size Number of layers to reserve
		 */
		static void Reserve(int _size);

		/**
		 * @brief Clears all layers
		 *
		 * Removes all objects from the rendering queue.
		 */
		static void Clear();

		/**
		 * @brief Updates and sorts the rendering order
		 *
		 * Sorts layers based on camera type (Y-position for isometric,
		 * Z-position for orthogonal). Sorting is done asynchronously.
		 *
		 * @param _camType Type of camera projection used for sorting
		 */
		static void Update(CameraType _camType);

		/**
		 * @brief Draws all layers in sorted order
		 *
		 * Renders all GameObjects in the layer list using the provided camera.
		 *
		 * @param _cam Camera used for rendering transformations
		 */
		static void Draw(Camera* _cam);
	};
}

#endif