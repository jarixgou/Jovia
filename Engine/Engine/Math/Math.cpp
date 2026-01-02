#include "Math.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <immintrin.h> // AVX/SSE

namespace Engine
{
	namespace Math
	{
		Mat3x3 MultiplyMatrix(const Mat3x3& _a, const Mat3x3& _b)
		{
			Mat3x3 result = { 0 };

#ifdef __AVX__
			// Optimisation AVX pour multiplication de matrices 3x3
			for (int i = 0; i < 3; ++i)
			{
				__m256 row = _mm256_setzero_ps();
				for (int k = 0; k < 3; ++k)
				{
					__m256 aValue = _mm256_set1_ps(_a[i][k]);
					__m256 bRow = _mm256_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, _b[k][2], _b[k][1], _b[k][0]);
					row = _mm256_add_ps(row, _mm256_mul_ps(aValue, bRow));
				}
				alignas(32) float temp[8];
				_mm256_store_ps(temp, row);
				result[i][0] = temp[0];
				result[i][1] = temp[1];
				result[i][2] = temp[2];
			}
#else
			// Fallback version classique
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
#endif

			return result;
		}

		Mat3x3 InvertMatrix(const Mat3x3& _a)
		{
			Mat3x3 result = { 0 };

			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					result[i][j] = _a[j][i];
				}
			}

			return result;
		}

		sf::Vector3f MultiplyMatrixVector(const Mat3x3& _a, const sf::Vector3f& _b)
		{
			sf::Vector3f result;

#ifdef __AVX__
			// Chargement du vecteur dans un registre AVX
			__m256 vec = _mm256_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, _b.z, _b.y, _b.x);

			// Multiplication matrice-vecteur optimisée
			__m256 row0 = _mm256_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, _a[0][2], _a[0][1], _a[0][0]);
			__m256 row1 = _mm256_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, _a[1][2], _a[1][1], _a[1][0]);
			__m256 row2 = _mm256_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, _a[2][2], _a[2][1], _a[2][0]);

			__m256 mul0 = _mm256_mul_ps(row0, vec);
			__m256 mul1 = _mm256_mul_ps(row1, vec);
			__m256 mul2 = _mm256_mul_ps(row2, vec);

			// Somme horizontale
			__m256 sum0 = _mm256_hadd_ps(mul0, mul1);
			__m256 sum1 = _mm256_hadd_ps(mul2, _mm256_setzero_ps());
			__m256 sum = _mm256_hadd_ps(sum0, sum1);

			alignas(32) float output[8];
			_mm256_store_ps(output, sum);

			result.x = output[0];
			result.y = output[1];
			result.z = output[2];
#else
			// Classic version
			result.x = _a[0][0] * _b.x + _a[0][1] * _b.y + _a[0][2] * _b.z;
			result.y = _a[1][0] * _b.x + _a[1][1] * _b.y + _a[1][2] * _b.z;
			result.z = _a[2][0] * _b.x + _a[2][1] * _b.y + _a[2][2] * _b.z;
