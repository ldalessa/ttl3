cmake_minimum_required(VERSION 3.23)
project(ttl CXX)

include (FetchContent)

FetchContent_Declare(fmt
  GIT_REPOSITORY     https://github.com/fmtlib/fmt.git
  GIT_TAG            8.1.1)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(kumi
  GIT_REPOSITORY     https://github.com/jfalcou/kumi.git
  GIT_TAG            main)
set(KUMI_BUILD_TEST OFF)
FetchContent_MakeAvailable(kumi)

FetchContent_Declare(tag_invoke_wrapper
  GIT_REPOSITORY     https://github.com/ldalessa/tag_invoke_wrapper.git
  GIT_TAG            main)
FetchContent_MakeAvailable(tag_invoke_wrapper)

add_library(ttl_lib INTERFACE)
target_include_directories(ttl_lib INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_link_libraries(ttl_lib INTERFACE fmt::fmt kumi::kumi tag_invoke_wrapper::tag_invoke)
target_compile_features(ttl_lib INTERFACE cxx_std_23)
add_library(ttl::ttl ALIAS ttl_lib)

add_subdirectory(tests)
