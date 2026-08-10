cmake_minimum_required(VERSION 3.25)
project (Core)
add_library(engine.core)
add_library(engine::core ALIAS engine.core)

target_sources(
  engine.core
	 PRIVATE
	 FILE_SET SOURCES
		BASE_DIRS
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source
		  ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors
		FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Source/transform.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/application.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/camera.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/layer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Vendors/stb/stb_image.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/file.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Renderer/renderer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Renderer/shader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Renderer/texture.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Vendors/glad/src/glad.c
)

target_sources(
  engine.core
	 PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source
		  ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors
		FILES
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors/stb/stb_image.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/movement.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/layer.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/window.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/application.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/transform.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/camera.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/event.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/windowevents.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/inputevents.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/file.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/renderables.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/shader.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/mesh.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/renderer.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/2dprimitives.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/texture.h
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

target_link_libraries(engine.core PUBLIC glfw)
