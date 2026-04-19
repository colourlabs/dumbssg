cmake_minimum_required(VERSION 3.20)
project(dumbssg)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra)

find_package(yaml-cpp REQUIRED)

add_executable(dumbssg
  src/main.cpp
  src/parser.cpp
  src/generator.cpp
)

target_include_directories(dumbssg PRIVATE include)

target_link_libraries(dumbssg PRIVATE
  yaml-cpp
  md4c
  md4c-html
)

add_custom_command(
  TARGET dumbssg POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/templates
    ${CMAKE_BINARY_DIR}/templates
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/content
    ${CMAKE_BINARY_DIR}/content
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/config.yaml
    ${CMAKE_BINARY_DIR}/config.yaml
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
  COMMENT "Copying assets to build directory"
)