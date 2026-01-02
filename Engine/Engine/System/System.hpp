#ifndef SYSTEM__HPP
#define SYSTEM__HPP
#include <SFML/Graphics/RenderWindow.hpp>

#include "../Time/Time.hpp"


namespace Engine
{
	class Camera;
	/**
	* @struct System
	* @brief Global system state and utilities
	*
	* Provides access to the main render window and tracks rendering statistics.
	* All members are static for global access throughout the engine.
	*/
	struct System
	{
		static std::unique_ptr<sf::RenderWindow> window; ///< Main application window
		static bool drawLight;							 ///< Flag to enable/disable light rendering
		static int verticeNb;							 ///< Counter for number of vertices rendered this frame
		static int drawCall;							 ///< Counter for number of draw calls this frame
		static Camera** currentCamera; 					 ///< Pointer to the currently active camera
		static Time time;                                 ///< Global time management instance
	};

	/**
	 * @brief Decrypts and loads a file from the asset system
	 *
	 * Reads an encrypted .asset file, decrypts it using the engine's key,
	 * and returns the decrypted content as a string for loading into memory.
	 *
	 * @param _filePath Path to the encrypted asset file
	 * @return Decrypted file content as a string
	 */
	std::string ChangeFileFormat(std::string _filePath);
}

#endif