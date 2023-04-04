
cmake_minimum_required(VERSION 3.7...3.26)

add_subdirectory(external)

project(osmio-loader
        VERSION 0.1
        DESCRIPTION "An implementation of a libosmio loader"
        LANGUAGES CXX)

file(GLOB_RECURSE LIBOSMIO_LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/lib/**.h)
file(GLOB_RECURSE LIBOSMIO_LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lib/**.cpp)

add_library(osmio-loader
        STATIC ${LIBOSMIO_LIB_SOURCES})
add_library(libosmio-loader
        ALIAS osmio-loader)

target_include_directories(osmio-loader
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set_target_properties(osmio-headers PROPERTIES
        LIBOSMIO_HIDE_LOADER_FUNCTIONS OFF)

target_link_libraries(osmio-loader
        PUBLIC osmio-headers)
