#ifndef GAME_OBJECT__HPP
#define GAME_OBJECT__HPP

#include <string>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../Object/Object.hpp"
#include "../Transform/Transform.hpp"

namespace Engine
{
	/**
	 * @class GameObject
	 * @brief Base class for all game entities in the engine
	 *
	 * GameObject is an abstract class that provides the foundation for all interactive
	 * objects in the game world. It manages transform data, visual representation,
	 * and lifecycle methods (Init, Update, Display).
	 */
	class GameObject
	{
	private:
		Transform m_transform;   ///< Transform data (position, size, scale, rotation)
		Object* m_object;		 ///< Visual representation (sprite, shape, etc.)
		std::string m_tag;       ///< Identifier tag for categorization and lookup
	public:
		/**
		 * @brief Default constructor
		 */
		GameObject();
		GameObject(const Transform& _transform);

		/**
		 * @brief Default destructor
		 */
		~GameObject();

		/**
		 * @brief Updates the game object each frame
		 *
		 * Pure virtual function that must be implemented by derived classes.
		 * Called every frame to update object logic and state.
		 *
		 */
		void virtual Update() = 0;

		/**
		 * @brief Displays the game object
		 *
		 * Renders the object using the provided camera's transformation.
		 * Default implementation provided, can be overridden.
		 *
		 */
		void virtual Display();

	public: // Getters/Setters
		/**
		 * @brief Gets the transform component
		 *
		 * @return Const reference to the object's transform data
		 */
		const Transform& GetTransform() const;

		/**
		 * @brief Gets the visual representation
		 *
		 * @return Pointer to the Object containing the drawable shape
		 */
		Object* GetShape() const;

		/**
		 * @brief Gets the object's tag
		 *
		 * @return Const reference to the tag string
		 */
		const std::string& GetTag();

		void SetTransform(const Transform& _transform);
		void SetObject(Object** _object);
		void SetTag(const std::string& _tag);
	};
}

#endif // !GAME_OBJECT__HPP