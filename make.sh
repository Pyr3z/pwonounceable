#! /bin/bash
# lol was gonna use a makefile, but let's just keep it simple

SRC=("src/3pw.c")
OUT="bin/3pw"

CC=${1:-$(which gcc)}
shift

if [[ $# -eq 0 ]]; then # default gcc [and clang] args
  CCARGS=("-ansi" "-Wall" "-Wextra" "-pedantic")
  CCARGS+=("-Wno-implicit-fallthrough")
  CCARGS+=("-Werror")
else
  CCARGS=$@
fi

mkdir -p ${OUT%/*}

"$CC" ${CCARGS[@]} ${SRC[@]} -o "$OUT"
