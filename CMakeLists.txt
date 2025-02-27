cmake_minimum_required(VERSION 3.14)

project(grpc-extended-cpp-plugin
    DESCRIPTION "gRPC extended C++ code generator"
    LANGUAGES CXX C
)

find_package(everest-cmake 0.4
    REQUIRED
    PATHS ../everest-cmake
)

evc_setup_edm()

add_executable(grpc_extended_cpp_plugin
    extended_cpp_plugin.cc
    extended_cpp_generator.cc
)
target_include_directories(grpc_extended_cpp_plugin
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${grpc_SOURCE_DIR}
)
target_link_libraries(grpc_extended_cpp_plugin
    grpc_plugin_support
)

install(TARGETS grpc_extended_cpp_plugin)