#ifndef MOMENTUM_APPLICATION_H
#define MOMENTUM_APPLICATION_H
#include <memory>
#include <string>

#include "EventBase.h"
#include "Events/ApplicationEvents.h"

class Window;

class Application {
public:
    Application(const std::string& name = "Momentum");
    virtual ~Application();

    void Run();
    void Quit();

    virtual void OnEvent(EventBase& event);

    static Application& GetApplication() {return *s_Instance;}
    Window& GetWindow() { return *m_Window; }


protected:
    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnShutdown() {}

private:
    std::unique_ptr<Window> m_Window;
    static Application* s_Instance;
    bool m_Running = true;
    bool m_Minimized = false;
    float m_LastFrameTime = 0.0f;

    bool OnWindowResize(WindowResizeEvent& e);
    bool OnWindowClose(WindowCloseEvent& e);
};


#endif //MOMENTUM_APPLICATION_H
