cmake_minimum_required(VERSION 3.25)
project(renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(Vendors/googletest)

add_subdirectory(Core)
add_subdirectory(App)

add_custom_target(run
    COMMAND $<TARGET_FILE:App>
    DEPENDS App copy_assets_app
    DEPENDS Core copy_assets_core
    WORKING_DIRECTORY $<TARGET_FILE_DIR:App>
)
