#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H

#include <memory>
#include <roingine/singleton.h>

namespace roingine {
	class NoServiceProvidedException final : public std::exception {
	public:
		char const *what() const noexcept {
			return "No service provided";
		}
	};

	template<class Service>
	class ServiceLocator final {
	public:
		static void Provide(std::unique_ptr<Service> &&service) {
			m_Service = std::move(service);
		}

		[[nodiscard]]
		static Service &GetService() {
			if (m_Service == nullptr)
				throw NoServiceProvidedException{};

			return *m_Service;
		}

	private:
		static inline std::unique_ptr<Service> m_Service{};
	};
}// namespace roingine


#endif// SERVICE_LOCATOR_H
