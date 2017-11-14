project (Common)

add_library (Common STATIC
  CryptoHash.cpp
  Diagnostics.cpp
  Settings.cpp
)

target_link_libraries (Common
  Qt5::Core
)

add_subdirectory (QImagePlugins/DDS)
add_subdirectory (Obj8)
add_subdirectory (Classification)
add_subdirectory (Widgets)
