add_executable(
  ${PROJECT_NAME}
  sfec.cpp
  tui/utils/style.h
  tui/utils/style.cpp
  tui/tui.h
  tui/tui.cpp
  tui/event/key_handler.h
  tui/event/key_handler.cpp
  config/options.h
  config/options.cpp
  config/interpreter.h
  config/interpreter.cpp
  )

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 "${CMAKE_BINARY_DIR}/bin")
