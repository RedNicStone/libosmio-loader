
function(CREATE_TEST name)

    set(TEST_NAME osmio-loader_test-${name})
    add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${name}/${name}.cpp)

    target_link_libraries(${TEST_NAME} PRIVATE osmio-loader)

    get_target_property(OSMIO_LOADER_PRIVATE_INCLUDE_DIRS osmio-loader INCLUDE_DIRECTORIES)
    get_target_property(OSMIO_LOADER_PRIVATE_LINK_LIBRARIES ${TEST_NAME} LINK_LIBRARIES)

    target_include_directories(${TEST_NAME} PRIVATE ${OSMIO_LOADER_PRIVATE_INCLUDE_DIRS})
    target_link_libraries(${TEST_NAME} PRIVATE ${OSMIO_LOADER_PRIVATE_LINK_LIBRARIES})

endfunction()

add_subdirectory(platform)
