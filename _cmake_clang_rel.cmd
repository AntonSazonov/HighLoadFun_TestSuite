@echo off
cls
echo.
wsl -e cmake -B _build_clang_rel . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release && ^
wsl -e cmake --build _build_clang_rel
if %errorlevel% NEQ 0 ( del /f ts_main 2> nul )
