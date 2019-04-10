cmake_minimum_required(VERSION 3.14)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project (StaticsMapping)

if (CMAKE_CXX_COMPILER_ID MATCHES "[Cc][Ll][Aa][Nn][Gg]")
  set (CMAKE_COMPILER_IS_CLANG)
  if (CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-instr-generate -fcoverage-mapping")
    find_package (Clang 3.9)
  endif ()
endif ()

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CXX_STANDARD_REQUIRED ON)

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
