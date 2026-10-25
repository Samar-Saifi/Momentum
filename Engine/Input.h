//
// Created by mdsamar on 11/10/26.
//

#ifndef MOMENTUM_INPUT_H
#define MOMENTUM_INPUT_H
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>

#include "EventBase.h"
#include "glm/vec2.hpp"


enum class InputEventType {Pressed, Released, Repeat};

struct Action {
    int keyCode = 0;
    std::string context = "Gameplay";
};

struct ActionCallbackEntry {
    std::string action;
    InputEventType eventType;
    std::function<void()> callback;
};

class Input {
public:
    static void BindKey(const std::string& action, int keyCode, std::string context = "Gameplay");
    static const std::unordered_map<std::string, Action>& GetBindings() {return s_Bindings;}

    static void BindAction(const std::string& action, InputEventType eventType, std::function<void()> callback);

    static bool KeyPressed(int key);
    static bool ActionPressed(const std::string& action);
    static bool MouseButtonPressed(int button);
    static glm::vec2 GetMousePosition();

    static void SetContext(const std::string& context) {s_InputContext = context;};
    static const std::string& GetContext() {return s_InputContext;}

    static void OnEvent(EventBase& e);

private:

    static inline std::string s_InputContext = "Gameplay";
    static std::unordered_map<std::string, Action> s_Bindings;
    static inline std::vector<ActionCallbackEntry> s_ActionCallbacks;
};


#endif //MOMENTUM_INPUT_H
