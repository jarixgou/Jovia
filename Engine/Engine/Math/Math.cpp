#include "Math.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Engine
{
	namespace Math
	{
		Mat3x3 MultiplyMat(const Mat3x3& _a, const Mat3x3& _b)
		{
			Mat3x3 result = { 0 };
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					for (int k = 0; k < 3; ++k)
					{
						result[i][j] += _a[i][k] * _b[k][j];
					}
				}
			}
			return result;
		}

		sf::Vector3f MultiplyMatVector(const Mat3x3& _a, const sf::Vector3f& _b)
		{
			sf::Vector3f result;
			result.x = _a[0][0] * _b.x + _a[0][1] * _b.y + _a[0][2] * _b.z;
			result.y = _a[1][0] * _b.x + _a[1][1] * _b.y + _a[1][2] * _b.z;
			result.z = _a[2][0] * _b.x + _a[2][1] * _b.y + _a[2][2] * _b.z;
			return result;
		}

		Mat3x3 CreateRotationMatrix(float _angleX, float _angleY, float _angleZ)
		{
			const float radX = DegToRad(_angleX);
			const float radY = DegToRad(_angleY);
			const float radZ = DegToRad(_angleZ);

			const float cX = cosf(radX);
			const float sX = sinf(radX);

			const float cY = cosf(radY);
			const float sY = sinf(radY);

			const float cZ = cosf(radZ);
			const float sZ = sinf(radZ);

			Mat3x3 rX = { {
				{1, 0, 0},
				{0, cX, -sX},
				{0, sX, cX}
			} };

			Mat3x3 rY = { {
				{cY, 0, sY},
				{0, 1, 0},
				{-sY, 0, cY}
			} };

			Mat3x3 rZ = { {
				{cZ, -sZ, 0},
				{sZ, cZ, 0},
				{0, 0, 1}
			} };

			return MultiplyMat(rY, MultiplyMat(rX, rZ));
		}

		Mat3x3 CreateIsoMatrix(const sf::Vector2f& _objectSize)
		{
			const float w = _objectSize.x;
			const float h = _objectSize.y;
			Mat3x3 result = { {
					{0.5f * w, -0.5f * w, 0},
					{0.25f * h, 0.25f * h, -0.25f * h},
					{1, 1, 1}
				} };

			return result;
		}

		Mat2x2 MultiplyMat(const Mat2x2& _a, const Mat2x2& _b)
		{
			Mat2x2 result = { 0 };
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					for (int k = 0; k < 2; ++k)
					{
						result[i][j] += _a[i][k] * _b[k][j];
					}
				}
			}
			return result;
		}

		sf::Vector2f MultiplyMatVector(const Mat2x2& _a, const sf::Vector2f& _b)
		{
			sf::Vector2f result;
			result.x = _a[0][0] * _b.x + _a[0][1] * _b.y;
			result.y = _a[1][0] * _b.x + _a[1][1] * _b.y;
			return result;
		}

		Mat2x2 CreateRotationMatrix(const float& _angle)
		{
			const float rad = DegToRad(_angle);

			const float c = cosf(rad);
			const float s = sinf(rad);

			Mat2x2 result = { {
				{c, -s},
				{s, c}
				} };

			return result;
		}

		float RadToDeg(const float& _rad)
		{
			return static_cast<float>(_rad * (180.f / M_PI));
		}

		float DegToRad(const float& _deg)
		{
			return static_cast<float>(_deg * (M_PI / 180.f));
		}
	}
}
