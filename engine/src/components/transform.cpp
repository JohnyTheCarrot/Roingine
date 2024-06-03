#include <SDL_opengl.h>
#include <cmath>
#include <duktape.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numbers>
#include <roingine/component_init_types.h>
#include <roingine/components/transform.h>
#include <roingine/game_time.h>

namespace roingine {
	Transform::Transform(GameObject &gameObject, glm::vec2 position, float rotation)
	    : Component{gameObject}
	    , m_Position{position}
	    , m_Rotation{rotation} {
	}

	void Transform::Update() {
	}

	void Transform::FixedUpdate() {
	}

	void Transform::Render() const {
	}

	void Transform::Rotate(float angle) noexcept {
		constexpr float MAX_ANGLE{360.0f};

		auto const rad{angle * (std::numbers::pi_v<float> / 180.f)};
		m_Rotation += rad * GameTime::GetInstance().GetDeltaTime();

		m_Rotation = m_Rotation - std::floor(m_Rotation / MAX_ANGLE) * MAX_ANGLE;
	}

	void Transform::Translate(glm::vec2 translation) noexcept {
		Translate(translation.x, translation.y);
	}

	void Transform::Translate(float x, float y) noexcept {
		m_Position.x += x;
		m_Position.y += y;
	}

	void Transform::SetLocalPosition(glm::vec2 position) {
		m_Position = position;
	}

	glm::vec2 Transform::GetLocalPosition() const noexcept {
		return m_Position;
	}

	glm::vec2 Transform::GetWorldPosition() const noexcept {
		auto const transformationMat{GetTransformationMatrix()};

		return glm::vec4{m_Position, 0.f, 1.f} * transformationMat;
	}

	float Transform::GetRotation() const noexcept {
		return m_Rotation;
	}

	glm::vec2 Transform::GetPivot() const noexcept {
		return m_Pivot;
	}

	void Transform::SetPivot(glm::vec2 pivot) noexcept {
		m_Pivot = pivot;
	}

	void Transform::SetPivot(float x, float y) noexcept {
		m_Pivot = {x, y};
	}

	void Transform::SetParent(Transform &parent) {
		m_pParent = &parent;
	}

	Transform &Transform::GetParent() const noexcept {
		return *m_pParent;
	}

	glm::mat4 Transform::GetTransformationMatrix() const noexcept {
		glm::mat4        transMat{1.f};
		Transform const *pCurrentTransform{this};

		do {
			glm::mat4        currentTransMat;
			glm::vec2 const &pivot{pCurrentTransform->GetPivot()};
			currentTransMat = glm::translate(glm::mat4{1.f}, glm::vec3{pCurrentTransform->GetLocalPosition(), 0.f});

			if (pivot != glm::vec2{0, 0})
				currentTransMat = glm::translate(currentTransMat, glm::vec3{pivot, 0.f});

			currentTransMat = glm::rotate(currentTransMat, GetRotation(), glm::vec3{0, 0, 1});

			if (pivot != glm::vec2{0, 0})
				currentTransMat = glm::translate(currentTransMat, glm::vec3{-pivot, 0.f});

			transMat = currentTransMat * transMat;
		} while ((pCurrentTransform = &pCurrentTransform->GetParent()) != nullptr);

		return transMat;
	}

	char const *Transform::GetName() const {
		return NAME;
	}

	int JsAPITranslate(duk_context *ctx) {
		float const x{static_cast<float>(duk_require_number(ctx, 0))};
		float const y{static_cast<float>(duk_require_number(ctx, 1))};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Transform *ptr{static_cast<Transform *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		ptr->Translate(x, y);

		return 0;
	}

	int JsAPISetLocalPosition(duk_context *ctx) {
		float const x{static_cast<float>(duk_require_number(ctx, 0))};
		float const y{static_cast<float>(duk_require_number(ctx, 1))};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Transform *ptr{static_cast<Transform *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		ptr->SetLocalPosition(glm::vec2{x, y});

		return 0;
	}

	int JsAPIGetWorldPosition(duk_context *ctx) {
		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Transform *ptr{static_cast<Transform *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		auto const worldPos{ptr->GetWorldPosition()};

		duk_push_object(ctx);
		duk_push_number(ctx, worldPos.x);
		duk_put_prop_literal(ctx, -2, "x");
		duk_push_number(ctx, worldPos.y);
		duk_put_prop_literal(ctx, -2, "y");

		return 1;
	}

	int JsAPIGetLocalPosition(duk_context *ctx) {
		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Transform *ptr{static_cast<Transform *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		auto const localPos{ptr->GetLocalPosition()};

		duk_push_object(ctx);
		duk_push_number(ctx, localPos.x);
		duk_put_prop_literal(ctx, -2, "x");
		duk_push_number(ctx, localPos.y);
		duk_put_prop_literal(ctx, -2, "y");

		return 1;
	}

	duk_function_list_entry const transformAPI[]{
	        {"translate", JsAPITranslate, 2},
	        {"setLocalPosition", JsAPISetLocalPosition, 2},
	        {"getWorldPosition", JsAPIGetWorldPosition, 0},
	        {"getLocalPosition", JsAPIGetLocalPosition, 0},
	        {nullptr, nullptr, 0}
	};

	duk_function_list_entry const *Transform::SetUpScriptAPI(duk_context *) const {
		return transformAPI;
	}

	std::unique_ptr<Transform>
	Transform::JSFactory(GameObject *pGameObject, std::vector<ComponentInitArgument> const &args) {
		auto const posX{comp_init::RequireDouble(0, args)};
		auto const posY{comp_init::RequireDouble(1, args)};

		return std::make_unique<Transform>(*pGameObject, glm::vec2{posX, posY}, 0.f);
	}

	TransformContext::TransformContext(Transform const &transform) noexcept {
		auto const transformationMat{transform.GetTransformationMatrix()};

		glLoadIdentity();
		glMultMatrixf(glm::value_ptr(transformationMat));
	}

	TransformContext::~TransformContext() noexcept {
		glPopMatrix();
	}
}// namespace roingine
