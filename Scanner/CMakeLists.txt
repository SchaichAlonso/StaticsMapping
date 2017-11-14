find_package (Qt5 5.6 COMPONENTS REQUIRED Core Gui Widgets)

add_executable (scanner
  ObjectFileInstaller.cpp
  ScanDialog.cpp
  Scanner.cpp
  Worker.cpp
  main.cpp
)

target_link_libraries (scanner
  Classification
  DDSImageFormat
  Obj8
  Qt5::Widgets
  Qt5::Gui
  Qt5::Core
)
