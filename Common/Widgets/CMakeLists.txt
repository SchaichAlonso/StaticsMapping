find_package (Qt5 5.6 COMPONENTS REQUIRED Core Gui Widgets OpenGL)
find_package (OpenGL REQUIRED)

include_directories (${OPENGL_INCLUDE_DIR})

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
  ObjScreen.cpp
  ObjView.cpp
  OpenGLTexture.cpp
  OpenGLWidget.cpp
  TableDialog.cpp
  VisualModel.cpp
  VisualObject.cpp
)

set (Widgets_LIBS
  Classification
  DDSImageFormat
  Obj8
  
  Qt5::Widgets
  Qt5::Gui
  Qt5::OpenGL
  ${OPENGL_LIBRARIES}
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
