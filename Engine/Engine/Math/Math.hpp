#ifndef MATH__HPP
#define MATH__HPP
#include <array>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Engine
{
	namespace Math
	{
		using Mat3x3 = std::array<std::array<float, 3>, 3>;
		using Mat2x2 = std::array<std::array<float, 2>, 2>;

		Mat3x3 MultiplyMat(const Mat3x3& _a, const Mat3x3& _b);
		sf::Vector3f MultiplyMat3x3Vector(const Mat3x3& _a, const sf::Vector3f& _b);
		Mat3x3 CreateRotationMatrix(float _angleX, float _angleY, float _angleZ);
		Mat3x3 CreateIsoMatrix(const sf::Vector2f& _objectSize);

		Mat2x2 MultiplyMat(const Mat2x2& _a, const Mat2x2& _b);
		Mat2x2 CreateRotationMatrix(const float& _angle);

		float RadToDeg(const float& _rad);
		float DegToRad(const float& _deg);
	}
}

#endif