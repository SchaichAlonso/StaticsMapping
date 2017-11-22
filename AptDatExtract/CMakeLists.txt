add_executable (AptDatExtract AptDatExtract.cpp)

target_link_libraries (AptDatExtract
  Classification
  Widgets
  Qt5::Widgets
)
