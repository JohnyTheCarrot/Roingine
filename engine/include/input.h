#ifndef INPUT_H
#define INPUT_H

#include "singleton.h"
#include <memory>

namespace roingine {
	class Input final : public Singleton<Input> {
	public:
	private:
		// Allowing Engine to access the Impl directly to enable use of internal functions.
		friend class Engine;

		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// INPUT_H
