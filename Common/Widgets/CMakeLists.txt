set (Widgets_SOURCES
  AircraftDelegate.cpp
  AircraftTableDialog.cpp
  AirlineDelegate.cpp
  AirlineTableDialog.cpp
  AirportDataQueryDialog.cpp
  Application.cpp
  FilenameLineEdit.cpp
  GlobalDistributionDialog.cpp
  GlobalDistributionWidget.cpp
  LibraryTableDialog.cpp
  ListDelegate.cpp
  ObjectTableDialog.cpp
  OpenGL/Camera/Camera.cpp
  OpenGL/Camera/OnScreenDisplay.cpp
  OpenGL/Camera/OrthoCamera.cpp
  OpenGL/Camera/PerspectiveCamera.cpp
  OpenGL/Camera/ZoomableCamera.cpp
  OpenGL/State/DepthMask.cpp
  OpenGL/State/PolygonMode.cpp
  OpenGL/API.cpp
  OpenGL/BufferObject.cpp
  OpenGL/Context.cpp
  OpenGL/DrawElementsObj8.cpp
  OpenGL/DrawElements.cpp
  OpenGL/FrameBufferObject.cpp
  OpenGL/Light.cpp
  OpenGL/Mesh.cpp
  OpenGL/Model.cpp
  OpenGL/Object.cpp
  OpenGL/Obj8NamedLightFactory.cpp
  OpenGL/Obj8Lod.cpp
  OpenGL/Obj8Visitor.cpp
  OpenGL/Obj8Shader.cpp
  OpenGL/Obj8Scene.cpp
  OpenGL/RawImage.cpp
  OpenGL/Scene.cpp
  OpenGL/Screen.cpp
  OpenGL/Shader.cpp
  OpenGL/Texture.cpp
  TableDialog.cpp
  VisualObject.cpp
)

set (Widgets_LIBS
  Classification
  DDSImageFormat
  Obj8
  
  Qt5::Widgets
  Qt5::Gui
)

find_package (GraphViz)

if (GraphViz_FOUND)
  include_directories (${graphviz_INCLUDE_DIRECTORIES})
  set (Widgets_SOURCES
    ${Widgets_SOURCES}
    AirlineGraphWidget.cpp
    AirlineScreen.cpp
    GraphBackend.cpp
    GraphWidget.cpp
  )
  set (Widgets_LIBS
    ${Widgets_LIBS}
    ${graphviz_LIBRARIES}
  )
endif (GraphViz_FOUND)

add_library (Widgets STATIC
  ${Widgets_SOURCES}
)

target_include_directories (Widgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (Widgets
  ${Widgets_LIBS}
)

if (GraphViz_FOUND)
  set_property (
    TARGET
      Widgets
    PROPERTY
      INTERFACE_COMPILE_DEFINITIONS
        HAVE_GRAPHS=1
  )
endif (GraphViz_FOUND)
