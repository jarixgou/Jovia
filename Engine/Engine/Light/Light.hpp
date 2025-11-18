#ifndef LIGHT__HPP
#define LIGHT__HPP
#include <SFML/System/Vector3.hpp>
#include <vector>
#include <SFML/Graphics/Color.hpp>

namespace Engine
{
	class Light
	{
	private:
		float m_radius;
		sf::Vector3f m_pos;
		sf::Color m_color;

		float m_intensity;
		float m_radialFalloff;
		float m_angularFalloff;
		float m_volumetricIntensity;
	public:
		Light();
		~Light();

		void SetRadius(const float& _radius);
		void SetIntensity(const float& _intensity);
		void SetPos(const sf::Vector3f& _pos);
		void SetColor(const sf::Color& _color);

		float GetRadius() const;
		float GetIntensity() const;
		const sf::Vector3f& GetPos() const;
		const sf::Color& GetColor() const;
	};
}

#endif
