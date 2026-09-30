//
// Created by mdsamar on 30/09/26.
//

#ifndef MOMENTUM_IMGUIMANAGER_H
#define MOMENTUM_IMGUIMANAGER_H
#include "EventBase.h"
#include "GLFW/glfw3.h"


class ImGuiManager {
public:
    ImGuiManager() = default;
    ~ImGuiManager() = default;

    void Init(GLFWwindow* window);
    void Shutdown();
    void Begin();
    void End();
    void OnEvent(EventBase& event);
    void SetBlockEvent(bool block) {m_BlockEvent = block;};

private:
    bool m_BlockEvent = true;
};


#endif //MOMENTUM_IMGUIMANAGER_H
