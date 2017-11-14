
add_executable (TagTool
  VisualObjectsModel.cpp
  MainWindow.cpp
  gui.cpp
)

target_link_libraries (
  TagTool
    Classification
    Widgets
    
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
