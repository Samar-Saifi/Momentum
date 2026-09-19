#include <iostream>

#include "Application.h"
#include "glm/gtc/constants.hpp"

class SandboxApp : public Application {
public:
    SandboxApp() : Application() {}

    void OnEvent(EventBase &e) override {
        if (e.GetGroup() == EventGroup::Keyboard) {
            std::cout << "[APP TRACE] Keyboard Event: " << e.GetName() << "\n";
        } else if (e.GetGroup() == EventGroup::Mouse) {
            std::cout << "[APP INFO] Mouse Event: " << e.GetName() << "\n";
        }
        Application::OnEvent(e);
    }

    void OnShutdown() override {
        std::cout << "SHUTTING DOWN... " << "\n";
    }
};

int main() {
    SandboxApp app;
    app.Run();
    return 0;
}