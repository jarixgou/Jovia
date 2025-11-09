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

	enum class DrawableType : uint8_t
	{
		SPRITE,
		RECTANGLE_SHAPE,
		CIRCLE_SHAPE
	};

	struct DrawableObject
	{
		DrawableType type;
		union
		{
			sf::Sprite* sprite;
			sf::RectangleShape* rectangleShape;
			sf::CircleShape* circleShape;
		};

		DrawableObject(sf::Sprite* _sprite) : type(DrawableType::SPRITE), sprite(_sprite) {};
		DrawableObject(sf::RectangleShape* _rectangleShape) : type(DrawableType::RECTANGLE_SHAPE), rectangleShape(_rectangleShape) {};
		DrawableObject(sf::CircleShape* _circleShape) : type(DrawableType::CIRCLE_SHAPE), circleShape(_circleShape) {};
	};

	struct Layer
	{
		sf::Vector3f pos;
		sf::Vector2f size;
		int order;
		DrawableObject object;
	};

	class LayerManager
	{
	private:
		static std::vector<Layer> m_layers;
		static std::vector<Layer> m_layersBuffer;
		static std::future<void> m_sortTask;
		static std::atomic<bool> m_useBuffer;
	public:
		static void Add(sf::Sprite* _sprite, const sf::Vector3f& _pos, const sf::Vector2f& _size, int _order);
		static void Add(sf::RectangleShape* _rectangleShape, const sf::Vector3f& _pos, const sf::Vector2f& _size, int _order);
		static void Add(sf::CircleShape* _circleShape, const sf::Vector3f& _pos, const sf::Vector2f& _size, int _order);

		static void Reserve(int _size);
		static void Clear();
		static void Update(CameraType _camType);
		static void Draw(Camera* _cam, sf::RenderWindow& _window);
	};
}

#endif