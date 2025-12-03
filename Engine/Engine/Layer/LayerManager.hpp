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

	struct Layer
	{
		GameObject* object;
		uint8_t order;
	};

	class LayerManager
	{
	private:
		static std::vector<Layer> m_layers;
		static std::vector<Layer> m_layersBuffer;
		static std::future<void> m_sortTask;
		static std::atomic<bool> m_useBuffer;
	public:
		static void Add(GameObject* _object, const uint8_t& _order);

		static void Reserve(int _size);
		static void Clear();
		static void Update(CameraType _camType);
		static void Draw(Camera* _cam);
	};
}

#endif