#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/mat3x3.hpp>
#include <roingine/components/component.h>

namespace roingine {
	class Transform final : public Component {
	public:
		Transform(GameObject *pGameObject, glm::vec2 position, float rotation);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		void Rotate(float angle) noexcept;

		void Translate(glm::vec2 translation) noexcept;

		void Translate(float x, float y) noexcept;

		[[nodiscard]]
		glm::vec2 GetLocalPosition() const noexcept;

		[[nodiscard]]
		glm::vec2 GetWorldPosition() const noexcept;

		[[nodiscard]]

		[[nodiscard]]
		float GetRotation() const noexcept;

		[[nodiscard]]
		glm::vec2 GetPivot() const noexcept;

		void SetPivot(glm::vec2 pivot) noexcept;

		void SetPivot(float x, float y) noexcept;

		void SetParent(Transform *pParent);

		[[nodiscard]]
		Transform *GetParent() const noexcept;

		[[nodiscard]]
		glm::mat4 GetTransformationMatrix() const noexcept;

	private:
		Transform          *m_pParent{nullptr};
		glm::vec2           m_Position{};
		glm::vec2           m_Pivot{};
		float               m_Rotation{};
	};

	class TransformContext final {
	public:
		TransformContext(Transform const &) noexcept;
		~TransformContext() noexcept;

		TransformContext(TransformContext const &)            = delete;
		TransformContext(TransformContext &&)                 = delete;
		TransformContext &operator=(TransformContext const &) = delete;
		TransformContext &operator=(TransformContext &&)      = delete;
	};
}// namespace roingine

#endif// TRANSFORM_H
