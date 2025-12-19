#ifndef MATH__HPP
#define MATH__HPP
#include <array>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Engine
{
	/**
	 * @namespace Math
	 * @brief Mathematical utilities for 2D and 3D transformations
	 *
	 * Provides matrix operations, rotation matrices, and coordinate conversions
	 * for camera projections and object transformations.
	 */
	namespace Math
	{
		using Mat3x3 = std::array<std::array<float, 3>, 3>; ///< 3x3 matrix type
		using Mat2x2 = std::array<std::array<float, 2>, 2>; ///< 2x2 matrix type

		/**
		 * @brief Multiplies two 3x3 matrices
		 *
		 * @param _a First matrix
		 * @param _b Second matrix
		 * @return Result of matrix multiplication (_a * _b)
		 */
		Mat3x3 MultiplyMat(const Mat3x3& _a, const Mat3x3& _b);

		/**
		 * @brief Multiplies a 3x3 matrix by a 3D vector
		 *
		 * @param _a Matrix to multiply
		 * @param _b Vector to transform
		 * @return Transformed vector
		 */
		sf::Vector3f MultiplyMatVector(const Mat3x3& _a, const sf::Vector3f& _b);

		/**
		 * @brief Creates a 3D rotation matrix from Euler angles
		 *
		 * Combines rotations around X, Y, and Z axes into a single rotation matrix.
		 *
		 * @param _angleX Rotation around X axis in degrees
		 * @param _angleY Rotation around Y axis in degrees
		 * @param _angleZ Rotation around Z axis in degrees
		 * @return Combined rotation matrix
		 */
		Mat3x3 CreateRotationMatrix(float _angleX, float _angleY, float _angleZ);

		/**
		 * @brief Creates an isometric projection matrix
		 *
		 * Converts 3D coordinates to 2D isometric view coordinates.
		 *
		 * @param _objectSize Size of the object for scaling
		 * @return Isometric projection matrix
		 */
		Mat3x3 CreateIsoMatrix(const sf::Vector2f& _objectSize);

		/**
		 * @brief Multiplies two 2x2 matrices
		 *
		 * @param _a First matrix
		 * @param _b Second matrix
		 * @return Result of matrix multiplication (_a * _b)
		 */
		Mat2x2 MultiplyMat(const Mat2x2& _a, const Mat2x2& _b);

		/**
		 * @brief Multiplies a 2x2 matrix by a 2D vector
		 *
		 * @param _a Matrix to multiply
		 * @param _b Vector to transform
		 * @return Transformed vector
		 */
		sf::Vector2f MultiplyMatVector(const Mat2x2& _a, const sf::Vector2f& _b);

		/**
		 * @brief Creates a 2D rotation matrix
		 *
		 * @param _angle Rotation angle in degrees
		 * @return 2D rotation matrix
		 */
		Mat2x2 CreateRotationMatrix(const float& _angle);

		/**
		 * @brief Converts radians to degrees
		 *
		 * @param _rad Angle in radians
		 * @return Angle in degrees
		 */
		float RadToDeg(const float& _rad);

		/**
		 * @brief Converts degrees to radians
		 *
		 * @param _deg Angle in degrees
		 * @return Angle in radians
		 */
		float DegToRad(const float& _deg);
	}
}

#endif