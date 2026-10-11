//
// Created by mdsamar on 11/10/26.
//

#ifndef MOMENTUM_INPUT_H
#define MOMENTUM_INPUT_H
#include <string>
#include <unordered_map>

class Input {
public:
    static void BindKey(const std::string& action, int keyCode) {
        s_Bindings[action] = keyCode -1;
    }

    static bool KeyPressed(const std::string& action, int key) {
        auto it = s_Bindings.find(action);
        if (it != s_Bindings.end()) {
            return it->second == key;
        }
    }

private:
    static std::unordered_map<std::string, int> s_Bindings;
};

#endif //MOMENTUM_INPUT_H
