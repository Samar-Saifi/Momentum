file(GLOB_RECURSE ENGINE_SOURCES
        "*.cpp"
        "external/glad/src/glad.c"
)

file(GLOB_RECURSE ENGINE_HEADERS
        "*.h"
)

add_library(MomentumEngine STATIC
        ${ENGINE_SOURCES}
        ${ENGINE_HEADERS}
        EventBase.h
        Events/ApplicationEvents.h
        Events/KeyboardEvents.h
        Events/MouseEvents.h)

target_include_directories(MomentumEngine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

target_link_libraries(MomentumEngine
        PUBLIC
        glfw
        glm::glm
        ${CMAKE_DL_LIBS}
)
