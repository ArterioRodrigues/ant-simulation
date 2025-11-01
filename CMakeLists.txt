cmake_minimum_required(VERSION 3.28)
project(Ant-Simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE NUMPY_RESULT
)

if(NOT NUMPY_RESULT EQUAL 0)
    message(FATAL_ERROR "NumPy not found. Please install it: pip install numpy matplotlib")
endif()

message(STATUS "Python executable: ${Python3_EXECUTABLE}")
message(STATUS "Python include dir: ${Python3_INCLUDE_DIRS}")
message(STATUS "NumPy include dir: ${NUMPY_INCLUDE_DIR}")

add_executable(main 
src/configuration.cpp
src/game.cpp
src/main.cpp
src/world.cpp
src/player.cpp
)
target_include_directories(main PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${Python3_INCLUDE_DIRS}
    ${NUMPY_INCLUDE_DIR}
)

target_link_libraries(main PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
    Python3::Python
)

target_compile_features(main PRIVATE cxx_std_17)
target_compile_options(main PRIVATE -Wno-deprecated-declarations)

install(TARGETS main DESTINATION bin)
