#ifndef RENDER_API__HPP
#define RENDER_API__HPP

#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace Engine
{
	/**
	 * @class RenderAPI
	 * @brief Manages deferred rendering pipeline with lighting
	 *
	 * Provides a two-pass rendering system: scene rendering and light compositing.
	 * Separates scene geometry from lighting calculations, allowing for complex
	 * lighting effects without re-rendering the scene multiple times.
	 */
	class RenderAPI
	{
	private:
		static bool m_used;                       ///< Flag indicating if API is active
		static sf::RenderStates m_renderPipline;  ///< Render states for compositing
		static sf::Shader* m_lightShader;         ///< Shader for light compositing

		static std::unique_ptr<sf::Sprite> m_lightMapSprite; ///< Sprite for light map
		static std::unique_ptr<sf::Sprite> m_sceneMapSprite; ///< Sprite for scene map
	public:
		static std::unique_ptr<sf::RenderTexture> m_lightMap; ///< Light accumulation buffer
		static std::unique_ptr<sf::RenderTexture> m_sceneMap; ///< Scene geometry buffer

	public:
		/**
		 * @brief Initializes the rendering pipeline
		 *
		 * Creates render textures, loads shaders, and sets up render states.
		 * Must be called before using the RenderAPI.
		 */
		static void Init();

		/**
		 * @brief Clears all render targets
		 *
		 * Clears both scene and light buffers, preparing for a new frame.
		 */
		static void Clear();

		/**
		 * @brief Composites and displays the final image
		 *
		 * Combines the scene map and light map using the light shader,
		 * then displays the result to the main window.
		 */
		static void Display();

		/**
		 * @brief Cleans up rendering resources
		 *
		 * Releases render textures and shaders. Call on application shutdown.
		 */
		static void Cleanup();

		/**
		 * @brief Checks if the RenderAPI is active
		 *
		 * @return True if the deferred rendering pipeline is enabled
		 */
		static bool GetIsUsed();
	};
}
#endif