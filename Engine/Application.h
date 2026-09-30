#ifndef MOMENTUM_APPLICATION_H
#define MOMENTUM_APPLICATION_H
#include <memory>
#include <string>

#include "DeltaTime.h"
#include "EventBase.h"
#include "ImGuiManager.h"
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
    Window& GetWindow() const { return *m_Window; }
    float GetFixedDeltaTime() const { return m_FixedDeltaTime; }
    void SetFixedDeltaTime(float dt) {m_FixedDeltaTime = dt;}
    ImGuiManager& GetImGuiManager() const { return *m_ImGuiManager.get(); }

protected:
    virtual void OnStart() {}
    virtual void OnProcessInput() {}
    virtual void OnFixedUpdate(float fixedDeltaTime) {}
    virtual void OnUpdate(DeltaTime dt) {}
    virtual void OnRender() {}
    virtual void OnRenderImGui() {}
    virtual void OnShutdown() {}

private:
    std::unique_ptr<ImGuiManager> m_ImGuiManager;
    std::unique_ptr<Window> m_Window;
    static Application* s_Instance;
    bool m_Running = true;
    bool m_Minimized = false;
    float m_LastFrameTime = 0.0f;
    float m_Timer = 0.0f;
    float m_FixedDeltaTime = 1.0f/60.0f;

    bool OnWindowResize(WindowResizeEvent& e);
    bool OnWindowClose(WindowCloseEvent& e);
};


#endif //MOMENTUM_APPLICATION_H
