# RoingusEngine

Reads best with a markdown viewer, could just open it on GitHub.

Read the instructions in the terminal.
Don't forget to press a key once done reading.

Only thing that's missing is the high score thing.

If you want to speed up testing of the door and upgrade pick-ups, there's a commented out loop in the LevelFlyweight
constructor
that destroys all tiles immediately, so you don't need to find the exact ones.

## Git

https://github.com/JohnyTheCarrot/Roingine

## Specifics of the engine

### Entity Component System

I made the entity component system based off the component type.
By this I mean that you get/add a component by its type:

```cpp
// Get a component, throws if not found
auto &transform{entity.GetComponent<roingine::Transform>()};

// Optionally get a component, returns nullptr if not found
auto *transform{entity.GetOptionalComponent<roingine::Transform>()};

// Add a component, returns reference to newly added component, or existing one if already added
auto &transform{entity.AddComponent<roingine::Transform>(glm::vec2{0.f, 0.f}, rotation)};
```

As you might have noticed, AddComponent's parameters are forwarded to the component's constructor.

### Event Queue

I am particularly proud of this one.
The event queue is entirely type-safe:

In the following code I will demonstrate how to create an event queue, as well as an event, add it to the queue, and
then process it.

```cpp
// Set up enum of event types
enum class EventType
{
    BingusEvent,
    BongusEvent
};

// Specify the data for each event type
template<>
struct roingine::EventTypeData<EventType, EventType::BingusEvent>
        final {
	struct Data_t final {
        float schmooberness;
        int meaningOfLife;
        bool bingus;
	};
};
// Similar for BongusEvent

// Set up aliases to the data structures
using BingusEventData = roingine::EventTypeData<EventType, EventType::BingusEvent>::Data_t;
using BongusEventData = roingine::EventTypeData<EventType, EventType::BongusEvent>::Data_t;

// Set up event queue, first goes the event type, then the event data structures.
using EventQueue = roingine::EventQueue<EventType, BingusEventData, BongusEventData>;

// Exciting part: fire off an event!
// The FireEvent function parameters are forwarded to the constructor of the matching event type in an entirely type-safe manner.
EventQueue::GetInstance().FireEvent<EventType::BingusEvent>(1.f, 42, true);

// Handling events
// Also exciting! The handlers are RAII, so you can't forget to unregister them.
// This one's also type safe in that you can't accidentally take another event's data structure as a parameter.

roingine::EventHandlerHandle<EventQueue> handler{
    EventQueue::GetInstance()
        .AttachEventHandler<EventType::BingusEvent>(
            [](BingusEventData const &data) {
                // Do something with the data
            }
        )
};
// When handler goes out of scope, it unregisters itself.
```

### GameObject

The GameObject class is a simple handle, really.
It's why I pass it by value most times.
Its contents are a pointer to the scene it's in, and an index to the object in the scene's list of objects.

### Components

The components are stored in the scene class.
They are stored in an unordered_map, with the key being the gameobject handle, and the value being another unordered_map
with the key being the component type hash, and the value being the component itself.
This allows for quick access to components by their type.