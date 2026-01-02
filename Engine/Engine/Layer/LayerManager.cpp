#include "LayerManager.hpp"

#include <execution>
#include <algorithm>

#include "../Camera/Camera.hpp"
#include "../GameObject/GameObject.hpp"
#include "../System/System.hpp"

namespace Engine
{
	std::vector<Layer> LayerManager::m_layers;
	std::vector<Layer> LayerManager::m_layersBuffer;
	std::future<void> LayerManager::m_sortTask;
	std::atomic<bool> LayerManager::m_useBuffer = false;

	void LayerManager::Add(GameObject* _object, const uint8_t& _order)
	{
		m_layers.emplace_back(Layer{ _object, _order });
	}

	void LayerManager::Reserve(int _size)
	{
		m_layers.reserve(_size);
		m_layersBuffer.reserve(_size);
	}

	void LayerManager::Clear()
	{
		if (m_sortTask.valid())
		{
			m_sortTask.wait();
		}

		m_layers.clear();
		m_layersBuffer.clear();
		m_useBuffer = false;
	}

	void LayerManager::Update()
	{
		if (m_layers.size() <= 1)
		{
			return;
		}

		if (m_sortTask.valid())
		{
			m_sortTask.wait();
		}

		m_useBuffer = false;
		m_layersBuffer.resize(m_layers.size());
		std::copy(std::execution::par_unseq, m_layers.begin(), m_layers.end(), m_layersBuffer.begin());
		CameraType camType = (*System::currentCamera)->GetType();

		m_sortTask = std::async(std::launch::async, [camType, &layers = m_layersBuffer]()
			{
				if (camType == CameraType::ORTHOGONAL)
				{
					auto orthographiqueCompare = [](const Layer& _a, const Layer& _b) noexcept -> bool
						{
							sf::Vector3f aPos = _a.object->GetTransform().position;
							sf::Vector3f bPos = _b.object->GetTransform().position;
							return std::tie(_a.order, bPos.z, aPos.y, aPos.x) <
								std::tie(_b.order, aPos.z, bPos.y, bPos.x);
						};

					if (layers.size() > 5000)
					{
						std::sort(std::execution::par_unseq, layers.begin(), layers.end(), orthographiqueCompare);
					}
					else if (layers.size() > 500)
					{
						std::sort(std::execution::par, layers.begin(), layers.end(), orthographiqueCompare);
					}
					else
					{
						std::ranges::sort(layers, orthographiqueCompare);
					}
				}
				else if (camType == CameraType::ISOMETRIC)
				{
					auto isometricCompare = [](const Layer& _a, const Layer& _b) noexcept -> bool
						{
							if (_a.order != _b.order)
							{
								return _a.order < _b.order;
							}

							const sf::Vector3f aPos = _a.object->GetTransform().position;
							const sf::Vector3f bPos = _b.object->GetTransform().position;

							constexpr float epsilon = 0.001f;
							const float isoDepthA = aPos.x + aPos.y;
							const float isoDepthB = bPos.x + bPos.y;
							const float depthDiff = isoDepthA - isoDepthB;

							if (depthDiff < -epsilon || depthDiff > epsilon)
							{
								return isoDepthA < isoDepthB;
							}

							const float yDiff = aPos.y - bPos.y;
							if (yDiff < -epsilon || yDiff > epsilon)
							{
								return aPos.y < bPos.y;
							}

							return aPos.x< bPos.x;
						};

					if (layers.size() > 5000)
					{
						std::sort(std::execution::par_unseq, layers.begin(), layers.end(), isometricCompare);
					}
					else if (layers.size() > 500)
					{
						std::sort(std::execution::par, layers.begin(), layers.end(), isometricCompare);
					}
					else
					{
						std::ranges::sort(layers, isometricCompare);
					}
				}
				m_useBuffer = true;
			});
	}

	void LayerManager::Display()
	{
		const auto& layersToDraw = m_useBuffer ? m_layersBuffer : m_layers;

		if (layersToDraw.empty())
		{
			return;
		}

		for (auto it = layersToDraw.begin(); it != layersToDraw.end(); ++it)
		{
			const Layer& layer = *it;

			layer.object->Display();
		}
	}
}