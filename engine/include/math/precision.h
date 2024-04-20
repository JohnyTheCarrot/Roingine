#ifndef PRECISION_H
#define PRECISION_H

namespace roingine::math {
#ifdef ROINGINE_MATH_PRECISION_SINGLE
	using FloatingPoint = float;
#define glTranslate glTranslatef
#define glRotate glRotatef
#else
	using FloatingPoint = double;
#define glTranslate glTranslated
#define glRotate glRotated
#endif

	constexpr FloatingPoint operator""_fp(long double value) {
		return static_cast<FloatingPoint>(value);
	}
}// namespace roingine::math

#endif// PRECISION_H
