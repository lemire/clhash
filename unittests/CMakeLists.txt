PROJECT(TOOLS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")
set(EXTERNAL_DIR "${ROOT_DIR}/../3p")
message("ROOT_DIR: ${ROOT_DIR}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Read the git tags to determine the project version
include(GetGitVersion)
get_git_version(GIT_VERSION)

# # Tell the user what versions we are using
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" VERSION ${GIT_VERSION})
message("-- Version: ${VERSION}")

# The version of the libraries
set(GENERIC_LIB_VERSION ${VERSION})
string(SUBSTRING ${VERSION} 0 1 GENERIC_LIB_SOVERSION)

include(CheckCXXCompilerFlag)
include(AddCXXCompilerFlag)
include(CXXFeatureCheck)

# Use libc++  provided by llvm
# set(CMAKE_CXX_COMPILER "clang++")
# add_cxx_compiler_flag(-stdlib=libc++)

set (CMAKE_BUILD_TYPE Release)
add_cxx_compiler_flag(-O3)
add_cxx_compiler_flag(-march=native)

add_cxx_compiler_flag(-msse4.2)
add_cxx_compiler_flag(-mpclmul)
add_cxx_compiler_flag(-funroll-loops)
add_cxx_compiler_flag(-Wstrict-overflow)
add_cxx_compiler_flag(-Wstrict-aliasing)
add_cxx_compiler_flag(-fsanitize=undefined  -fno-omit-frame-pointer -fsanitize=address)

message("Build type: ${CMAKE_BUILD_TYPE}")

add_cxx_compiler_flag(-std=c++14)
add_cxx_compiler_flag(-Wall)

if (UTILS_ENABLE_WERROR)
  add_cxx_compiler_flag(-pedantic-errors)
endif()

# add_cxx_compiler_flag(-Wextra)
# add_cxx_compiler_flag(-pedantic)
# add_cxx_compiler_flag(-Wshadow)

add_cxx_compiler_flag(-Werror RELEASE)
add_cxx_compiler_flag(-Werror RELWITHDEBINFO)
add_cxx_compiler_flag(-Werror MINSIZEREL)

add_cxx_compiler_flag(-Wshorten-64-to-32)
add_cxx_compiler_flag(-Wfloat-equal)
add_cxx_compiler_flag(-Wint-to-void-pointer-cast)

add_cxx_compiler_flag(-fstrict-aliasing)
if (HAVE_CXX_FLAG_FSTRICT_ALIASING)
  add_cxx_compiler_flag(-Wstrict-aliasing)
endif()
add_cxx_compiler_flag(-Wthread-safety)
if (HAVE_WTHREAD_SAFETY)
  add_definitions(-DHAVE_WTHREAD_SAFETY)
  cxx_feature_check(THREAD_SAFETY_ATTRIBUTES)
endif()

add_cxx_compiler_flag(-DFMT_HEADER_ONLY)

ENABLE_TESTING()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include other headers such as fmt library.
include_directories ("${EXTERNAL_DIR}/include")

# Include utils headers
include_directories ("${ROOT_DIR}/src")

set(COMMAND_SRC_FILES basic_tests)
foreach (src_file ${COMMAND_SRC_FILES})
  ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
  TARGET_LINK_LIBRARIES(${src_file})
  ADD_TEST(${src_file} ./${src_file})
endforeach (src_file)
