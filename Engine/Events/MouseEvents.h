//
// Created by mdsamar on 20/09/26.
//

#ifndef MOMENTUM_MOUSEEVENTS_H
#define MOMENTUM_MOUSEEVENTS_H
#include "EventBase.h"

class MouseMovedEvent : public EventBase {
private:
    float m_PosX, m_PosY;

public:
    MouseMovedEvent(float x, float y) : m_PosX(x), m_PosY(y) {};
    float GetX() const { return m_PosX; }
    float GetY() const { return m_PosY; }
    EVENT_CLASS_GROUP(EventGroup::Mouse)
    EVENT_CLASS_TYPE(MouseMoved)
};

class MouseScrolledEvent : public EventBase {
private:
    float m_OffsetX, m_OffsetY;

public:
    MouseScrolledEvent(float x, float y) : m_OffsetX(x), m_OffsetY(y) {};
    float GetXOffset() const { return m_OffsetX; }
    float GetYOffset() const { return m_OffsetY; }
    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_GROUP(EventGroup::Mouse)
};

class MouseButtonEvent : public EventBase {
private:
    int m_Button;

public:
    MouseButtonEvent(int button) : m_Button(button) {};
    int GetMouseButton() const { return m_Button; }
    EVENT_CLASS_GROUP(EventGroup::Mouse)
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {};
    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {};
    EVENT_CLASS_TYPE(MouseButtonReleased)
};

#endif //MOMENTUM_MOUSEEVENTS_H
