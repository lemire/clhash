PROJECT(TOOLS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")
set(EXTERNAL_DIR "${ROOT_DIR}/../3p")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# CMake build flags
include(CheckCXXCompilerFlag)
include(AddCXXCompilerFlag)
include(CXXFeatureCheck)

# Use libc++  provided by llvm
# set(CMAKE_CXX_COMPILER "clang++")
# add_cxx_compiler_flag(-stdlib=libc++)

set (CMAKE_BUILD_TYPE Release)
add_cxx_compiler_flag(-O3)
add_cxx_compiler_flag(-march=native)
# add_cxx_compiler_flag(-msse4.2)
add_cxx_compiler_flag(-mpclmul)
add_cxx_compiler_flag(-funroll-loops)
add_cxx_compiler_flag(-Wstrict-overflow)
add_cxx_compiler_flag(-Wstrict-aliasing)
add_cxx_compiler_flag(-std=c++14)
add_cxx_compiler_flag(-Wall)
# add_cxx_compiler_flag(-flto)

add_cxx_compiler_flag(-fstrict-aliasing)
if (HAVE_CXX_FLAG_FSTRICT_ALIASING)
  add_cxx_compiler_flag(-Wstrict-aliasing)
endif()

# add_cxx_compiler_flag(-Wthread-safety)
if (HAVE_WTHREAD_SAFETY)
  add_definitions(-DHAVE_WTHREAD_SAFETY)
  cxx_feature_check(THREAD_SAFETY_ATTRIBUTES)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include other headers such as fmt library.
include_directories ("${EXTERNAL_DIR}/include")

# Include clhash headers
include_directories ("${ROOT_DIR}/src")

# Used libraries
SET(LIB_BENCHMARK "${EXTERNAL_DIR}/lib/libbenchmark.a")
set(COMMAND_SRC_FILES basic_benchmark random_string)
foreach (src_file ${COMMAND_SRC_FILES})
  ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
  TARGET_LINK_LIBRARIES(${src_file} -lpthread -lm ${LIB_BENCHMARK})
endforeach (src_file)
