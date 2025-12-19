#ifndef SCENE__HPP
#define SCENE__HPP
#include "SFML/Graphics/RenderWindow.hpp"

namespace Engine
{
	/**
	 * @class Scene
	 * @brief Base class for game scenes/states
	 * 
	 * Provides the foundation for different game states (menus, gameplay, etc.).
	 * Each scene manages its own initialization, update loop, rendering, and cleanup.
	 * Scenes can be switched through the ScenesManager.
	 */
	class Scene
	{
	public:
		/**
		 * @brief Constructor
		 */
		Scene();

		/**
		 * @brief Virtual destructor
		 */
		virtual ~Scene();

		/**
		 * @brief Initializes the scene
		 * 
		 * Called once when the scene is first loaded. Override to set up
		 * scene-specific resources, objects, and initial state.
		 */
		virtual void Init();

		/**
		 * @brief Handles window events
		 * 
		 * Called each frame to process input and window events (keyboard, mouse, etc.).
		 * 
		 * @param _event SFML event structure containing event data
		 */
		virtual void PollEvents(sf::Event& _event);

		/**
		 * @brief Updates the scene logic
		 * 
		 * Called each frame to update game logic, physics, and object states.
		 * 
		 * @param _dt Delta time in seconds since last frame
		 */
		virtual void Update(float _dt);

		/**
		 * @brief Renders the scene
		 * 
		 * Called each frame to draw all visual elements to the screen.
		 */
		virtual void Display();

		/**
		 * @brief Cleans up the scene
		 * 
		 * Called when the scene is unloaded. Override to free resources,
		 * delete objects, and perform necessary cleanup.
		 */
		virtual void Cleanup();
	};
}

#endif