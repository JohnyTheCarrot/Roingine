#ifndef ROINGINE_H
#define ROINGINE_H

#include <memory>
#include <optional>
#include <string_view>

namespace roingine {
	class Engine final {
	public:
		struct Settings final {
			std::string_view   gameTitle;
			std::optional<int> windowPosX{std::nullopt}, windowPosY{std::nullopt};
			int                windowWidth, windowHeight;
		};

		Engine(Settings const &);
		~Engine();

		Engine(Engine &&);
		Engine &operator=(Engine &&);

		void Run();

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif
