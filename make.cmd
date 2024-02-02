@echo off
cls
echo.
wsl -e g++ -g -Wall -std=c++17 -O3 -march=native -fno-exceptions ts_main.cpp -o ts_main
rem wsl -e file ./ts_main
