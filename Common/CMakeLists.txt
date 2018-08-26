project (Common)

add_library (Common STATIC
  CryptoHash.cpp
  DataPath.cpp
  Diagnostics.cpp
)

target_link_libraries (Common
  Qt5::Core
)

set_property (
  TARGET
    Common
  PROPERTY
    COMPILE_DEFINITIONS
      QT_SHA3_KECCAK_COMPAT
)

add_subdirectory (QImagePlugins/DDS)
add_subdirectory (Obj8)
add_subdirectory (Classification)
add_subdirectory (Widgets)
