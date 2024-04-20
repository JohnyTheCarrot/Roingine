#ifndef GAME_TIME_H
#define GAME_TIME_H

#include "singleton.h"
#include <memory>

namespace roingine {
	class GameTime final : public Singleton<GameTime> {
	public:
		using DurationPrecision = double;

		GameTime();
		~GameTime();

		[[nodiscard]]
		DurationPrecision GetDeltaTime() const noexcept;

	private:
		// We're letting Engine access the implementation directly as this public API does not expose some of the
		// methods that the engine uses internally.
		friend class Engine;

		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// GAME_TIME_H
