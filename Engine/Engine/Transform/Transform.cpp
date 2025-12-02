#include "Transform.hpp"

namespace Engine
{
	Transform::Transform()
	{
		position = { 0.f,0.f,0.f };
		size = { 0.f,0.f,0.f };
		scale = { 0.f, 0.f };
		angle = 0.f;
	}
}
