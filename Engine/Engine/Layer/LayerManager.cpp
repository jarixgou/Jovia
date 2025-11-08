#include "LayerManager.hpp"

#include <execution>
#include <algorithm>

#include "../Camera/Camera.hpp"

namespace Engine
{
	std::vector<Layer> LayerManager::m_layers;
	std::future<void> LayerManager::m_sortTask;

	void LayerManager::Add(sf::Sprite&& _sprite, const sf::Vector3f& _pos, const sf::Vector2f& _size, int _order)
	{
		m_layers.emplace_back(Layer{ _pos, _size, _order, std::move(_sprite) });
	}

	void LayerManager::Add(sf::RectangleShape&& _rectangleShape, const sf::Vector3f& _pos, const sf::Vector2f& _size, int _order)
	{
		m_layers.emplace_back(Layer{ _pos, _size, _order, std::move(_rectangleShape) });
	}

	void LayerManager::Add(sf::CircleShape&& _circleShape, const sf::Vector3f& _pos, const sf::Vector2f& _size, int _order)
	{
		m_layers.emplace_back(Layer{ _pos, _size, _order, std::move(_circleShape) });
	}

	void LayerManager::Reserve(int _size)
	{
		m_layers.reserve(_size);
	}

	void LayerManager::Clear()
	{
		if (m_sortTask.valid())
		{
			m_sortTask.wait();
		}
		m_layers.clear();
	}

	void LayerManager::Update(CameraType _camType)
	{
		if (m_sortTask.valid())
		{
			m_sortTask.wait();
		}

		if (m_layers.size() <= 1)
		{
			return;
		}

		m_sortTask = std::async(std::launch::async, [_camType, &layers = m_layers]()
		{
				if (_camType == CameraType::ORTHOGRAPHIC)
				{
					auto orthographiqueCompare = [](const Layer& a, const Layer& b) noexcept -> bool
						{
							return std::tie(a.order, b.pos.z, a.pos.y, a.pos.x) <
								std::tie(b.order, a.pos.z, b.pos.y, b.pos.x);
						};

					if (layers.size() > 10000)
					{
						std::sort(std::execution::par_unseq, layers.begin(), layers.end(), orthographiqueCompare);
					}
					else if (layers.size() > 1000)
					{
						std::sort(std::execution::par, layers.begin(), layers.end(), orthographiqueCompare);
					}
					else
					{
						std::ranges::sort(layers, orthographiqueCompare);
					}
				}
				else if (_camType == CameraType::ISOMETRIC)
				{
					auto isometricCompare = [](const Layer& a, const Layer& b) noexcept -> bool
						{
							if (a.order != b.order)
							{
								return a.order < b.order;
							}

							constexpr float epsilon = 0.001f;
							const float isoDepthA = a.pos.x + a.pos.y;
							const float isoDepthB = b.pos.x + b.pos.y;
							const float depthDiff = isoDepthA - isoDepthB;

							if (depthDiff < -epsilon || depthDiff > epsilon)
							{
								return isoDepthA < isoDepthB;
							}

							const float yDiff = a.pos.y - b.pos.y;
							if (yDiff < -epsilon || yDiff > epsilon)
							{
								return a.pos.y < b.pos.y;
							}

							return a.pos.x < b.pos.x;
						};

					if (layers.size() > 10000)
					{
						std::sort(std::execution::par_unseq, layers.begin(), layers.end(), isometricCompare);
					}
					else if (layers.size() > 1000)
					{
						std::sort(std::execution::par, layers.begin(), layers.end(), isometricCompare);
					}
					else
					{
						std::ranges::sort(layers, isometricCompare);
					}
				}
		});
	}

	void LayerManager::Draw(Camera* _cam, sf::RenderWindow& _window)
	{
		if (m_sortTask.valid())
		{
			m_sortTask.wait();
		}

		for (const auto& layer : m_layers)
		{
			std::visit([&](const auto& obj)
				{
					_cam->DrawObject(obj, layer.pos, layer.size, _window);
				}, layer.object);
		}
	}
}