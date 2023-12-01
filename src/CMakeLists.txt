add_executable(${PROJECT_NAME} sfec.cpp
                                tui/tui.h
                                tui/tui.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")