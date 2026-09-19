//
// Created by mdsamar on 20/09/26.
//

#ifndef MOMENTUM_KEYBOARDEVENTS_H
#define MOMENTUM_KEYBOARDEVENTS_H
#include "EventBase.h"

class KeyboardEvent : public EventBase {
protected:
    int m_KeyCode;
    KeyboardEvent(int keyCode) : m_KeyCode(keyCode) {};

public:
    int GetKeyCode() const { return m_KeyCode; }
    EVENT_CLASS_GROUP(EventGroup::Keyboard)
};

class KeyPressedEvent : public KeyboardEvent {
private:
    int m_RepeatCount;

public:
    KeyPressedEvent(int keyCode, int repeatCount) : KeyboardEvent(keyCode), m_RepeatCount(repeatCount) {};
    int GetRepeatCount() const { return m_RepeatCount; }
    EVENT_CLASS_TYPE(KeyPressed);
};

class KeyReleasedEvent : public KeyboardEvent {
public:
    KeyReleasedEvent(int keyCode) : KeyboardEvent(keyCode) {};
    EVENT_CLASS_TYPE(KeyReleased);
};

class KeyTypedEvent : public KeyboardEvent {
public:
    KeyTypedEvent(int keyCode) : KeyboardEvent(keyCode) {};
    EVENT_CLASS_TYPE(KeyTyped);
};

#endif //MOMENTUM_KEYBOARDEVENTS_H
