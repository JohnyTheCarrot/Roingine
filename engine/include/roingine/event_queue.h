#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <array>
#include <functional>
#include <optional>
#include <roingine/Singleton.h>
#include <unordered_map>
#include <variant>

namespace roingine {
	template<typename EventType, EventType Specific>
	    requires std::is_enum_v<EventType>
	struct EventTypeData final {};

	template<typename EventQueue>
	class EventHandlerHandle final {
	public:
		EventHandlerHandle() = default;

		EventHandlerHandle(EventQueue *eventQueue, EventQueue::EventTypeEnum eventType, std::size_t hHandler)
		    : m_EventQueue{eventQueue}
		    , m_hHandler{hHandler}
		    , m_EventType{eventType} {};

		~EventHandlerHandle() {
			Destroy();
		}

		EventHandlerHandle(EventHandlerHandle &&other)
		    : m_EventQueue{std::move(other.m_EventQueue)}
		    , m_EventType{other.m_EventType} {
			Destroy();
			m_hHandler       = other.m_hHandler;
			other.m_hHandler = std::nullopt;
		}

		EventHandlerHandle &operator=(EventHandlerHandle &&other) {
			if (this == &other)
				return *this;

			Destroy();
			m_EventQueue     = std::move(other.m_EventQueue);
			m_hHandler       = other.m_hHandler;
			m_EventType      = other.m_EventType;
			other.m_hHandler = std::nullopt;

			return *this;
		}

		EventHandlerHandle(EventHandlerHandle const &other) = delete;

		EventHandlerHandle &operator=(EventHandlerHandle const &other) = delete;

	private:
		void Destroy() {
			if (m_hHandler.has_value())
				m_EventQueue->DetachEventHandler(m_EventType, m_hHandler.value());
		}

		EventQueue                *m_EventQueue{nullptr};
		std::optional<std::size_t> m_hHandler{std::nullopt};
		EventQueue::EventTypeEnum  m_EventType{};
	};

	template<typename EventType, class... EventDataTypes>
	    requires std::is_enum_v<EventType>
	class EventQueue final : public Singleton<EventQueue<EventType, EventDataTypes...>> {
	public:
		using EventTypeEnum    = EventType;
		using EventDataVariant = std::variant<EventDataTypes...>;
		using EventHandlers    = std::unordered_map<std::size_t, std::function<void(EventDataVariant const &)>>;

		using Self = EventQueue<EventType, EventDataTypes...>;

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
		[[nodiscard]]
		EventHandlerHandle<Self>
		AttachEventHandler(std::function<void(typename EventTypeData<EventType, EType>::Data_t const &)> &&handler) {
			auto genericHandler{[=](EventDataVariant const &eventData) {
				const auto &specificallyTypedEventData{
				        std::get<typename EventTypeData<EventType, EType>::Data_t>(eventData)
				};


				handler(specificallyTypedEventData);
			}};

			if (!m_EventHandlers.contains(EType))
				m_EventHandlers.emplace(EType, EventHandlers{});

			m_CurrentHandle++;

			m_EventHandlers.at(EType).emplace(m_CurrentHandle, genericHandler);

			return {this, EType, m_CurrentHandle};
		}

		void DetachEventHandler(EventType eType, std::size_t hHandler) {
			if (!m_EventHandlers.contains(eType))
				return;

			m_EventHandlers.at(eType).erase(hHandler);
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
		std::size_t                   m_Tail{0};
		std::size_t                   m_Head{0};
		std::size_t                   m_CurrentHandle{0};

		std::unordered_map<EventType, EventHandlers> m_EventHandlers;
	};
};// namespace roingine

#endif// EVENT_QUEUE_H
