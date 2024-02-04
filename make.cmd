@echo off
cls
echo.
rem -g
rem 
wsl -e g++ -Wall -std=c++17 -O3 -mavx2 -mbmi -mbmi2 -mpopcnt -mlzcnt -march=native -funroll-loops -fno-stack-protector -fcf-protection=none -fno-exceptions ts_main.cpp -s -o ts_main
rem wsl -e file ./ts_main
