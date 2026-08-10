cmake_minimum_required(VERSION 3.25)
project (App)


add_executable(App)


target_sources(
  App
	 PRIVATE
	 FILE_SET SOURCES
		BASE_DIRS
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source
		  ${CMAKE_CURRENT_SOURCE_DIR}/Vendors
		FILES
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Main.cpp
)


target_sources(
  App
	 PRIVATE
	 FILE_SET HEADERS
		BASE_DIRS
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source
		FILES
		  ${CMAKE_CURRENT_SOURCE_DIR}/Source/AppLayer.h
)

target_include_directories(
App
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Source
)


add_custom_command(TARGET App POST_BUILD 
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Source/shaders  ${CMAKE_CURRENT_BINARY_DIR}/shaders
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Source/images  ${CMAKE_CURRENT_BINARY_DIR}/images
)

target_link_libraries(App engine.core)
