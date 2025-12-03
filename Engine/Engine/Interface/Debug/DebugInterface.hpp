#ifndef DEBUG_INTERFACE__HPP
#define DEBUG_INTERFACE__HPP

#include <Pdh.h>
#include <GL/glew.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Engine
{
	class DebugInterface
	{
	private:
		static float m_dt;
		static float m_gpuTime;
	public:
		static void Update(float _dt, float _gpuTime);
	};
}

#endif