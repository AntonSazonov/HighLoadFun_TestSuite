@echo off
cls
echo.

rem wsl -e llvm-mca -march=x86-64 -mcpu=znver1 -timeline -bottleneck-analysis -iterations=1 main.s > _mca.log
wsl -e llvm-mca -march=x86-64 -mcpu=haswell -timeline -bottleneck-analysis -iterations=1 main.s > _mca.log

rem wsl -e clang -g %FLAGS% %FLAGS_CLANG% main.cpp -o main
