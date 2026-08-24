cmake_minimum_required(VERSION 3.25)
project(renderer)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Core)
add_subdirectory(Pong)
# add_subdirectory(App)

# add_custom_target(run
#   COMMAND $<TARGET_FILE:App>
#   DEPENDS App copy_assets
#   WORKING_DIRECTORY $<TARGET_FILE_DIR:App>
# )

add_custom_target(pong
  COMMAND $<TARGET_FILE:Pong>
  DEPENDS Pong copy_assets
  WORKING_DIRECTORY $<TARGET_FILE_DIR:Pong>
)
