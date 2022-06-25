cmake_minimum_required(VERSION 3.23)
project(ttl CXX)

# include (FetchContent)

# FetchContent_Declare(fmt
#   GIT_REPOSITORY     https://github.com/fmtlib/fmt.git
#   GIT_TAG            8.1.1)
# FetchContent_MakeAvailable(fmt)

add_library(ttl_lib INTERFACE)
target_include_directories(ttl_lib INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)
# target_link_libraries(ttl_lib INTERFACE fmt::fmt)
target_compile_features(ttl_lib INTERFACE cxx_std_23)
add_library(ttl::ttl ALIAS ttl_lib)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE ttl_lib)
