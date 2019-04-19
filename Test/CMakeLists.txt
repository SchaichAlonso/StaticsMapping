project (Tests)

add_library (
  TestHelpers
  STATIC
  DefinitionsHelper.cpp
)

target_link_libraries (
  TestHelpers
  Classification
  Obj8
)

set (test_list
  Afiliates
  Definitions
  Models
  Mnemonics
  Parser
  XPClass
)

set (test_libs
  TestHelpers
  Classification
  Qt5::Test
)

foreach (test ${test_list})
  add_executable (${test} ${test}.cpp)
  target_link_libraries (${test} ${test_libs})
  add_test (NAME ${test} COMMAND ${test})
endforeach ()
