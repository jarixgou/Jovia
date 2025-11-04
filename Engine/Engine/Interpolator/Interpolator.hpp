#ifndef INTERPOLATOR__HPP
#define INTERPOLATOR__HPP

namespace Engine
{
	enum class InterpolationType
	{
		LINEAR,
		QUADRATIC,
		CUBIC,
		SINE
	};

	template <enum InterpolationType>
	float Interpolate(float _t);

	template <typename T>
	T Lerp(T _start, T _end, float _t, InterpolationType _interpolationType = InterpolationType::LINEAR)
	{
		switch (_interpolationType) {
		case InterpolationType::LINEAR:
			_t = Interpolate<InterpolationType::LINEAR>(_t);
			break;
		case InterpolationType::QUADRATIC:
			_t = Interpolate<InterpolationType::QUADRATIC>(_t);
			break;
		case InterpolationType::CUBIC:
			_t = Interpolate<InterpolationType::CUBIC>(_t);
			break;
		case InterpolationType::SINE:
			_t = Interpolate<InterpolationType::SINE>(_t);
			break;
		default:
			// Cas par défaut
			break;
		}

		return _start + (_end - _start) * _t;
	}
}

#endif
