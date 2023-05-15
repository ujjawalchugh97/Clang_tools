cmake_minimum_required(VERSION 3.15)

project(MyClangTool)

# Set the LLVM directory path

set(LLVM_DIR "/path/to/llvm/build/lib/cmake/llvm")

# Find the LLVM package

find_package(LLVM REQUIRED CONFIG)

# Add the LLVM include directories

include_directories(

  ${LLVM_INCLUDE_DIRS}

  "/path/to/llvm/include"

  "/path/to/clang/include"

)

# Add the LLVM definitions

add_definitions(

  -D__STDC_LIMIT_MACROS

  -D__STDC_CONSTANT_MACROS

  -D__STDC_FORMAT_MACROS

)

# Set the C++ standard to C++11

set(CMAKE_CXX_STANDARD 11)

# Set the Clang tool executable

add_executable(my-clang-tool MyClangTool.cpp)

# Link against LLVM libraries

llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link the Clang tool against LLVM libraries

target_link_libraries(my-clang-tool ${llvm_libs})


