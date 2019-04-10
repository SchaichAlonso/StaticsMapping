project (Common)

set(SHARED_DATA_PATH "share/StaticsMapping")

add_library (Common STATIC
  CryptoHash.cpp
  DataPath.cpp
  Diagnostics.cpp
  Json.cpp
)

set_property (
  TARGET
    Common
  PROPERTY
    COMPILE_DEFINITIONS
      QT_SHA3_KECCAK_COMPAT
)

set_property(
  SOURCE
    DataPath.cpp
  PROPERTY
    COMPILE_DEFINITIONS
      SHARED_DATA_PATH="${SHARED_DATA_PATH}"
)

target_compile_options(Common PUBLIC $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>)
target_compile_options(Common PUBLIC $<$<CXX_COMPILER_ID:MSVC>:-W4>)

target_include_directories (Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Common Qt5::Core)

set (data_files
  Data/earthmap1k.jpg
  Data/data.json
  Data/obj8-lights.json
  Data/obj8-frag.glsl
  Data/obj8-vert.glsl
)

foreach (f IN LISTS data_files)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${f} DESTINATION ${SHARED_DATA_PATH})
endforeach ()

add_subdirectory (QImagePlugins/DDS)
add_subdirectory (Obj8)
add_subdirectory (Classification)
add_subdirectory (Widgets)
