#ifndef RENDER_API__HPP
#define RENDER_API__HPP

#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>

namespace Engine
{
	enum class DrawableType : uint8_t;
	struct DrawableObject;

	class RenderAPI
	{
	private:
		static sf::RenderStates lightRender;
		static sf::Shader lightShader;

		static std::unique_ptr<sf::RenderTexture> sceneMap;

	public: // Public variable
		static std::unique_ptr<sf::RenderTexture> lightMap;
		std::vector<DrawableObject*> sceneObject;
	public: // Public function
		static void Init();
		static void Display();
	};

}
#endif