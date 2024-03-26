cmake_minimum_required(VERSION 3.22)
project(Bark_POW4)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

set(PORT "44444" CACHE STRING "Set the port")
set(HOST_NAME "localhost" CACHE STRING "Set the host name")

add_definitions("-DPORT=${PORT}")
add_definitions("-DHOST_NAME=\"${HOST_NAME}\"")


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

# Graphics & Networking library using sfml
file(GLOB_RECURSE SAMPLE_FILES game/include/*.h game/src/*.cpp)
add_library(Game ${SAMPLE_FILES})
set_target_properties(Game PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Game PUBLIC game/include)
target_include_directories(Game PUBLIC engine/include/)
target_include_directories(Game PUBLIC libs/Math/include/)
target_include_directories(Game PUBLIC common/include/)

target_link_libraries(Game PUBLIC Engine Common sfml-system sfml-network sfml-graphics sfml-window ImGui-SFML::ImGui-SFML)


# Copy font file to build directory
configure_file(SFML/ressources/LiberationSans.ttf SFML/ressources/LiberationSans.ttf COPYONLY)

if (USE_TRACY)
    target_compile_definitions(Game PUBLIC TRACY_ENABLE)
    # Link the TracyClient library
    target_link_libraries(Game PRIVATE tracyClient)
endif()

file(GLOB TEST_FILES tests/*.cpp )
foreach(test_file ${TEST_FILES} )
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name} PUBLIC Game)
    target_link_libraries(${test_name} PRIVATE GTest::gtest GTest::gtest_main)

    IF(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi /Oy-")
    else()
        set_target_properties(${test_name} PROPERTIES COMPILE_FLAGS "-save-temps -march=haswell -ffast-math -fno-omit-frame-pointer")
    ENDIF()
endforeach()

add_executable(Client Client.cpp)
target_link_libraries(Client Game)

add_executable(Server Server.cpp)
target_link_libraries(Server Game)