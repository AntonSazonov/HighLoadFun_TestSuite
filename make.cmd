@echo off
cls
echo.

rem -g

wsl -e g++ -Wall -std=c++17 -O3 -march=native -fno-exceptions -fno-rtti ts_main.cpp -I. -I./3rd -s -o ts_main
rem wsl -e g++ -Wall -std=c++17 -O3 -march=x86-64 -fno-exceptions -fno-rtti -fno-stack-protector -fcf-protection=none ts_main.cpp -I. -s -o ts_main
rem wsl -e file ./ts_main
