#include "Transform.hpp"

namespace Engine
{
	bool Transform::operator==(const Transform& _other) const
	{
		if (position == _other.position && 
			size == _other.size &&
			scale == _other.scale &&
			rotation == _other.rotation)
		{
			return true;
		}
		return false;
	}

	bool Transform::operator!=(const Transform& _other) const
	{
		return !(*this == _other);
	}
}
