
cmake_minimum_required(VERSION 3.7...3.26)

add_subdirectory(external)

project(osmio-loader
        VERSION 0.1
        DESCRIPTION "An implementation of a libosmio loader"
        LANGUAGES CXX)

file(GLOB_RECURSE LIBOSMIO_LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/**.h)
file(GLOB_RECURSE LIBOSMIO_LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp)

add_library(osmio-loader
        SHARED ${LIBOSMIO_LIB_SOURCES})
add_library(libosmio-loader
        ALIAS osmio-loader)

target_include_directories(osmio-loader
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(osmio-loader
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(osmio-headers PROPERTIES
        LIBOSMIO_HIDE_LOADER_FUNCTIONS OFF)

target_link_libraries(osmio-loader
        PUBLIC osmio-headers)


set(OSMIO_LOADER_TARGET_PLATFORM "UNKNOWN")
if (UNIX)
    set(OSMIO_LOADER_TARGET_PLATFORM "UNIX")
endif ()

add_subdirectory(platform)
add_subdirectory(test)
