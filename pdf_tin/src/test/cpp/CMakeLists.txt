enable_testing()

if (NOT TARGET pdf_tin_upstream::pdf_tin)
  find_package(pdf_tin REQUIRED)
endif()

find_library(POPPLER_LIB poppler REQUIRED)
find_library(POPPLER_GLIB poppler-glib REQUIRED)

set(pdf_tin_test_SRCS
  pdf_tin/BoundingBoxTests.cpp
  pdf_tin/DocumentTest.cpp
  pdf_tin/ImmutableListTests.cpp
  pdf_tin/NoSuchPageErrorTests.cpp
  pdf_tin/PageTests.cpp
)

add_executable(pdf_tin_tests ${pdf_tin_test_SRCS})

#add_executable(
#  NoSuchPageErrorTests
#  pdf_tin/NoSuchPageErrorTests.cpp
#)

#add_executable(
#  BoundingBoxTests
#  pdf_tin/BoundingBoxTests.cpp
#)

target_include_directories(
  pdf_tin_tests
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE $<TARGET_PROPERTY:pdf_tin,INCLUDE_DIRECTORIES>
)

target_link_libraries(
  pdf_tin_tests
  pdf_tin_upstream::pdf_tin
  gtest_main
  ${POPPLER_GLIB}
  ${POPPLER_LIB}
  gio-2.0
  gobject-2.0
  glib-2.0
  cairo
  freetype
  fontconfig
  jpeg
  openjp2
  png16
  z
)

include(GoogleTest)
gtest_discover_tests(pdf_tin_tests)

add_custom_command(
  TARGET pdf_tin_tests
  COMMENT "Run unit tests"
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/resources
  COMMAND
    ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../resources ${CMAKE_CURRENT_BINARY_DIR}/resources
  COMMAND TEST_RESOURCE_DIR=${CMAKE_CURRENT_BINARY_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/pdf_tin_tests
)
