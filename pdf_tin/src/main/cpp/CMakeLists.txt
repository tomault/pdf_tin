include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${POPPLER_INCLUDE_DIR})
include_directories(${glib_deps_INCLUDE_DIRS})

set(pdf_tin_SRCS
  pdf_tin/BoundingBox.cpp
  pdf_tin/Color.cpp
  pdf_tin/Document.cpp
  pdf_tin/DocumentCreationError.cpp
  pdf_tin/NoSuchPageError.cpp
  pdf_tin/Page.cpp
  pdf_tin/PageLayout.cpp
  pdf_tin/PageMode.cpp
  pdf_tin/PropertyAccessError.cpp
  pdf_tin/TextStyle.cpp
  pdf_tin/TextStyleSegment.cpp
)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_library(pdf_tin STATIC ${pdf_tin_SRCS})
else()
  add_library(pdf_tin ${pdf_tin_SRCS})
endif()

set_target_properties(pdf_tin
  PROPERTIES VERSION "${PDF_TIN_VERSION}"
  SOVERSION "${PDF_TIN_MAJOR_VERSION}.${PDF_TIN_MINOR_VERSION}"
)

target_compile_options(pdf_tin PRIVATE -fPIC)

install(
  TARGETS pdf_tin
  EXPORT pdf_tin_export
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT pdf_tin_export
  FILE pdf_tin_export.cmake
  NAMESPACE pdf_tin_upstream::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pdf_tin
)

add_library(pdf_tin_upstream::pdf_tin ALIAS pdf_tin)

