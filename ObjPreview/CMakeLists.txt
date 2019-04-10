
add_executable (ObjPreview
  main.cpp
)

target_link_libraries (
  ObjPreview
    Widgets
    
    Qt5::Widgets
    Qt5::Gui
)

install(TARGETS ObjPreview RUNTIME DESTINATION "bin")

