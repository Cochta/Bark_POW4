cmake_minimum_required(VERSION 3.22)
project(Bark_POW4)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest CONFIG REQUIRED)
find_package(imgui-sfml CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

option(ENABLE_ADDRESS_SANITIZER "Enable Address Sanitizer" OFF)

# Add a CMake option to enable or disable Tracy Profiler
option(USE_TRACY "Use Tracy Profiler" OFF)

if (USE_TRACY)
    # Enable Tracy profiling by setting the preprocessor directive
    add_compile_definitions(TRACY_ENABLE)
    # Include Tracy's headers for the entire project
    include_directories(libs/TracyProfiler/tracy)

    # Add the Tracy source code as part of the project
    add_library(tracyClient STATIC libs/TracyProfiler/TracyClient.cpp)
endif()

IF(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi /Oy-")

    # Check if AddressSanitizer is enabled
    if(ENABLE_ADDRESS_SANITIZER)
        # Enable AddressSanitizer flags and options
        add_compile_options(/fsanitize=address /DEBUG)
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps -march=haswell -ffast-math")

    # Check if AddressSanitizer is enabled
    if(ENABLE_ADDRESS_SANITIZER)
        # Enable AddressSanitizer flags and options
        add_compile_options(-fsanitize=address /DEBUG)
    endif()
ENDIF()

# Common library
file(GLOB_RECURSE COMMON_FILES common/include/*.h common/src/*.cpp)
add_library(Common ${COMMON_FILES})
set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Common PUBLIC common/include/)

if (USE_TRACY)
    target_compile_definitions(Common PUBLIC TRACY_ENABLE)
    # Link the TracyClient library
    target_link_libraries(Common PRIVATE tracyClient)
endif()

# Engine library
file(GLOB_RECURSE ENGINE_FILES engine/include/*.h engine/src/*.cpp)
add_library(Engine ${ENGINE_FILES})
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Engine PUBLIC engine/include/)
target_include_directories(Engine PUBLIC libs/Math/include/)
target_include_directories(Engine PUBLIC common/include/)
target_link_libraries(Engine PUBLIC fmt::fmt)

if (USE_TRACY)
    target_compile_definitions(Engine PUBLIC TRACY_ENABLE)
    # Link the TracyClient library
    target_link_libraries(Engine PRIVATE tracyClient)
endif()

# Sample library
file(GLOB_RECURSE SAMPLE_FILES samples/include/*.h samples/src/*.cpp)
add_library(Samples ${SAMPLE_FILES})
set_target_properties(Samples PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Samples PUBLIC samples/include)
target_include_directories(Samples PUBLIC engine/include/)
target_include_directories(Samples PUBLIC libs/Math/include/)
target_include_directories(Samples PUBLIC common/include/)

# Graphics & Networking library using sfml


file(GLOB_RECURSE GRAPHICS_FILES SFML/include/*.h SFML/src/*.cpp)
add_library(GraphNet ${GRAPHICS_FILES})
set_target_properties(GraphNet PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(GraphNet PUBLIC SFML/include/)
target_include_directories(GraphNet PUBLIC engine/include/)
target_include_directories(GraphNet PUBLIC libs/Math/include/)
target_include_directories(GraphNet PUBLIC common/include/)
target_include_directories(GraphNet PUBLIC samples/include/)

target_link_libraries(GraphNet PUBLIC Samples Engine Common sfml-system sfml-network sfml-graphics sfml-window ImGui-SFML::ImGui-SFML)


if (USE_TRACY)
    target_compile_definitions(GraphNet PUBLIC TRACY_ENABLE)
    # Link the TracyClient library
    target_link_libraries(GraphNet PRIVATE tracyClient)
endif()

add_executable(Main Main.cpp)
target_link_libraries(Main GraphNet)