#endif

			return result;
		}

		Mat3x3 CreateRotationMatrix(const sf::Vector3f& _rotation)
		{
			const float radX = DegToRad(_rotation.x);
			const float radY = DegToRad(_rotation.y);
			const float radZ = DegToRad(_rotation.z);

			const float cX = std::cos(radX);
			const float sX = std::sin(radX);

			const float cY = std::cos(radY);
			const float sY = std::sin(radY);

			const float cZ = std::cos(radZ);
			const float sZ = std::sin(radZ);

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

			return MultiplyMatrix(rY, MultiplyMatrix(rX, rZ));
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

		Mat2x2 MultiplyMatrix(const Mat2x2& _a, const Mat2x2& _b)
		{
			Mat2x2 result = { 0 };

#ifdef __AVX__
			for (int i = 0; i < 2; ++i)
			{
				__m256 row = _mm256_setzero_ps();
				for (int k = 0; k < 2; ++k)
				{
					__m256 aValue = _mm256_set1_ps(_a[i][k]);
					__m256 bValue = _mm256_set_ps(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, _b[k][1], _b[k][0]);
					row = _mm256_add_ps(row, _mm256_mul_ps(aValue, bValue));
				}
				alignas(32) float temp[8];
				_mm256_store_ps(temp, row);
				result[i][0] = temp[0];
				result[i][1] = temp[1];
			}
#else
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
#endif

			return result;
		}

		Mat2x2 InvertMatrix(const Mat2x2& _a)
		{
			Mat2x2 result = { 0 };

			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					result[i][j] = _a[j][i];
				}
			}

			return result;
		}

		sf::Vector2f MultiplyMatrixVector(const Mat2x2& _a, const sf::Vector2f& _b)
		{
			sf::Vector2f result;

#ifdef __AVX__
			__m256 vec = _mm256_set_ps(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, _b.y, _b.x);

			__m256 row0 = _mm256_set_ps(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, _a[0][1], _a[0][0]);
			__m256 row1 = _mm256_set_ps(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, _a[1][1], _a[1][0]);

			// Multiply matrix-vector
			__m256 mul0 = _mm256_mul_ps(row0, vec);
			__m256 mul1 = _mm256_mul_ps(row1, vec);

			// Horizontal sum
			__m256 sum = _mm256_hadd_ps(mul0, mul1);

			alignas(32) float output[8];
			_mm256_store_ps(output, sum);
			result.x = output[0];
			result.y = output[2];
#else
			result.x = _a[0][0] * _b.x + _a[0][1] * _b.y;
			result.y = _a[1][0] * _b.x + _a[1][1] * _b.y;
#endif

			return result;
		}

		Mat2x2 CreateRotationMatrix(const float& _angle)
		{
			const float rad = DegToRad(_angle);

			const float c = std::cos(rad);
			const float s = std::sin(rad);

			Mat2x2 result = { {
				{c, -s},
				{s, c}
				} };

			return result;
		}

		float GetAreaOfVA(const sf::VertexArray& _va)
		{
			const size_t vertexCount = _va.getVertexCount();

			if (vertexCount < 3)
				return 0.f;

#ifdef __AVX__
			__m256 areaVec = _mm256_setzero_ps();

			// Traiter 4 sommets à la fois avec AVX
			size_t i = 0;
			for (; i + 3 < vertexCount; i += 4)
			{
				// Charger 4 paires de sommets
				alignas(32) float ax[8], ay[8], bx[8], by[8];

				for (size_t j = 0; j < 4; ++j)
				{
					const sf::Vector2f a = _va[i + j].position;
					const sf::Vector2f b = _va[(i + j + 1) % vertexCount].position;
					ax[j] = a.x;
					ay[j] = a.y;
					bx[j] = b.x;
					by[j] = b.y;
				}

				__m256 axVec = _mm256_set_ps(0.f, 0.f, 0.f, 0.f, ax[3], ax[2], ax[1], ax[0]);
				__m256 ayVec = _mm256_set_ps(0.f, 0.f, 0.f, 0.f, ay[3], ay[2], ay[1], ay[0]);
				__m256 bxVec = _mm256_set_ps(0.f, 0.f, 0.f, 0.f, bx[3], bx[2], bx[1], bx[0]);
				__m256 byVec = _mm256_set_ps(0.f, 0.f, 0.f, 0.f, by[3], by[2], by[1], by[0]);

				// Calcul: (a.x * b.y) - (b.x * a.y)
				__m256 term1 = _mm256_mul_ps(axVec, byVec);
				__m256 term2 = _mm256_mul_ps(bxVec, ayVec);
				__m256 diff = _mm256_sub_ps(term1, term2);

				areaVec = _mm256_add_ps(areaVec, diff);
			}

			// Réduction horizontale
			alignas(32) float results[8];
			_mm256_store_ps(results, areaVec);
			float area = results[0] + results[1] + results[2] + results[3];

			// Traiter les sommets restants
			for (; i < vertexCount; ++i)
			{
				const sf::Vector2f a = _va[i].position;
				const sf::Vector2f b = _va[(i + 1) % vertexCount].position;
				area += (a.x * b.y) - (b.x * a.y);
			}

			area *= 0.5f;
			return area;
#else
			// Fallback classique
			float area = 0.f;
			for (size_t i = 0; i < vertexCount; ++i)
			{
				const sf::Vector2f a = _va[i].position;
				const sf::Vector2f b = _va[(i + 1) % vertexCount].position;
				area += (a.x * b.y) - (b.x * a.y);
			}
			area *= 0.5f;
			return area;
#endif
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