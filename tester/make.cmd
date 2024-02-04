@echo off
cls
echo.
wsl -e g++ -g -Wall -std=c++17 -O3 -march=haswell -Wno-multichar -fno-exceptions -DLOCAL main.cpp -o main
wsl -e file ./main
