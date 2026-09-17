add_executable(Momentum main.cpp)

target_link_libraries(
        Momentum PRIVATE MomentumEngine
)

set_target_properties(
        Momentum PROPERTIES OUTPUT_NAME "Momentum"
)