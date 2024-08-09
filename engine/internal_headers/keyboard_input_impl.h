#ifndef INPUT_INTERNAL_H
#define INPUT_INTERNAL_H

#include <SDL_scancode.h>
#include <roingine/keyboard_input.h>
#include <roingine/pair_hash.h>
#include <unordered_map>

namespace roingine {
	class SDLKeyboardInputService::Impl final {
	public:
		Impl();

		void ProcessInput();

		void AddCommand(InputKeys input, KeyEventType eventType, std::unique_ptr<Command> command);

		void RemoveCommand(InputKeys input, KeyEventType eventType, Command *command);

		[[nodiscard]]
		KeyEventType GetKeyState(InputKeys input) const;

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
			Shift = SDL_SCANCODE_LSHIFT,
			F1    = SDL_SCANCODE_F1,
			F2    = SDL_SCANCODE_F2,
			F3    = SDL_SCANCODE_F3,
			F4    = SDL_SCANCODE_F4,
			F5    = SDL_SCANCODE_F5,
			F6    = SDL_SCANCODE_F6,
			F7    = SDL_SCANCODE_F7,
			F8    = SDL_SCANCODE_F8,
			F9    = SDL_SCANCODE_F9,
			F10   = SDL_SCANCODE_F10,
			F11   = SDL_SCANCODE_F11,
			F12   = SDL_SCANCODE_F12,
		};

		using Commands = std::unordered_multimap<std::pair<SDLKey, KeyEventType>, std::unique_ptr<Command>, PairHash>;

		[[nodiscard]]
		static constexpr SDLKey GetSDLKeyFromInputKey(InputKeys inputKey);

		Commands           m_Commands;
		int                m_NumKeys{};
		uint8_t const     *m_pKeyData;
		std::vector<bool>  m_KeyStates;
		std::vector<float> m_KeyHeldTimes;
	};
}// namespace roingine

#endif// INPUT_INTERNAL_H
