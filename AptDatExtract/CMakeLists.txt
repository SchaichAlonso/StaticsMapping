add_executable (AptDatExtract AptDatExtract.cpp)

target_link_libraries (AptDatExtract
  Classification
  Qt5::Core
)
