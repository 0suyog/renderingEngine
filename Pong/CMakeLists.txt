cmake_minimum_required(VERSION 3.25)
project (Pong)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Pong)


target_sources(
  Pong
	 PRIVATE
	 FILE_SET SOURCES
		BASE_DIRS
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors
		FILES
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Main.cpp
)


target_sources(
  Pong
	 PRIVATE
	 FILE_SET HEADERS
		BASE_DIRS
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source
		FILES
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/gameLayer.h
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/paddle.h
)

target_include_directories(
Pong
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Source
)


add_custom_target(copy_assets_pong ALL 
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Source/shaders  ${CMAKE_CURRENT_BINARY_DIR}/shaders
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Source/images  ${CMAKE_CURRENT_BINARY_DIR}/images
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Source/models  ${CMAKE_CURRENT_BINARY_DIR}/models
)

target_link_libraries(Pong engine.core)
