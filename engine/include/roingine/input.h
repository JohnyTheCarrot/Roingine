#ifndef INPUT_H
#define INPUT_H

#include <memory>

namespace roingine {
	class Command;

	enum class InputKeys : int {
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Space,
		Enter,
		Shift,
	};

	enum class KeyEventType { Up, Down, LongPress, Held };

	class KeyboardInputService {
	public:
		virtual ~KeyboardInputService() = default;

		virtual void ProcessInput() = 0;

		virtual void AddCommand(InputKeys input, KeyEventType eventType, std::unique_ptr<Command> command) = 0;

		virtual void RemoveCommand(InputKeys input, KeyEventType eventType, Command *command) = 0;
	};

	class NullKeyboardInputService final : public KeyboardInputService {
	public:
		~NullKeyboardInputService() override = default;

		void ProcessInput() override {
		}

		void AddCommand(InputKeys, KeyEventType, std::unique_ptr<Command>) override;

		void RemoveCommand(InputKeys input, KeyEventType eventType, Command *command) override;
	};

	class SDLKeyboardInputService final : public KeyboardInputService {
	public:
		SDLKeyboardInputService();

		~SDLKeyboardInputService() override;

		void ProcessInput() override;

		void AddCommand(InputKeys input, KeyEventType eventType, std::unique_ptr<Command> command) override;

		void RemoveCommand(InputKeys input, KeyEventType eventType, Command *command) override;

	private:
		class Impl;

		std::unique_ptr<Impl> m_pImpl;
	};

}// namespace roingine

#endif// INPUT_H
