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
	class Camera;

	class GameObject
	{
	private:
		Transform m_transform;
		Object* m_object = nullptr;
		std::string m_tag;
	public:
		GameObject() = default;
		~GameObject() = default;

		void virtual Init() = 0;
		void virtual Update(const float& _dt) = 0;
		void virtual Display(const Camera* _camera);

	public: // Getters
		const Transform& GetTransform();
		Object* GetShape();
		const std::string& GetTag();
	};
}

#endif // !GAME_OBJECT__HPP