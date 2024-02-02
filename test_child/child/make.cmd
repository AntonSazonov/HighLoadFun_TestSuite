@echo off
cls
echo.
wsl -e g++ -g -Wall -std=c++17 -O1 -march=haswell -fno-exceptions main.cpp -o main
