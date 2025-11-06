#ifndef QUAD_TREE__HPP
#define QUAD_TREE__HPP
#include <memory>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Engine
{
	template <typename T>
	struct QuadTreeObject
	{
		sf::Vector3f pos;
		sf::Vector2f size;
		T* data;
	};

	template <typename T>
	class QuadTree
	{
	private:
		void Split();
		int GetIndex(const QuadTreeObject<T>& _object);

		int m_level;
		int m_maxObjects;
		int m_maxLevels;
		sf::FloatRect m_bounds;

		std::vector<QuadTreeObject<T>> m_objects;
		std::unique_ptr<QuadTree<T>> m_nodes[4];
	public:
		QuadTree(sf::FloatRect _bounds, int _maxObjects = 10, int _maxLevel = 5, int _level = 0);

		void Clear();
		void Insert(const QuadTreeObject<T>& _object);
		std::vector<T*> Retrieve(const sf::FloatRect& _area);
	};

	template <typename T>
	void QuadTree<T>::Split()
	{
		sf::Vector2f sub = {m_bounds.width / 2.f, m_bounds.height / 2.f};
		sf::Vector2f pos = { m_bounds.left, m_bounds.top };

		// Top-Right
		m_nodes[0] = std::make_unique<QuadTree<T>>(
			sf::FloatRect(pos.x + sub.x, pos.y, sub.x, sub.y),
			m_maxObjects, m_maxLevels, m_level + 1);

		// Top-Left
		m_nodes[1] = std::make_unique<QuadTree<T>>(
			sf::FloatRect(pos.x, pos.y, sub.x, sub.y),
			m_maxObjects, m_maxLevels, m_level + 1);

		// Bottom-Left
		m_nodes[2] = std::make_unique<QuadTree<T>>(
			sf::FloatRect(pos.x, pos.y + sub.y, sub.x, sub.y),
			m_maxObjects, m_maxLevels, m_level + 1);

		// Bottom-Right
		m_nodes[3] = std::make_unique<QuadTree<T>>(
			sf::FloatRect(pos.x + sub.x, pos.y + sub.y, sub.x, sub.y),
			m_maxObjects, m_maxLevels, m_level + 1);
	}

	template <typename T>
	int QuadTree<T>::GetIndex(const QuadTreeObject<T>& _object)
	{
		int index = -1;

		float verticalMidpoint = m_bounds.left + (m_bounds.width / 2.f);
		float horizontalMidpoint = m_bounds.top + (m_bounds.height / 2.f);

		bool topQuadrant = (_object.pos.y < horizontalMidpoint && _object.pos.y + _object.size.y < horizontalMidpoint);
		bool bottomQuadrant = (_object.pos.y > horizontalMidpoint);

		if (_object.pos.x < verticalMidpoint && _object.pos.x + _object.size.x < verticalMidpoint)
		{
			if (topQuadrant)
			{
				index = 1; // Top-Left
			}
			else if (bottomQuadrant)
			{
				index = 2; // Bottom-Left
			}
		}
		else if (_object.pos.x > verticalMidpoint)
		{
			if (topQuadrant)
			{
				index = 0; // Top-Right
			}
			else if (bottomQuadrant)
			{
				index = 3; // Bottom-Right
			}
		}
		return index;
	}

	template <typename T>
	QuadTree<T>::QuadTree(sf::FloatRect _bounds, int _maxObjects, int _maxLevel, int _level)
	{
		m_bounds = _bounds;
		m_maxObjects = _maxObjects;
		m_maxLevels = _maxLevel;
		m_level = _level;
	}

	template <typename T>
	void QuadTree<T>::Clear()
	{
		m_objects.clear();
		for (int i = 0; i < 4; ++i)
		{
			if (m_nodes[i])
			{
				m_nodes[i]->Clear();
				m_nodes[i].reset();
			}
		}
	}

	template <typename T>
	void QuadTree<T>::Insert(const QuadTreeObject<T>& _object)
	{
		if (m_nodes[0] != nullptr)
		{
			int idx = GetIndex(_object);
			if (idx != -1)
			{
				m_nodes[idx]->Insert(_object);
			}
		}

		m_objects.push_back(_object);

		if (m_objects.size() > m_maxObjects && m_level < m_maxLevels)
		{
			if (m_nodes[0] == nullptr)
			{
				Split();
			}

			auto it = m_objects.begin();
			while (it != m_objects.end())
			{
				int idx = GetIndex(*it);
				if (idx != -1)
				{
					m_nodes[idx]->Insert(*it);
					it = m_objects.erase(it);
				}
				else
				{
					it += 1;
				}
			}
		}
	}

	template <typename T>
	std::vector<T*> QuadTree<T>::Retrieve(const sf::FloatRect& _area)
	{
		std::vector<T*> foundObjects;

		if (m_nodes[0] != nullptr)
		{
			for (int i = 0; i < 4; ++i)
			{
				auto nodeObject = m_nodes[i]->Retrieve(_area);
				foundObjects.insert(foundObjects.end(), nodeObject.begin(), nodeObject.end());
			}
		}

		// Add object on the current node
		for (auto & obj : m_objects)
		{
			sf::FloatRect objectBounds(obj.pos.x, obj.pos.y, obj.size.x, obj.size.y);
			if (objectBounds.intersects(_area))
			{
				foundObjects.push_back(obj.data);
			}
		}

		return foundObjects;
	}
}

#endif