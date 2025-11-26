#include "Light.hpp"

#include <cmath>

namespace Engine
{
	Light::Light(sf::CircleShape* _lightShape)
	{
		m_projectedShadow.setPrimitiveType(sf::Quads);

		m_shadowMap.create(1920, 1080);
		m_shadowMapSprite.setTexture(m_shadowMap.getTexture());

		m_lightMap.create(1920, 1080);
		m_lightMapSprite.setTexture(m_lightMap.getTexture());

		m_compositeLightAndShadow.create(1920, 1080);
		m_compositeLightAndShadowSprite.setTexture(m_compositeLightAndShadow.getTexture());

		m_lightShape = _lightShape;
	}

	void Light::Update()
	{
		m_projectedShadow.clear();
	}

	void Light::Display()
	{
		// Render shadow map
		m_shadowMap.clear(sf::Color::White);

		m_shadowMap.draw(m_projectedShadow);

		m_shadowMap.display();

		// Render light map
		m_lightMap.clear(sf::Color::Black);

		m_lightShape->setRadius(m_radius);
		m_lightShape->setOrigin(m_radius, m_radius);
		m_lightShape->setPosition({ m_pos.x, m_pos.y });
		m_lightShape->setFillColor(m_color);

		m_lightMap.draw(*m_lightShape, sf::BlendAdd);

		m_lightMap.display();

		// Fusion light and shadow maps
		m_compositeLightAndShadow.clear(sf::Color::White);

		m_compositeLightAndShadow.draw(m_lightMapSprite, sf::BlendMultiply);
		m_compositeLightAndShadow.draw(m_shadowMapSprite, sf::BlendMultiply);

		m_compositeLightAndShadow.display();
	}

	void Light::BuildProjectedShadow(const sf::VertexArray& _object)
	{
		const size_t objectVertexCount = _object.getVertexCount();

		// Early out if object is outside light radius
		for (int i = 0; i < objectVertexCount; ++i)
		{
			sf::Vector2f pos = _object[i].position;
			sf::Vector2f lightPos = {m_pos.x, m_pos.y};
			sf::Vector2f ap = { pos.x - lightPos.x, pos.y - lightPos.y };
			if (std::sqrt((ap.x * ap.x) + (ap.y * ap.y)) > m_radius)
			{
				return;
			}
		}

		// Calulate the area of the object
		float area = 0.f;
		for (size_t i = 0; i < objectVertexCount; ++i)
		{
			const sf::Vector2f a = _object[i].position;
			const sf::Vector2f b = _object[(i + 1) % objectVertexCount].position;
			area += (a.x * b.y) - (b.x * a.y);
		}
		area *= 0.5f;

		// Project shadows
		for (size_t i = 0; i < objectVertexCount; ++i)
		{
			const sf::Vector2f a = _object[i].position;
			const sf::Vector2f b = _object[(i + 1) % objectVertexCount].position;

			const sf::Vector2f ab = b - a;
			const sf::Vector2f toLight = sf::Vector2f(m_pos.x, m_pos.y) - a;

			// Cross product to determine if edge faces light
			const float cross = (ab.x * toLight.y) - (ab.y * toLight.x);

			// If polygon CCW (area > 0) then cross < 0 => edge faces light
			// If polygon CW (area < 0) then cross > 0 => edge faces light
			const bool facesLight = (area >= 0.f) ? (cross < 0.f) : (cross > 0.f);
			if (facesLight)
			{
				// Normalize direction from light to vertex
				float denom = std::sqrt((a.x - m_pos.x) * (a.x - m_pos.x) + (a.y - m_pos.y) * (a.y - m_pos.y));
				const sf::Vector2f dir1 = {(a.x - m_pos.x) / denom, (a.y - m_pos.y) / denom };

				denom = std::sqrt((b.x - m_pos.x) * (b.x - m_pos.x) + (b.y - m_pos.y) * (b.y - m_pos.y));
				const sf::Vector2f dir2 = { (b.x - m_pos.x) / denom, (b.y - m_pos.y) / denom };

				// Project points far away
				const sf::Vector2f p1 = a + dir1 * 2000.f;
				const sf::Vector2f p2 = b + dir2 * 2000.f;

				m_projectedShadow.append({ a, sf::Color::Black });      
				m_projectedShadow.append({ b, sf::Color::Black });
				m_projectedShadow.append({ p2, sf::Color::Black });        
				m_projectedShadow.append({ p1, sf::Color::Black });
			}
		}
	}

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

	sf::Sprite& Light::GetCompositeLightAndShadow()
	{
		return m_compositeLightAndShadowSprite;
	}

	sf::Sprite& Light::GetLightMap()
	{
		return m_lightMapSprite;
	}

	sf::Sprite& Light::GetShadowMap()
	{
		return m_shadowMapSprite;
	}
}
