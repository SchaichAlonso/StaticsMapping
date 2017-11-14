project (DDSImageFormat)

add_definitions (-DQT_STATICPLUGIN)

add_library (
  DDSImageFormat
  STATIC
    ddsheader.cpp
    main.cpp
    qddshandler.cpp
)

target_link_libraries (
  DDSImageFormat
    Qt5::Gui
    Qt5::Core
)
