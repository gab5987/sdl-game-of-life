cmake_minimum_required(VERSION 3.10)

set(TARGET_BIN "game")

project(${TARGET_BIN})

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

# add_subdirectory(lib)

file(GLOB SOURCES src/*.*)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${TARGET_BIN} ${SOURCES})
target_link_libraries(${TARGET_BIN} ${SDL2_LIBRARIES})
set_property(TARGET ${TARGET_BIN} PROPERTY CXX_STANDARD 17)
target_compile_options(${TARGET_BIN} PRIVATE -g -Wall -Wextra -O0)
set_target_properties(${TARGET_BIN} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

install(TARGETS ${OBJECT_FILES}
    RUNTIME DESTINATION bin
    # LIBRARY DESTINATION lib
    # ARCHIVE DESTINATION lib
)

