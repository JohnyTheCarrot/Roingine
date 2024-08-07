#ifndef COMMAND_H
#define COMMAND_H

namespace roingine {
	class Command {
	public:
		virtual ~Command()     = default;
		virtual void Execute() const = 0;
	};
}// namespace roingine

#endif// COMMAND_H
