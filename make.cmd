@echo off
cls
echo.
rem -g
rem 
wsl -e g++ -Wall -std=c++17 -O3 -march=native -fno-stack-protector -fcf-protection=none -fno-exceptions ts_main.cpp -s -o ts_main
rem wsl -e file ./ts_main
