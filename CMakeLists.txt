cmake_minimum_required(VERSION 3.25)
project(renderer)

add_subdirectory(Core)
add_subdirectory(App)

add_custom_target(run
  COMMAND $<TARGET_FILE:App>
  DEPENDS App
  WORKING_DIRECTORY $<TARGET_FILE_DIR:App>
)
