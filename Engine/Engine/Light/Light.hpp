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
	/**
	 * @class Light
	 * @brief 2D dynamic lighting system with shadow projection
	 * 
	 * Manages light properties, shadow casting, and composite rendering.
	 * Supports volumetric lighting, radial/angular falloff, and dynamic shadow
	 * projection from occluding objects.
	 */
	class Light
	{
	private:
		float m_radius;									///< Light radius in pixels
		sf::Vector3f m_pos;								///< Light position in 3D space
		sf::Color m_color;								///< Light color (RGB)

		float m_intensity;								///< Light intensity multiplier
		float m_radialFalloff;							///< Radial attenuation factor
		float m_angularFalloff;							///< Angular attenuation factor
		float m_volumetricIntensity;					///< Volumetric effect strength

		sf::CircleShape* m_lightShape;					///< Visual representation of light source

		sf::VertexArray m_projectedShadow;				///< Vertex array for shadow geometry

		sf::RenderTexture m_shadowMap;					///< Shadow map render target
		sf::Sprite m_shadowMapSprite;					///< Shadow map sprite for rendering

		sf::RenderTexture m_lightMap;					///< Light map render target
		sf::Sprite m_lightMapSprite;					///< Light map sprite for rendering

		sf::RenderTexture m_compositeLightAndShadow;	///< Combined light+shadow render target
		sf::Sprite m_compositeLightAndShadowSprite;		///< Composite sprite for final output

		sf::RenderStates m_lightMapStates;				///< Render states for light rendering
		sf::Shader* m_lightShader;						///< Shader for light effects
	public:
		/**
		 * @brief Constructs a light with a visual shape
		 * 
		 * @param _lightShape Pointer to the circle shape representing the light
		 */
		Light(sf::CircleShape* _lightShape);

		/**
		 * @brief Default destructor
		 */
		~Light() = default;

		/**
		 * @brief Updates the light state
		 * 
		 * Rebuilds light maps and applies shader parameters.
		 */
		void Update();

		/**
		 * @brief Renders the light
		 * 
		 * Draws the composite light and shadow to the current render target.
		 */
		void Display();

		/**
		 * @brief Builds projected shadows from an occluding object
		 * 
		 * Calculates and adds shadow geometry based on the object's vertices
		 * and the light's position.
		 * 
		 * @param _object Vertex array representing the occluding geometry
		 */
		void BuildProjectedShadow(const sf::VertexArray& _object);

	public: // Getters and Setters
		/**
		 * @brief Sets the light radius
		 * @param _radius Radius in pixels
		 */
		void SetRadius(const float& _radius);

		/**
		 * @brief Sets the light position
		 * @param _pos Position in 3D space
		 */
		void SetPos(const sf::Vector3f& _pos);

		/**
		 * @brief Sets the light color
		 * @param _color RGB color
		 */
		void SetColor(const sf::Color& _color);

		/**
		 * @brief Sets the light intensity
		 * @param _intensity Intensity multiplier (typically 0.0-2.0)
		 */
		void SetIntensity(const float& _intensity);

		/**
		 * @brief Sets the radial falloff
		 * @param _radialFalloff Falloff factor (0.0-1.0)
		 */
		void SetRadialFalloff(const float& _radialFalloff);

		/**
		 * @brief Sets the angular falloff
		 * @param _angularFalloff Falloff factor (0.0-1.0)
		 */
		void SetAngularFalloff(const float& _angularFalloff);

		/**
		 * @brief Sets the volumetric intensity
		 * @param _volumetricIntensity Volumetric effect strength (0.0-1.0)
		 */
		void SetVolumetricIntensity(const float& _volumetricIntensity);

		/**
		 * @brief Gets the light radius
		 * @return Radius in pixels
		 */
		float GetRadius() const;

		/**
		 * @brief Gets the light position
		 * @return Position in 3D space
		 */
		const sf::Vector3f& GetPos() const;

		/**
		 * @brief Gets the light color
		 * @return RGB color
		 */
		const sf::Color& GetColor() const;

		/**
		 * @brief Gets the light intensity
		 * @return Intensity multiplier
		 */
		float GetIntensity() const;

		/**
		 * @brief Gets the radial falloff
		 * @return Falloff factor
		 */
		float GetRadialFalloff() const;

		/**
		 * @brief Gets the angular falloff
		 * @return Falloff factor
		 */
		float GetAngularFalloff() const;

		/**
		 * @brief Gets the volumetric intensity
		 * @return Volumetric effect strength
		 */
		float GetVolumetricIntensity() const;

		/**
		 * @brief Gets the composite light and shadow sprite
		 * @return Reference to the composite sprite
		 */
		sf::Sprite& GetCompositeLightAndShadow();

		/**
		 * @brief Gets the light map sprite
		 * @return Reference to the light map sprite
		 */
		sf::Sprite& GetLightMap();

		/**
		 * @brief Gets the shadow map sprite
		 * @return Reference to the shadow map sprite
		 */
		sf::Sprite& GetShadowMap();
	};
}

#endif
