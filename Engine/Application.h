#ifndef MOMENTUM_APPLICATION_H
#define MOMENTUM_APPLICATION_H
#include <memory>
#include <string>

class Window;

class Application {
public:
    Application(const std::string& name = "Momentum");
    virtual ~Application();

    void Run();
    void Quit();

    static Application& GetApplication() {return *s_Instance;}


protected:
    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnShutdown() {}

private:
    std::unique_ptr<Window> m_Window;
    static Application* s_Instance;
    bool m_Running = true;
};


#endif //MOMENTUM_APPLICATION_H
