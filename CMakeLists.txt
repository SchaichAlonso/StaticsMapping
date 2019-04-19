cmake_minimum_required(VERSION 3.14)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project (StaticsMapping)

enable_testing()

if (CMAKE_CXX_COMPILER_ID MATCHES Clang OR CXX_COMPILER_ID MATCHES AppleClang)
  set (CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-instr-generate -fcoverage-mapping")
elseif (CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set (CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} -ftest-coverage")
endif ()

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5Core    5.12 CONFIG REQUIRED)
find_package(Qt5Gui     5.12 CONFIG REQUIRED)
find_package(Qt5Widgets 5.12 CONFIG REQUIRED)
find_package(Qt5Network 5.12 CONFIG REQUIRED)
find_package(Qt5Test    5.12 CONFIG REQUIRED)
find_package(Threads)

add_subdirectory (Common)

add_subdirectory (TagTool)
add_subdirectory (ObjIdentify)
add_subdirectory (ObjPreview)
# add_subdirectory (Library)
add_subdirectory (Test)
