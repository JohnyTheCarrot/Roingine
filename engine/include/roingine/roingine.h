#ifndef ROINGINE_H
#define ROINGINE_H

#include <functional>
#include <memory>
#include <optional>
#include <roingine/keyboard_input.h>
#include <roingine/service_locator.h>
#include <string_view>

namespace roingine {
	using KeyboardInput = ServiceLocator<KeyboardInputService>;

	class Engine final {
	public:
		 Engine(std::string_view windowTitle, int windowWidth, int windowHeight,
		        std::optional<int> windowX = std::nullopt, std::optional<int> windowY = std::nullopt);
		~Engine();

		Engine(Engine &&) noexcept;

		Engine &operator=(Engine &&) noexcept;

		void Run(std::function<void()> const &fn);

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif
