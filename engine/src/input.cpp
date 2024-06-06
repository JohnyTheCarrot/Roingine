#include <SDL_events.h>
#include <algorithm>
#include <input_impl.h>
#include <roingine/commands/command.h>
#include <roingine/game_info.h>
#include <roingine/game_time.h>


namespace roingine {
	SDLKeyboardInputService::Impl::Impl()
	    : m_pKeyData{SDL_GetKeyboardState(&m_NumKeys)} {
		m_KeyStates.resize(m_NumKeys);
		m_KeyHeldTimes.resize(m_NumKeys);
	};

	void SDLKeyboardInputService::Impl::ProcessInput() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				GameInfo::GetInstance().QuitGame();
		}

		SDL_PumpEvents();

		auto const executeCommands{[&](size_t keyStateIdx, KeyEventType eventType) {
			SDLKey const sdlKey{static_cast<SDLKey>(keyStateIdx)};

			auto const [first, last] = m_Commands.equal_range(std::pair{sdlKey, eventType});

			std::for_each(first, last, [](auto const &pair) {
				auto const &[_, command] = pair;
				command->Execute();
			});
		}};

		for (std::size_t keyStateIdx = 0; keyStateIdx < m_KeyStates.size(); ++keyStateIdx) {
			bool const newState{m_pKeyData[keyStateIdx] != 0};
			bool const currentState{m_KeyStates[keyStateIdx]};

			bool const isPressed{!currentState && newState};
			bool const isReleased{currentState && !newState};
			bool const isHeld{currentState && newState};
			m_KeyStates[keyStateIdx] = newState;

			KeyEventType eventType;
			if (isHeld) {
				eventType = KeyEventType::Held;
				m_KeyHeldTimes[keyStateIdx] += GameTime::GetInstance().GetDeltaTime();

				if (m_KeyHeldTimes[keyStateIdx] >= GameInfo::GetInstance().GetKeyHeldThresholdMs())
					executeCommands(keyStateIdx, KeyEventType::LongPress);
			} else {
				m_KeyHeldTimes[keyStateIdx] = 0.f;

				if (isPressed)
					eventType = KeyEventType::Down;
				else if (isReleased)
					eventType = KeyEventType::Up;
				else
					continue;
			}

			executeCommands(keyStateIdx, eventType);
		}
	}

	void SDLKeyboardInputService::Impl::AddCommand(
	        InputKeys input, KeyEventType eventType, std::unique_ptr<Command> command
	) {
		SDLKey const sdlKey{GetSDLKeyFromInputKey(input)};
		m_Commands.emplace(std::pair{sdlKey, eventType}, std::move(command));
	}

	void SDLKeyboardInputService::Impl::RemoveCommand(InputKeys input, KeyEventType eventType, Command *command) {
		SDLKey const sdlKey{GetSDLKeyFromInputKey(input)};
		auto [start, end]{m_Commands.equal_range(std::pair{sdlKey, eventType})};

		if (start == m_Commands.end())
			return;

		auto toEraseIt{std::find_if(start, end, [command](auto &pair) {
			//
			return pair.second.get() == command;
		})};
		if (toEraseIt == m_Commands.end())
			return;

		m_Commands.erase(toEraseIt);
	}

	constexpr SDLKeyboardInputService::Impl::SDLKey
	SDLKeyboardInputService::Impl::GetSDLKeyFromInputKey(InputKeys inputKey) {
		switch (inputKey) {
			case InputKeys::A:
				return SDLKey::A;
			case InputKeys::B:
				return SDLKey::B;
			case InputKeys::C:
				return SDLKey::C;
			case InputKeys::D:
				return SDLKey::D;
			case InputKeys::E:
				return SDLKey::E;
			case InputKeys::F:
				return SDLKey::F;
			case InputKeys::G:
				return SDLKey::G;
			case InputKeys::H:
				return SDLKey::H;
			case InputKeys::I:
				return SDLKey::I;
			case InputKeys::J:
				return SDLKey::J;
			case InputKeys::K:
				return SDLKey::K;
			case InputKeys::L:
				return SDLKey::L;
			case InputKeys::M:
				return SDLKey::M;
			case InputKeys::N:
				return SDLKey::N;
			case InputKeys::O:
				return SDLKey::O;
			case InputKeys::P:
				return SDLKey::P;
			case InputKeys::Q:
				return SDLKey::Q;
			case InputKeys::R:
				return SDLKey::R;
			case InputKeys::S:
				return SDLKey::S;
			case InputKeys::T:
				return SDLKey::T;
			case InputKeys::U:
				return SDLKey::U;
			case InputKeys::V:
				return SDLKey::V;
			case InputKeys::W:
				return SDLKey::W;
			case InputKeys::X:
				return SDLKey::X;
			case InputKeys::Y:
				return SDLKey::Y;
			case InputKeys::Z:
				return SDLKey::Z;
			case InputKeys::Space:
				return SDLKey::Space;
			case InputKeys::Enter:
				return SDLKey::Enter;
			case InputKeys::Shift:
				return SDLKey::Shift;
			case InputKeys::F1:
				return SDLKey::F1;
			case InputKeys::F2:
				return SDLKey::F2;
			case InputKeys::F3:
				return SDLKey::F3;
			case InputKeys::F4:
				return SDLKey::F4;
			case InputKeys::F5:
				return SDLKey::F5;
			case InputKeys::F6:
				return SDLKey::F6;
			case InputKeys::F7:
				return SDLKey::F7;
			case InputKeys::F8:
				return SDLKey::F8;
			case InputKeys::F9:
				return SDLKey::F9;
			case InputKeys::F10:
				return SDLKey::F10;
			case InputKeys::F11:
				return SDLKey::F11;
			case InputKeys::F12:
				return SDLKey::F12;
			default:
				return SDLKey::A;
		}
	}

	SDLKeyboardInputService::SDLKeyboardInputService()
	    : m_pImpl{std::make_unique<Impl>()} {
	}

	void SDLKeyboardInputService::ProcessInput() {
		m_pImpl->ProcessInput();
	}

	SDLKeyboardInputService::~SDLKeyboardInputService() = default;

	void
	SDLKeyboardInputService::AddCommand(InputKeys input, KeyEventType eventType, std::unique_ptr<Command> command) {
		m_pImpl->AddCommand(input, eventType, std::move(command));
	}

	void SDLKeyboardInputService::RemoveCommand(InputKeys input, KeyEventType eventType, Command *command) {
		m_pImpl->RemoveCommand(input, eventType, command);
	}

	void NullKeyboardInputService::AddCommand(InputKeys, KeyEventType, std::unique_ptr<Command>) {
	}

	void NullKeyboardInputService::RemoveCommand(InputKeys, KeyEventType, Command *) {
	}

}// namespace roingine
