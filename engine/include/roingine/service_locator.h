#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H

template<class Service>
class ServiceLocator final {
public:
	static void Provide(Service *service) {
		m_Service = service;
	}

	[[nodiscard]]
	static Service &GetService() {
		return *m_Service;
	}

private:
	static inline Service *m_Service{};
};


#endif// SERVICE_LOCATOR_H
