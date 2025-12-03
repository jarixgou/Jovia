#include "GameObject.hpp"

#include "../Camera/Camera.hpp"
#include "../Render/RenderAPI.hpp"

namespace Engine
{
	void GameObject::Display(const Camera* _camera)
	{
		// TODO: adapt the camera to use GameObject
		// TODO: create a static class who contains the render
		if (RenderAPI::GetIsUsed())
		{
			_camera->DrawObject(this, *RenderAPI::m_sceneMap);
		}
		else
		{
			//_camera->DrawObject(this, System::window);
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
