cmake_minimum_required(VERSION 3.20)
project(dumbssg)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra)

# probably not the best way but it worked ig
if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
  list(APPEND CMAKE_PREFIX_PATH /usr/local)
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
endif()

find_package(yaml-cpp REQUIRED)

set(md4c_DIR 
  $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},FreeBSD>,
    /usr/local/lib/cmake/md4c,
    /usr/lib/cmake/md4c>
)
find_package(md4c REQUIRED)

add_executable(dumbssg
  src/main.cpp
  src/parser.cpp
  src/generator.cpp
)

target_include_directories(dumbssg PRIVATE include)

target_link_libraries(dumbssg PRIVATE
  yaml-cpp::yaml-cpp
  md4c::md4c
  md4c::md4c-html
)

add_custom_command(
  TARGET dumbssg POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/templates
    ${CMAKE_BINARY_DIR}/templates
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/content
    ${CMAKE_BINARY_DIR}/content
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/config.yaml
    ${CMAKE_BINARY_DIR}/config.yaml
  COMMENT "Copying assets to build directory"
)