#!/bin/sh

set -e

# LLVM_VERSION=${LLVM_VERSION:=40};
# LLVM_PROFDATA=${LLVM_PROFDATA:=llvm-profdata${LLVM_VERSION}}
# LLVM_COV=${LLVM_COV:=llvm-cov${LLVM_VERSION}}

if test -n "${1}"; then
  LLVM_PROFILE_FILE=${1}.profraw ${1}
  @LLVM_PROFDATA@ merge ${1}.profraw -o ${1}.profdata
  @LLVM_COV@ show -format=html -instr-profile ${1}.profdata ${1} > ${1}.html
  @LLVM_COV@ report -instr-profile ${1}.profdata ${1} > ${1}.txt
else
  @LLVM_PROFDATA@ merge *.profraw -o merged.profdata
  @LLVM_COV@ show -format=html -instr-profile merged.profdata $(echo "@test_list@" | sed "s/\;/ /g") > Report.html
  @LLVM_COV@ report -instr-profile merged.profdata $(echo "@test_list@" | sed "s/\;/ /g") > Report.txt
fi
