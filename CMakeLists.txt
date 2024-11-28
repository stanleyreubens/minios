# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(MiniOS VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect source files
file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# Enable testing
# enable_testing()
# add_subdirectory(test)

# Find GTest
find_package(GTest REQUIRED)

# Create executable
add_executable(minios ${SOURCES})

# Find and link threads
find_package(Threads REQUIRED)
target_link_libraries(minios PRIVATE Threads::Threads)

# Debug builds
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Testing
# enable_testing()
# add_subdirectory(test)

#Installation rules
install(TARGETS minios DESTINATION bin)

# macOS specific settings
if(APPLE)
    execute_process(
        COMMAND brew --prefix
        OUTPUT_VARIABLE HOMEBREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    include_directories(${HOMEBREW_PREFIX}/include)
    link_directories(${HOMEBREW_PREFIX}/lib)
endif()

# Print source files for debugging
message(STATUS "Source files:")
foreach(source ${SOURCES})
    message(STATUS "  ${source}")
endforeach()

# FreeBSD specific settings
if(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    # Add FreeBSD specific includes
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)

    # Use clang compiler
    set(CMAKE_CXX_COMPILER clang++)
    
    # Add FreeBSD specific flags
    add_compile_options(-pthread)
    add_link_options(-pthread)
endif()