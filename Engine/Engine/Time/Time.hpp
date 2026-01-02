#ifndef TIME__HPP
#define TIME__HPP
#include <SFML/System/Clock.hpp>

namespace Engine
{
	class Time
	{
	private:
		sf::Clock m_clock;
		sf::Time m_time;
		float m_deltaTime;
		float m_clockSpeed;
	public:
		Time() = default;
		~Time() = default;

		void Restart();										///< Restart the internal clock
	public: // Getters/Setters

		const sf::Time& GetTime() const; 					///< Get the elapsed time since last restart
		const float& GetDeltaTime() const;					///< Get the delta time in seconds

		void SetClockSpeed(const float& _speed);			///< Set the clock speed multiplier
		const float& GetClockSpeed() const;					///< Get the clock speed multiplier
	};
}

#endif