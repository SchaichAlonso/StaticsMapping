cmake_minimum_required(VERSION 3.5)

if(WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT WIN32) #MSVC branching is broken
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
endif ()

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

find_package (Qt5 5.10 COMPONENTS REQUIRED Core Gui Network Widgets Test)
find_package (Threads)

add_subdirectory (Common)

add_subdirectory (TagTool)
add_subdirectory (ObjIdentify)
add_subdirectory (ObjPreview)
add_subdirectory (Library)
add_subdirectory (Test)
