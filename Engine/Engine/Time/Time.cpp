#include "Time.hpp"

namespace Engine
{
	void Time::Restart()
	{
		m_time = m_clock.restart();
		m_deltaTime = m_time.asSeconds() * m_clockSpeed;
	}

	const sf::Time& Time::GetTime() const
	{
		return m_time;
	}

	const float& Time::GetDeltaTime() const
	{
		return m_deltaTime;
	}

	void Time::SetClockSpeed(const float& _speed)
	{
		m_clockSpeed = _speed;
	}

	const float& Time::GetClockSpeed() const
	{
		return m_clockSpeed;
	}
}
