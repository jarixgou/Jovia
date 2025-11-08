#include "LayerManager.hpp"

#include <execution>

#include "../Camera/Camera.hpp"

namespace Engine
{
	std::vector<Layer> LayerManager::m_layers;

	void LayerManager::Add(sf::Sprite&& _sprite, const sf::Vector3f& _pos, const sf::Vector2f& _size, int _order)
	{
		m_layers.emplace_back(Layer{ _pos, _size, _order, _sprite });
	}

	void LayerManager::Add(sf::RectangleShape&& _rectangleShape, const sf::Vector3f& _pos, const sf::Vector2f& _size, int _order)
	{
		m_layers.emplace_back(Layer{ _pos, _size, _order, _rectangleShape });
	}

	void LayerManager::Add(sf::CircleShape&& _circleShape, const sf::Vector3f& _pos, const sf::Vector2f& _size, int _order)
	{
		m_layers.emplace_back(Layer{ _pos, _size, _order, _circleShape });
	}

	void LayerManager::Reserve(int _size)
	{
		m_layers.reserve(_size);
	}

	void LayerManager::Clear()
	{
		m_layers.clear();
	}

	void LayerManager::Update(CameraType _camType)
	{
		if (_camType == CameraType::ORTHOGRAPHIC)
		{
			// Tri parallèle avec std::execution::par
			std::sort(std::execution::par, m_layers.begin(), m_layers.end(),
				[](const Layer& a, const Layer& b)
				{
					if (a.order != b.order)
					{
						return a.order < b.order;
					}

					if (a.pos.z != b.pos.z)
					{
						return a.pos.z > b.pos.z;
					}

					if (a.pos.y != b.pos.y)
					{
						return a.pos.y < b.pos.y;
					}
					return a.pos.x < b.pos.x;
				});
		}
		else if (_camType == CameraType::ISOMETRIC)
		{
			std::sort(std::execution::par, m_layers.begin(), m_layers.end(),
				[](const Layer& a, const Layer& b)
				{
					if (a.order != b.order)
						return a.order < b.order;

					float isoDepthA = a.pos.x + a.pos.y;
					float isoDepthB = b.pos.x + b.pos.y;

					if (std::abs(isoDepthA - isoDepthB) > 0.001f)
						return isoDepthA < isoDepthB;
					if (std::abs(a.pos.y - b.pos.y) > 0.001f)
						return a.pos.y < b.pos.y;
					return a.pos.x < b.pos.x;
				});
		}
	}

	void LayerManager::Draw(Camera* _cam, sf::RenderWindow& _window)
	{
		for (auto& layer : m_layers)
		{
			std::visit([&](const auto& obj)
				{
					_cam->DrawObject(obj, layer.pos, layer.size, _window);
				}, layer.object);
		}
	}
}