@echo off
cls
echo.
rem -G Ninja
wsl -e cmake -B _build_clang_dbg . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug && ^
wsl -e cmake --build _build_clang_dbg
if %errorlevel% NEQ 0 ( del /f ts_main 2> nul )
