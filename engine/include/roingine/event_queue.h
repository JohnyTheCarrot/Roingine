#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <array>
#include <functional>
#include <roingine/Singleton.h>
#include <unordered_map>
#include <variant>

namespace roingine {
	// Note for person grading:
	// These events are not used as they use the observer pattern instead.
	// I didn't bother removing them because it's a good example for once I start using the event queue pattern.
	template<typename EventType, EventType Specific>
	    requires std::is_enum_v<EventType>
	struct EventTypeData final {};

	template<typename EventType, class... EventDataTypes>
	    requires std::is_enum_v<EventType>
	class EventQueue final : public Singleton<EventQueue<EventType, EventDataTypes...>> {
	public:
		using EventDataVariant = std::variant<EventDataTypes...>;
		using EventHandlers    = std::unordered_map<void const *, std::function<void(EventDataVariant const &)>>;

		struct Event final {
			EventType        type;
			EventDataVariant eventData;
		};

		template<EventType EType, typename... Args>
		    requires std::is_constructible_v<typename EventTypeData<EventType, EType>::Data_t, Args...>
		void FireEvent(Args &&...args) {
			using Data = typename EventTypeData<EventType, EType>::Data_t;

			m_Events[m_Tail] = {EType, Data{std::forward<Args>(args)...}};
			m_Tail           = (m_Tail + 1) % MAX_EVENTS;
		}

		template<EventType EType>
		void
		AttachEventHandler(std::function<void(typename EventTypeData<EventType, EType>::Data_t const &)> const &handler
		) {
			auto genericHandler{[=](EventDataVariant const &eventData) {
				const auto &specificallyTypedEventData{
				        std::get<typename EventTypeData<EventType, EType>::Data_t>(eventData)
				};


				handler(specificallyTypedEventData);
			}};

			if (!m_EventHandlers.contains(EType))
				m_EventHandlers.emplace(EType, EventHandlers{});

			auto pHandlerTarget{handler.target<void (*)(typename EventTypeData<EventType, EType>::Data_t const &)>()};
			m_EventHandlers.at(EType).emplace(reinterpret_cast<void const *>(pHandlerTarget), genericHandler);
		}

		template<EventType EType>
		void
		DetachEventHandler(std::function<void(typename EventTypeData<EventType, EType>::Data_t const &)> const &handler
		) {
			if (!m_EventHandlers.contains(EType))
				return;

			auto pHandlerTarget{handler.target<void (*)(typename EventTypeData<EventType, EType>::Data_t const &)>()};
			m_EventHandlers.at(EType).erase(reinterpret_cast<void const *>(pHandlerTarget));
		}

		void Update() {
			if (m_Head == m_Tail)
				return;

			auto const &event{m_Events[m_Head]};
			if (!m_EventHandlers.contains(event.type))
				return;

			auto const &handlers{m_EventHandlers.at(event.type)};

			for (auto const &[_, handler]: handlers) { handler(event.eventData); }

			m_Head = (m_Head + 1) % MAX_EVENTS;
		}

	private:
		static constexpr size_t       MAX_EVENTS{10};
		std::array<Event, MAX_EVENTS> m_Events;
		size_t                        m_Tail{0};
		size_t                        m_Head{0};

		std::unordered_map<EventType, EventHandlers> m_EventHandlers;
	};
};// namespace roingine

#endif// EVENT_QUEUE_H
