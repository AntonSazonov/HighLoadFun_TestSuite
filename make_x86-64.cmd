@echo off
cls
echo.

wsl -e g++ -I. -I./3rd -Wall -std=c++17 -O3 -march=native -fno-exceptions -fno-rtti -fcf-protection=none -fuse-ld=gold -s ts_main.cpp -o hlf_ts_x86-64
