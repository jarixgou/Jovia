#ifndef LIGHT__HPP
#define LIGHT__HPP
#include <SFML/System/Vector3.hpp>
#include <vector>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>

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

		sf::CircleShape* m_lightShape;

		sf::VertexArray m_projectedShadow;

		sf::RenderTexture m_shadowMap;
		sf::Sprite m_shadowMapSprite;

		sf::RenderTexture m_lightMap;
		sf::Sprite m_lightMapSprite;

		sf::RenderTexture m_compositeLightAndShadow;
		sf::Sprite m_compositeLightAndShadowSprite;
	public:
		Light(sf::CircleShape* _lightShape);
		~Light() = default;

		void Update();
		void Display();

		void BuildProjectedShadow(const sf::VertexArray& _object);
	public: // Getters and Setters
		void SetRadius(const float& _radius);
		void SetPos(const sf::Vector3f& _pos);
		void SetColor(const sf::Color& _color);
		void SetIntensity(const float& _intensity);
		void SetRadialFalloff(const float& _radialFalloff);
		void SetAngularFalloff(const float& _angularFalloff);
		void SetVolumetricIntensity(const float& _volumetricIntensity);

		float GetRadius() const;
		const sf::Vector3f& GetPos() const;
		const sf::Color& GetColor() const;
		float GetIntensity() const;
		float GetRadialFalloff() const;
		float GetAngularFalloff() const;
		float GetVolumetricIntensity() const;

		sf::Sprite& GetCompositeLightAndShadow();
		sf::Sprite& GetLightMap();
		sf::Sprite& GetShadowMap();
	};
}

#endif
