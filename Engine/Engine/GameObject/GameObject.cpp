#include "GameObject.hpp"

#include "../Camera/Camera.hpp"
#include "../Render/RenderAPI.hpp"
#include "../System/System.hpp"

namespace Engine
{
	GameObject::GameObject()
	{
		m_transform = { {0.0f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f}, {1.0f,1.0f}, {0.f, 0.f, 0.f} };
		m_object = nullptr;
		m_tag = "Untagged";
	}

	GameObject::GameObject(const Transform& _transform)
	{
		m_transform = _transform;
		m_object = nullptr;
		m_tag = "Untagged";
	}

	GameObject::~GameObject()
	{
		delete m_object;
	}

	void GameObject::Display()
	{
		System::drawCall += 1;
		if (System::currentCamera != nullptr)
		{
			if (RenderAPI::GetIsUsed())
			{
				if (m_object != nullptr && m_tag != "Light")
				{
					(*System::currentCamera)->DrawObject(this, *RenderAPI::m_sceneMap);
				}
				else if (m_object == nullptr && m_tag == "Light")
				{
					(*System::currentCamera)->DrawLight(this, *RenderAPI::m_lightMap);
				}
			}
			else if (m_tag != "Light")
			{
				(*System::currentCamera)->DrawObject(this, *System::window);
			}
		}
	}

	const Transform& GameObject::GetTransform() const
	{
		return m_transform;
	}

	Object* GameObject::GetShape() const
	{
		return m_object;
	}

	const std::string& GameObject::GetTag()
	{
		return m_tag;
	}

	void GameObject::SetTransform(const Transform& _transform)
	{
		m_transform = _transform;
	}

	void GameObject::SetObject(Object** _object)
	{
		m_object = *_object;
	}

	void GameObject::SetTag(const std::string& _tag)
	{
		m_tag = _tag;
	}
}
