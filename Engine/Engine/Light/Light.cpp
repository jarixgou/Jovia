#include "Light.hpp"

namespace Engine
{
	void Light::SetRadius(const float& _radius)
	{
		m_radius = _radius;
	}

	void Light::SetPos(const sf::Vector3f& _pos)
	{
		m_pos = _pos;
	}

	void Light::SetColor(const sf::Color& _color)
	{
		m_color = _color;
	}

	void Light::SetIntensity(const float& _intensity)
	{
		m_intensity = _intensity;
	}

	void Light::SetRadialFalloff(const float& _radialFalloff)
	{
		m_radialFalloff = _radialFalloff;
	}

	void Light::SetAngularFalloff(const float& _angularFalloff)
	{
		m_angularFalloff = _angularFalloff;
	}

	void Light::SetVolumetricIntensity(const float& _volumetricIntensity)
	{
		m_volumetricIntensity = _volumetricIntensity;
	}

	float Light::GetRadius() const
	{
		return m_radius;
	}

	const sf::Vector3f& Light::GetPos() const
	{
		return m_pos;
	}

	const sf::Color& Light::GetColor() const
	{
		return m_color;
	}

	float Light::GetIntensity() const
	{
		return m_intensity;
	}

	float Light::GetRadialFalloff() const
	{
		return m_radialFalloff;
	}

	float Light::GetAngularFalloff() const
	{
		return m_angularFalloff;
	}

	float Light::GetVolumetricIntensity() const
	{
		return m_volumetricIntensity;
	}
}