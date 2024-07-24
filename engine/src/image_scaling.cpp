#include <SDL_opengl.h>
#include <cassert>
#include <roingine/image_scaling.h>

namespace roingine::scaling_method {
	int ToOpenGLScalingMethod(ScalingMethod scalingMethod) noexcept {
		switch (scalingMethod) {
			case ScalingMethod::NearestNeighbor:
				return GL_NEAREST;
			case ScalingMethod::Linear:
				return GL_LINEAR;
		}

		assert(false);
		return GL_NEAREST;
	}
}// namespace roingine::scaling_method
