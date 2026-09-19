//
// Created by mdsamar on 20/09/26.
//

#ifndef MOMENTUM_APPLICATIONEVENTS_H
#define MOMENTUM_APPLICATIONEVENTS_H
#include <sstream>

#include "EventBase.h"

class WindowResizeEvent: public EventBase {
private:
    int m_Width; int m_Height;

public:
    WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {};
    int GetWidth() const { return m_Width; } int GetHeight() const { return m_Height; }
    EVENT_CLASS_TYPE(WindowResize);
    EVENT_CLASS_GROUP(EventGroup::Application);
};

class WindowCloseEvent: public EventBase {
public:
    WindowCloseEvent() = default;
    EVENT_CLASS_TYPE(WindowClose);
    EVENT_CLASS_GROUP(EventGroup::Application);
};

#endif //MOMENTUM_APPLICATIONEVENTS_H
