#ifndef INPUT_INTERNAL_H
#define INPUT_INTERNAL_H

#include <SDL_scancode.h>
#include <roingine/input.h>
#include <unordered_map>

struct pair_hash final {
	// credit goes to https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp
	template<class T1, class T2>
	std::size_t operator()(std::pair<T1, T2> const &pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

namespace roingine {
	class SDLKeyboardInputService::Impl final {
	public:
		Impl();

		void ProcessInput();

		void AddCommand(InputKeys input, KeyEventType eventType, std::unique_ptr<Command> command);

		void RemoveCommand(InputKeys input, KeyEventType eventType, Command *command);

	private:
		enum class SDLKey : int {
			A     = SDL_SCANCODE_A,
			B     = SDL_SCANCODE_B,
			C     = SDL_SCANCODE_C,
			D     = SDL_SCANCODE_D,
			E     = SDL_SCANCODE_E,
			F     = SDL_SCANCODE_F,
			G     = SDL_SCANCODE_G,
			H     = SDL_SCANCODE_H,
			I     = SDL_SCANCODE_I,
			J     = SDL_SCANCODE_J,
			K     = SDL_SCANCODE_K,
			L     = SDL_SCANCODE_L,
			M     = SDL_SCANCODE_M,
			N     = SDL_SCANCODE_N,
			O     = SDL_SCANCODE_O,
			P     = SDL_SCANCODE_P,
			Q     = SDL_SCANCODE_Q,
			R     = SDL_SCANCODE_R,
			S     = SDL_SCANCODE_S,
			T     = SDL_SCANCODE_T,
			U     = SDL_SCANCODE_U,
			V     = SDL_SCANCODE_V,
			W     = SDL_SCANCODE_W,
			X     = SDL_SCANCODE_X,
			Y     = SDL_SCANCODE_Y,
			Z     = SDL_SCANCODE_Z,
			Space = SDL_SCANCODE_SPACE,
			Enter = SDL_SCANCODE_RETURN,
			Shift = SDL_SCANCODE_LSHIFT
		};
		using Commands = std::unordered_multimap<std::pair<SDLKey, KeyEventType>, std::unique_ptr<Command>, pair_hash>;

		[[nodiscard]]
		constexpr SDLKey GetSDLKeyFromInputKey(InputKeys inputKey);

		// TODO: allocator
		Commands           m_Commands;
		std::vector<bool>  m_KeyStates;
		std::vector<float> m_KeyHeldTimes;
		uint8_t const     *m_pKeyData;
		int                m_NumKeys;
	};
}// namespace roingine

#endif// INPUT_INTERNAL_H
