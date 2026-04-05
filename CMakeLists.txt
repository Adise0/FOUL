cmake_minimum_required(VERSION 3.25)
project(FOUL VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point CMake to the installed Crow2D package
find_package(Crow2D REQUIRED
  PATHS "C:/Projects/Apps/Crow2D/Crow2D/out/Crow2D_SDK"
  NO_DEFAULT_PATH
)

file(GLOB_RECURSE SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/*.cxx"
)
file(GLOB_RECURSE HEADERS
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.h"
  "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

add_executable(FOUL ${SOURCES} ${HEADERS})

set_target_properties(FOUL PROPERTIES
  WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
  LINK_FLAGS_RELEASE "/ENTRY:mainCRTStartup"
)

target_link_libraries(FOUL PRIVATE Crow2D::Crow2D)

crow2d_copy_runtime(FOUL)
crow2d_copy_assets(FOUL)
