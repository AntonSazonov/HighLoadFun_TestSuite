@echo off
cls
echo.

set FLAGS=-std=c++17 -O3 -march=haswell -masm=intel -fno-exceptions -fno-rtti -fno-stack-protector -fcf-protection=none
set FLAGS_CLANG=-mllvm -unroll-count=0

wsl -e clang++ -g %FLAGS% %FLAGS_CLANG% main.cpp -o main
