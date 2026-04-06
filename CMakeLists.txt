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
  "${CMAKE_SOURCE_DIR}/src/*.h"
  "${CMAKE_SOURCE_DIR}/src/*.hpp"
)


add_executable(FOUL ${SOURCES})

set_target_properties(FOUL PROPERTIES
  WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
  LINK_FLAGS_RELEASE "/ENTRY:mainCRTStartup"
  LINK_FLAGS_RELWITHDEBINFO "/ENTRY:mainCRTStartup"
)

file(GLOB_RECURSE FOUL_ALL_HEADERS "${CMAKE_SOURCE_DIR}/src/*.h")
set(FOUL_INCLUDE_DIRS "")

foreach(_header ${FOUL_ALL_HEADERS})
  get_filename_component(_dir "${_header}" DIRECTORY)
  list(APPEND FOUL_INCLUDE_DIRS "${_dir}")
endforeach()

list(REMOVE_DUPLICATES FOUL_INCLUDE_DIRS)

target_include_directories(FOUL PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
  ${FOUL_INCLUDE_DIRS}
)

target_link_libraries(FOUL PRIVATE Crow2D::Crow2D)

crow2d_copy_runtime(FOUL)
crow2d_copy_assets(FOUL)
