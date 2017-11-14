
add_executable (ObjPreview
  ObjPreview.cpp
  main.cpp
)

target_link_libraries (
  ObjPreview
    Widgets
    
    Qt5::Widgets
    Qt5::Gui
)
