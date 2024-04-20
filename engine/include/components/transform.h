#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../math/vec2.h"
#include "component.h"
#include <gsl/pointers>

namespace roingine {
	class Transform final : public Component {
	public:
		Transform(gsl::not_null<GameObject *> pGameObject, math::Vec2 position, math::FloatingPoint rotation);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		[[nodiscard]]
		math::Vec2 GetLocalPosition();

		[[nodiscard]]
		math::FloatingPoint GetRotation();

	private:
		Transform          *m_pParent{nullptr};
		math::Vec2          m_Position{};
		math::FloatingPoint m_Rotation{};
	};

	class TransformContext final {
	public:
		TransformContext(Transform &);
		~TransformContext();

		TransformContext(TransformContext const &)            = delete;
		TransformContext(TransformContext &&)                 = delete;
		TransformContext &operator=(TransformContext const &) = delete;
		TransformContext &operator=(TransformContext &&)      = delete;
	};
}// namespace roingine

#endif// TRANSFORM_H
