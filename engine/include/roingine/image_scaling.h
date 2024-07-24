#ifndef IMAGE_SCALING_H
#define IMAGE_SCALING_H

namespace roingine {
	enum class ScalingMethod { NearestNeighbor, Linear };

	namespace scaling_method {
		int ToOpenGLScalingMethod(ScalingMethod scalingMethod) noexcept;
	}
}// namespace roingine

#endif//IMAGE_SCALING_H
