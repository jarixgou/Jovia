#include "Interpolator.hpp"

#include <algorithm>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>


template<>
float Engine::Interpolate<Engine::InterpolationType::LINEAR>(float _t)
{
	return std::clamp(_t, 0.f, 1.f);
}

template<>
float Engine::Interpolate<Engine::InterpolationType::QUADRATIC>(float _t)
{
	_t = std::clamp(_t, 0.f, 1.f);
	return _t * _t;
}

template<>
float Engine::Interpolate<Engine::InterpolationType::CUBIC>(float _t)
{
	_t = std::clamp(_t, 0.f, 1.f);
	return _t * _t * (3.0f - 2.0f * _t);
}

template<>
float Engine::Interpolate<Engine::InterpolationType::SINE>(float _t)
{
	_t = std::clamp(_t, 0.f, 1.f);
	return (1.0f - std::cosf(_t * M_PI)) * 0.5f;
}