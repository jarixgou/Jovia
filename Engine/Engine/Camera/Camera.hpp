#ifndef CAMERA__HPP
#define CAMERA__HPP
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Engine
{
	class GameObject;
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
	class Camera
	{
	private:
		sf::Vector3f m_pos = { 0,0,0 };             ///< Camera position in 3D space
		sf::Vector2f m_size = { 1920,1080 };        ///< Viewport size in pixels
		sf::Vector3f m_angle = { 0,0,0 };           ///< Camera rotation angles (degrees)
		bool m_free = false;                        ///< Free movement mode flag

		sf::Vector3f m_target = { 0,0,0 };          ///< Target position to follow

		CameraType m_type = CameraType::ORTHOGONAL; ///< Current projection type
	public:
		/**
		 * @brief Default constructor
		 */
		Camera() = default;

		/**
		 * @brief Default destructor
		 */
		~Camera() = default;

		/**
		 * @brief Updates the camera each frame
		 *
		 * Handles keyboard input for free camera movement and normalizes rotation angles.
		 * Movement behavior depends on the camera type (orthogonal or isometric).
		 *
		 * @param _dt Delta time in seconds since last frame
		 */
		void Update(float _dt);

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
		 * @brief Sets the camera position in 3D space
		 *
		 * @param _pos New camera position (x, y, z)
		 */
		void SetPos(sf::Vector3f _pos);

		/**
		 * @brief Sets the viewport size
		 *
		 * @param _size Viewport dimensions in pixels (width, height)
		 */
		void SetSize(sf::Vector2f _size);

		/**
		 * @brief Sets the camera rotation angles
		 *
		 * @param _angle Rotation angles in degrees (x, y, z)
		 */
		void SetAngle(const sf::Vector3f& _angle);

		/**
		 * @brief Sets the camera projection type
		 *
		 * @param _type Projection type (ORTHOGONAL or ISOMETRIC)
		 */
		void SetType(CameraType _type);

		/**
		 * @brief Calculates the visible area in world coordinates
		 *
		 * Computes the bounding rectangle of the world area visible by the camera.
		 * The calculation method differs based on projection type.
		 *
		 * @param _tileSize Size of a single tile in pixels (for scale calculation)
		 * @return FloatRect representing the visible area (x, y, width, height)
		 */
		sf::FloatRect GetVisibleArea(sf::Vector2f _tileSize) const;

		/**
		 * @brief Gets the current camera position
		 *
		 * @return 3D position vector (x, y, z)
		 */
		sf::Vector3f GetPos() const;

		/**
		 * @brief Gets the viewport size
		 *
		 * @return Viewport dimensions in pixels (width, height)
		 */
		sf::Vector2f GetSize() const;

		/**
		 * @brief Gets the free movement mode state
		 *
		 * @return True if free movement is enabled, false otherwise
		 */
		bool GetFree() const;

		/**
		 * @brief Gets the current rotation angles
		 *
		 * @return Const reference to rotation angles in degrees (x, y, z)
		 */
		const sf::Vector3f& GetAngle() const;

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

		/**
		 * @brief Converts world coordinates to screen coordinates
		 *
		 * Transforms a 3D world position to 2D screen position based on camera
		 * projection, position, and rotation.
		 *
		 * @param _transform Transform containing world position and properties
		 * @return 2D screen position in pixels
		 */
		sf::Vector2f WorldToScreen(const Transform& _transform) const;

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