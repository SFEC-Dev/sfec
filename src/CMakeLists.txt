add_executable(
  ${PROJECT_NAME}
  sfec.cpp
  tui/helpers/style.h
  tui/helpers/style.cpp
  tui/tui.h
  tui/tui.cpp
  config/options.h
  config/options.cpp
  config/interpreter.h
  config/interpreter.cpp
  )

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 "${CMAKE_BINARY_DIR}/bin")
