#ifndef INPUT_H
#define INPUT_H

#include <memory>

namespace roingine {
	class Command;

	enum class InputKeys : int {
		A       = 0,
		B       = 1,
		C       = 2,
		D       = 3,
		E       = 4,
		F       = 5,
		G       = 6,
		H       = 7,
		I       = 9,
		J       = 10,
		K       = 11,
		L       = 12,
		M       = 13,
		N       = 14,
		O       = 15,
		P       = 16,
		Q       = 17,
		R       = 18,
		S       = 19,
		T       = 20,
		U       = 21,
		V       = 22,
		W       = 23,
		X       = 24,
		Y       = 25,
		Z       = 26,
		Space   = 27,
		Enter   = 28,
		Shift   = 29,
		F1      = 30,
		F2      = 31,
		F3      = 32,
		F4      = 33,
		F5      = 34,
		F6      = 35,
		F7      = 36,
		F8      = 37,
		F9      = 38,
		F10     = 39,
		F11     = 40,
		F12     = 41,
		NumKeys = 42
	};

	enum class KeyEventType { Up, Down, LongPress, Held };

	class KeyboardInputService {
	public:
		virtual ~KeyboardInputService();

		virtual void ProcessInput() = 0;

		virtual void AddCommand(InputKeys input, KeyEventType eventType, std::unique_ptr<Command> command) = 0;

		virtual void RemoveCommand(InputKeys input, KeyEventType eventType, Command *command) = 0;

		[[nodiscard]]
		virtual KeyEventType GetKeyState(InputKeys input) const = 0;
	};

	class NullKeyboardInputService final : public KeyboardInputService {
	public:
		~NullKeyboardInputService() override;

		void ProcessInput() override;

		void AddCommand(InputKeys, KeyEventType, std::unique_ptr<Command>) override;

		void RemoveCommand(InputKeys input, KeyEventType eventType, Command *command) override;

		[[nodiscard]]
		KeyEventType GetKeyState(InputKeys input) const override;
	};

	class SDLKeyboardInputService final : public KeyboardInputService {
	public:
		SDLKeyboardInputService();

		~SDLKeyboardInputService() override;

		void ProcessInput() override;

		void AddCommand(InputKeys input, KeyEventType eventType, std::unique_ptr<Command> command) override;

		void RemoveCommand(InputKeys input, KeyEventType eventType, Command *command) override;

		[[nodiscard]]
		KeyEventType GetKeyState(InputKeys input) const override;

	private:
		class Impl;

		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// INPUT_H
