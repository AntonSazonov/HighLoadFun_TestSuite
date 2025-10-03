@echo off
cls
echo.

wsl -e g++ -Wall -std=c++17 -O3 -march=native -fno-exceptions -fno-rtti -fcf-protection=none ts_main.cpp -I. -I./3rd -s -o ts_main_x86-64
