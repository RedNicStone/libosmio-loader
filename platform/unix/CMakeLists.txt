
file(GLOB_RECURSE LIBOSMIO_PLATFORM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp)

target_sources(osmio-loader
        PRIVATE ${LIBOSMIO_PLATFORM_SOURCES})

target_include_directories(osmio-loader
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(osmio-loader
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(lib)
target_link_libraries(osmio-loader
        PRIVATE jsoncpp)


macro(osmio_compile_option name help_text default_value)
    option(${name} ${help_text} ${default_value})
    if (NOT ${name} EQUAL 0)
        target_compile_definitions(osmio-loader PRIVATE
                ${name})
    endif ()
endmacro()

option(OSMIO_CONFDIR "<help_text>" "vulkan/icd.d")
option(OSMIO_SYSCONFDIR "<help_text>" "/etc")
option(OSMIO_EXTRASYSCONFDIR "<help_text>")
option(OSMIO_USE_SYSCONFDIR "<help_text>" 1)
option(OSMIO_USE_ENVSYSCONFDIR "<help_text>" 1)
option(OSMIO_USE_ENVUSERCONFDIR "<help_text>" 1)
option(OSMIO_USE_SECURE_GETENV "<help_text>" 1)


target_compile_definitions(osmio-loader PRIVATE
        OSMIO_CONFDIR="osmio/icd.d"
        OSMIO_SYSCONFDIR="/etc"
        OSMIO_USE_SYSCONFDIR=1
        OSMIO_USE_ENVSYSCONFDIR=1
        OSMIO_USE_ENVUSERCONFDIR=1
        OSMIO_USE_SECURE_GETENV=1)
