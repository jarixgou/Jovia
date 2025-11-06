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

	}

	template <typename T>
	int QuadTree<T>::GetIndex(const QuadTreeObject<T>& _object)
	{
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
	}

	template <typename T>
	void QuadTree<T>::Insert(const QuadTreeObject<T>& _object)
	{
	}

	template <typename T>
	std::vector<T*> QuadTree<T>::Retrieve(const sf::FloatRect& _area)
	{
	}
}

#endif