# Specify minimum CMake version required
cmake_minimum_required(VERSION 3.20.0)

# Define project name
project(qry)

# Enable testing functionality in CMake
enable_testing()

# Include FetchContent module - this is a CMake module that helps download and configure
# external dependencies at configure time. Think of it like a package manager built into CMake.
include(FetchContent)

# Declare that we want to fetch Google Test.
# This is like saying "here's the recipe for getting Google Test"
FetchContent_Declare(
    googletest                                              # Name we give to this dependency
    GIT_REPOSITORY https://github.com/google/googletest.git # Where to download from
    GIT_TAG v1.15.2                                        # Which version to get
)

# FetchContent_MakeAvailable actually does the work:
# 1. Downloads Google Test if it's not already downloaded
# 2. Configures it with CMake
# 3. Makes its targets (like GTest::gtest_main) available for us to use
FetchContent_MakeAvailable(googletest)

# Find all source files recursively in the src directory
# CONFIGURE_DEPENDS means CMake will re-run if files are added or removed
file(GLOB_RECURSE MAIN_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.hpp
)

# Remove main.cpp from MAIN_SOURCES - we'll handle it separately
# This is important because we don't want main() in our library
list(FILTER MAIN_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Find all test source files
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/tests/*.cpp
    ${CMAKE_SOURCE_DIR}/tests/*.hpp
)

# Create a library from your source files (excluding main.cpp)
# This library contains all your actual functionality
add_library(qry_lib ${MAIN_SOURCES})

# Tell CMake where to find header files for this library
target_include_directories(qry_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Create the main executable
# We first create it without sources
add_executable(qry)

# Then we add main.cpp specifically to the executable
# This ensures main() is only in the executable, not the library
target_sources(qry PRIVATE ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Link our library to the main executable
# This gives the executable access to all the functionality in qry_lib
target_link_libraries(qry PRIVATE qry_lib)


add_executable(qry_tests ${TEST_SOURCES})

# Link the test executable with:
# 1. Our library (qry_lib) so tests can access the code they're testing
# 2. Google Test's main library which provides test running functionality
target_link_libraries(qry_tests 
    PRIVATE 
        qry_lib
        GTest::gtest_main
)

# This tells the test executable where to find headers
# Usually not needed because it inherits paths from qry_lib,
# but added here for clarity
target_include_directories(qry_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# Include Google Test module for test discovery
include(GoogleTest)

# Tell CMake to find all the tests in our test executable
gtest_discover_tests(qry_tests)