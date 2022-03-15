project(SamplesToNotes)

file(GLOB_RECURSE
    SOURCES
    ${CMAKE_SOURCE_DIR}/include/*.h
    *.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    ${EXTERNAL_DIR}/fftw/libfftw3f-3.lib
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${EXTERNAL_DIR}/fftw
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${EXTERNAL_DIR}/fftw/libfftw3f-3.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)