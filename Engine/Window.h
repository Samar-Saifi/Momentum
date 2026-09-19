//
// Created by mdsamar on 17/09/26.
//

#ifndef MOMENTUM_WINDOW_H
#define MOMENTUM_WINDOW_H
#include <functional>
#include <string>

#include "EventBase.h"

struct GLFWwindow;

struct WindowProperties {
    std::string title;
    int width;
    int height;

    WindowProperties(const std::string& title = "Momentum Engine", int width = 1280, int height = 720) : title(title), width(width), height(height) {}
};

class Window {
public:
    Window(const WindowProperties& properties = WindowProperties());
    ~Window();

    void OnUpdate();

    int GetWidth() const {return m_Data.width;}
    int GetHeight() const {return m_Data.height;}
    GLFWwindow* GetWindow() const {return m_Window;}

    void SetEventCallback(const std::function<void(EventBase&)>& callback){m_Data.eventCallback = callback;};

private:
    void Init(const WindowProperties& properties);
    void Shutdown();

    GLFWwindow* m_Window = nullptr;
    struct  WindowData {
        std::string title;
        int width;
        int height;
        std::function<void(EventBase&)> eventCallback;
    };

    WindowData m_Data;
};


#endif //MOMENTUM_WINDOW_H
