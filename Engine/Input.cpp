//
// Created by mdsamar on 24/10/26.
//

#include "Input.h"

#include "Application.h"
#include "Window.h"
#include "Events/KeyboardEvents.h"
#include "glm/vec2.hpp"

std::unordered_map<std::string, Action> Input::s_Bindings;

void Input::BindKey(const std::string &action, int keyCode, std::string context) {
    s_Bindings[action] = {keyCode, context};
}

void Input::BindAction(const std::string &action, InputEventType eventType, std::function<void()> callback) {
    s_ActionCallbacks.push_back({action, eventType, callback});
}

bool Input::ActionPressed(const std::string &action) {
    auto it = s_Bindings.find(action);
    if (it == s_Bindings.end()) return false;

    if (it->second.context != s_InputContext && it->second.context != "Default" && it->second.context != "*") {
        return false;
    }

    return KeyPressed(it->second.keyCode);
}

bool Input::KeyPressed(int key) {
    auto* window = Application::GetApplication().GetWindow().GetWindow();
    if (!window) return false;
    int state = glfwGetKey(window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::MouseButtonPressed(int button) {
    auto window = Application::GetApplication().GetWindow().GetWindow();
    if (!window) return false;

    int state = glfwGetKey(window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition() {
    auto* window = Application::GetApplication().GetWindow().GetWindow();
    if (!window) return glm::vec2(0.0f);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return glm::vec2(x, y);
}

void Input::OnEvent(EventBase &e) {
    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<KeyPressedEvent>([](KeyPressedEvent& event) {
        int key = event.GetKeyCode();
        InputEventType eventType = event.GetRepeatCount() > 0 ? InputEventType::Repeat : InputEventType::Pressed;

        for (const auto& [action, binding] : s_Bindings) {
            if (binding.keyCode == key) {
                if (binding.context == s_InputContext || binding.context == "*" || binding.context == "Default") {
                    for (const auto& entry : s_ActionCallbacks) {
                        if (entry.action == action && (entry.eventType == eventType || (eventType == InputEventType::Repeat && entry.eventType == InputEventType::Pressed))) {
                            if (entry.callback) entry.callback();
                        }
                    }
                }
            }
        }
        return false;
    });

    dispatcher.Dispatch<KeyReleasedEvent>([](KeyReleasedEvent& event) {
       int key = event.GetKeyCode();
        for (const auto& [action, binding] : s_Bindings) {
            if (binding.keyCode == key) {
                if (binding.context == s_InputContext || binding.context == "Default" || binding.context == "*") {
                    for (const auto& entry : s_ActionCallbacks) {
                        if (entry.action == action && entry.eventType == InputEventType::Released) {
                            if (entry.callback) entry.callback();
                        }
                    }
                }
            }
        }
        return false;
    });
}
