file(GLOB_RECURSE ENGINE_SOURCES
        "*.cpp"
        "Events/*.cpp"
        "external/glad/src/glad.c"
)

file(GLOB_RECURSE ENGINE_HEADERS
        "*.h"
        "Events/*.h"
)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp

        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui
        PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

add_library(MomentumEngine STATIC
        ${ENGINE_SOURCES}
        ${ENGINE_HEADERS}
)

target_include_directories(MomentumEngine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

target_compile_definitions(MomentumEngine
        PUBLIC
        GLFW_INCLUDE_NONE
)

target_link_libraries(MomentumEngine
        PUBLIC
        glfw
        glm::glm
        imgui
        ${CMAKE_DL_LIBS}
)
