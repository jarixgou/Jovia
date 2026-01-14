#ifndef PIN__HPP
#define PIN__HPP
#include <vector>

struct Link;

enum class PinType
{
	INT,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	TEXTURE,
	UV,
	COLOR,
};

struct Pin
{
	Link* link;
	PinType type;
	void* value;
};

#endif
