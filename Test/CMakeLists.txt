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


enable_testing ()

if (LLVM_FOUND)
  string(REPLACE "llvm-config" "llvm-profdata" LLVM_PROFDATA ${LLVM_CONFIG_EXECUTABLE})
  string(REPLACE "llvm-config" "llvm-cov" LLVM_COV ${LLVM_CONFIG_EXECUTABLE})
  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/coverage.sh ${CMAKE_CURRENT_BINARY_DIR}/coverage.sh @ONLY)
endif ()

foreach (test ${test_list})
  add_executable (${test} ${test}.cpp)
  target_link_libraries (${test} ${test_libs})
  
  if (LLVM_FOUND)
    add_test (NAME ${test} COMMAND sh coverage.sh ./${test})
  else ()
    add_test (NAME ${test} COMMAND ./${test})
  endif ()
endforeach ()
