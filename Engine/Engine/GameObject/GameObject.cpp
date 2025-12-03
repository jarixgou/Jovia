#include "GameObject.hpp"

#include "../Camera/Camera.hpp"
#include "../Render/RenderAPI.hpp"
#include "../System/System.hpp"

namespace Engine
{
	void GameObject::Display(const Camera* _camera)
	{
		if (RenderAPI::GetIsUsed())
		{
			_camera->DrawObject(this, *RenderAPI::m_sceneMap);
		}
		else
		{
			_camera->DrawObject(this, *System::window);
		}
	}

	const Transform& GameObject::GetTransform()
	{
		return m_transform;
	}

	Object* GameObject::GetShape()
	{
		return m_object;
	}

	const std::string& GameObject::GetTag()
	{
		return m_tag;
	}
}
