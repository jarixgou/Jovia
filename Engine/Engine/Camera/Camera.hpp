#ifndef CAMERA__HPP
#define CAMERA__HPP
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include "../GameObject/GameObject.hpp"
#include "../Math/Math.hpp"

namespace Engine
{
	struct Transform;

	/**
	 * @enum CameraType
	 * @brief Defines the type of camera projection
	 */
	enum class CameraType : uint8_t
	{
		ORTHOGONAL, ///< Orthogonal (2D) projection
		ISOMETRIC   ///< Isometric (2.5D) projection
	};

	/**
	 * @class Camera
	 * @brief Manages camera view, projection, and transformations for rendering
	 *
	 * The Camera class handles both orthogonal and isometric projections,
	 * providing world-to-screen coordinate transformations and viewport management.
	 */
	class Camera : public GameObject
	{
	private:
		Transform* m_lastTransform;					///< Last frame's transform for interpolation
		bool m_free;								///< Free movement mode flag
		bool m_hasMoved;							///< Indicates if the camera has moved this frame

		float m_renderDistance;						///< Render distance for culling objects

		Math::Mat3x3 m_rotationMatrix;				///< Cached rotation matrix for transformations

		sf::Vector3f m_target;						///< Target position to follow

		CameraType m_type = CameraType::ORTHOGONAL; ///< Current projection type
	public:
		/**
		 * @brief Default constructor
		 */
		Camera(const Transform& _transform, CameraType _type = CameraType::ORTHOGONAL, float _renderDistance = 5.f);

		/**
		 * @brief Default destructor
		 */
		~Camera();

		/**
		 * @brief Updates the camera each frame
		 *
		 * Handles keyboard input for free camera movement and normalizes rotation angles.
		 * Movement behavior depends on the camera type (orthogonal or isometric).
		 *
		 */
		void Update() override;

		/**
		 * @brief Enables or disables free camera movement
		 *
		 * @param _free True to enable manual keyboard control, false to disable
		 */
		void SetFree(bool _free = true);

		/**
		 * @brief Sets the target position for the camera to follow
		 *
		 * @param _target 3D position of the target
		 */
		void SetFollow(sf::Vector3f _target);

		/**
		 * @brief Sets the camera projection type
		 *
		 * @param _type Projection type (ORTHOGONAL or ISOMETRIC)
		 */
		void SetType(CameraType _type);

		void SetRenderDistance(const float& _renderDistance);

		const float& GetRenderDistance() const;

		const bool& GetHasMoved() const;

		/**
		 * @brief Calculates the visible area in world coordinates
		 *
		 * Computes the bounding rectangle of the world area visible by the camera.
		 * The calculation method differs based on projection type.
		 *
		 * @param _tileSize Size of a single tile in pixels (for scale calculation)
		 * @param _height
		 * @return FloatRect representing the visible area (x, y, width, height)
		 */
		sf::FloatRect GetVisibleArea(const sf::Vector2f& _tileSize, const float& _height) const;

		/**
		 * @brief Gets the free movement mode state
		 *
		 * @return True if free movement is enabled, false otherwise
		 */
		bool GetFree() const;

		/**
		 * @brief Gets the current projection type
		 *
		 * @return Current CameraType (ORTHOGONAL or ISOMETRIC)
		 */
		CameraType GetType() const;

		/**
		 * @brief Draws a GameObject with camera transformation applied
		 *
		 * Applies the camera's view transformation to the GameObject before rendering.
		 *
		 * @param _gameObject Pointer to the GameObject to draw
		 * @param _renderTarget SFML render target where to draw
		 */
		void DrawObject(GameObject* _gameObject, sf::RenderTarget& _renderTarget) const;

		void DrawLight(GameObject* _gameObject, sf::RenderTarget& _renderTarget) const;

		/**
		 * @brief Converts world coordinates to screen coordinates
		 *
		 * Transforms a 3D world position to 2D screen position based on camera
		 * projection, position, and rotation.
		 *
		 * @param _transform Transform containing world position and properties
		 * @return 2D screen position in pixels
		 */
		std::pair<sf::Vector2f, sf::Vector2f> WorldToScreen(const Transform& _transform) const;

	private:
		/**
		 * @brief Applies camera transformation to a drawable object
		 *
		 * Internal method that sets position, scale, and rotation of SFML drawables
		 * based on camera projection and world transform.
		 *
		 * @param _object Pointer to the drawable object to transform
		 * @param _transform Transform containing world-space properties
		 */
		void TransformObject(sf::Drawable* _object, const Transform& _transform) const;
	};
}

#endif