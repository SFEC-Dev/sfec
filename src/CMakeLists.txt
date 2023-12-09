add_executable(
  ${PROJECT_NAME}
  sfec.cpp
  tui/tui.h
  tui/tui.cpp
  tui/tui_widgets.cpp
  tui/event/event_handler.h
  tui/event/event_handler.cpp
  tui/event/key_handler.h
  tui/event/key_handler.cpp
  tui/render/matrix.cpp
  tui/render/matrix.h
  tui/utils/icon.h
  tui/utils/icon.cpp
  config/options.h
  config/options.cpp
  config/interpreter.h
  config/interpreter.cpp
  )

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 "${CMAKE_BINARY_DIR}/bin")
