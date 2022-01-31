cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(
  pdf_tin
  VERSION 0.1.0
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(PkgConfig REQUIRED)

if (WIN32)
  message("-- Using win32 threads")
else()
  set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  find_package(Threads)
endif()
include(TestBigEndian)
test_big_endian(WORDS_BIGENDIAN)

include(GenerateExportHeader)
include(GNUInstallDirs)
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

pkg_check_modules(glib_deps REQUIRED IMPORTED_TARGET glib-2.0 cairo)
find_library(POPPLER_LIB_DIR poppler REQUIRED)
find_path(POPPLER_INCLUDE_DIR poppler/glib/poppler.h REQUIRED)

#FetchContent_Declare(
#  poppler
#  URL https://github.com/freedesktop/poppler/archive/refs/tags/poppler-21.09.0.zip
#)
#FetchContent_MakeAvailable(poppler)

set(ENABLE_FUZZER FALSE)

#set(PDF_TIN_MAJOR_VERSION "0")
#set(PDF_TIN_MINOR_VERSION "1")
#set(PDF_TIN_MICRO_VERSION "0")
#set(PDF_TIN_VERSION "${PDF_TIN_MAJOR_VERSION}.${PDF_TIN_MINOR_VERSION}.${PDF_TIN_MICRO_VERSION}")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_LINK_DEPENDS_NO_SHARED TRUE)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory(src/main/cpp)
add_subdirectory(src/test/cpp)
