#include "Application.h"

class SandboxApp : public Application {
public:
    SandboxApp() : Application() {}
};

int main() {
    SandboxApp app;
    app.Run();
    return 0;
}