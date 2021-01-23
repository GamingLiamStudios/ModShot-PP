cmake_minimum_required(VERSION 3.9)
project(OneShot)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

# Find OpenSSL
find_package(SFML REQUIRED COMPONENTS window graphics audio)

add_subdirectory(extern)

set(SOURCE_FILES
    # MAIN
    source/main.cpp

    # CHARACTERS
    source/characters/character.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC "extern")
target_include_directories(${PROJECT_NAME} PUBLIC "source") # to allow paths relative to project root

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-window sfml-graphics sfml-audio Boxer)
