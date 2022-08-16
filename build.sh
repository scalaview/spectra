#/bin/bash
export PREFIX="$HOME/opt/cross_64"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"
make clean
make all