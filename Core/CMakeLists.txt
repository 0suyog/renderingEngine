cmake_minimum_required(VERSION 3.25)
project (Core)
add_library(engine.core)
add_library(engine::core ALIAS engine.core)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

target_sources(
  engine.core
	 PRIVATE
	 FILE_SET SOURCES
		BASE_DIRS
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors
		FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Source/transform.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/application.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/layer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Vendors/stb/stb_image.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/file.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/regularPolygon.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/camera.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/shader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/texture.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/model.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/mesh.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/renderer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Physics/boundingSphere.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Physics/aabb.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Physics/plane.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Physics/physicsObject.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Vendors/glad/src/glad.c
)

target_sources(
  engine.core
	 PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors
		FILES
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors/stb/stb_image.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/movement.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/layer.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/window.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/application.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/transform.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/event.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/windowevents.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/inputevents.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/file.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/regularPolygon.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/renderables.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/camera.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/shader.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/mesh.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/model.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/renderer.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/texture.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/embededimages.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/renderContext.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Physics/boundingSphere.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Physics/aabb.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Physics/plane.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Physics/physicsObject.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Physics/intersectData.h
)

set_target_properties(engine.core 
PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON
)

target_include_directories(engine.core 
		PUBLIC
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source
		  ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors/stb
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors/glad/include
)
add_subdirectory(Vendors/glfw)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(Vendors/assimp)

add_custom_target(copy_assets_core ALL 
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/assets  ${CMAKE_BINARY_DIR}/assets
)
target_link_libraries(engine.core PUBLIC glfw assimp)

option(BUILD_CORE_TESTS "Build Core tests" OFF)

if(BUILD_CORE_TESTS)
    add_subdirectory(tests)
endif()
