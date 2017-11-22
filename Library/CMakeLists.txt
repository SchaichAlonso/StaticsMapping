add_executable (library
  library.cpp
)

target_link_libraries (library
  Classification
)

add_executable (LibraryWizard
  AfiliatesPage.cpp
  IntroPage.cpp
  ObjViewDialog.cpp
  ObjFileInstaller.cpp
  PreviewablesModel.cpp
  ScanPage.cpp
  Scanner.cpp
  Wizard.cpp
  Worker.cpp
  main.cpp
)

target_link_libraries (LibraryWizard
  Classification
  Widgets
  Qt5::Widgets
)
