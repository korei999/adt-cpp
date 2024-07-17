cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(adt-cpp VERSION 0.2 LANGUAGES CXX C)
message(STATUS "CMAKE_PROJECT_VERSION: '${CMAKE_PROJECT_VERSION}'")

add_compile_options(-Wall -Wextra)

add_executable(
    ${CMAKE_PROJECT_NAME}
    src/main.cc
)

if (CMAKE_BUILD_TYPE MATCHES "Asan")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3 -fsanitize=undefined -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -fsanitize=undefined -fsanitize=address")
    add_link_options(-fsanitize=undefined -fsanitize=address)
endif()

message(STATUS "CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-field-initializers")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function)
endif()

message(STATUS "CMAKE_SYSTEM_NAME: '${CMAKE_SYSTEM_NAME}'")
message(STATUS "CMAKE_SOURCE_DIR: '${CMAKE_SOURCE_DIR}'")

# install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
