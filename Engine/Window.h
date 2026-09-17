//
// Created by mdsamar on 17/09/26.
//

#ifndef MOMENTUM_WINDOW_H
#define MOMENTUM_WINDOW_H
#include <string>

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

    int GetWidth() const {return m_Width;};
    int GetHeight() const {return m_Height;};
    GLFWwindow* GetWindow() {return m_Window;};

private:
    void Init(const WindowProperties& properties);
    void Shutdown();

    GLFWwindow* m_Window;
    std::string m_WindowTitle;
    int m_Width;
    int m_Height;
};


#endif //MOMENTUM_WINDOW_H
