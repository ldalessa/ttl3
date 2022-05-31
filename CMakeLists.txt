cmake_minimum_required(VERSION 3.23)
project(ttl CXX)

add_library(ttl_lib INTERFACE)
target_include_directories(ttl_lib INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
#target_link_libraries(ttl_lib INTERFACE ce::ce fmt::fmt tag_invoke_wrapper::tag_invoke)
target_compile_features(ttl_lib INTERFACE cxx_std_23)
add_library(ttl::ttl ALIAS ttl_lib)

add_subdirectory(tests)
