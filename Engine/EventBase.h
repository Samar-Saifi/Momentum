//
// Created by mdsamar on 19/09/26.
//

#ifndef MOMENTUM_EVENTBASE_H
#define MOMENTUM_EVENTBASE_H

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                               virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_GROUP(grp) virtual EventGroup GetGroup() const override { return grp; }
#include <string>

enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum class EventGroup {
    None = 0,
    Application,
    Keyboard,
    Mouse
};

class EventBase {
public:
    bool handled = false;

    ~EventBase() = default;

    virtual EventType GetEventType() const = 0;
    virtual EventGroup GetGroup() const = 0;
    virtual const char* GetName() const = 0;
};

class EventDispatcher {
private:
    EventBase& m_Event;

public:
    EventDispatcher(EventBase& event) : m_Event(event) {}

    template <typename T, typename F>
    bool Dispatch(const F& func) {
        if (m_Event.GetEventType() == T::GetStaticType()) {
            m_Event.handled |= func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }
};



#endif //MOMENTUM_EVENTBASE_H
