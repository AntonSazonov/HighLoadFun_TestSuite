@echo off
cls
echo.

rem  -A  autonomous mode. IDA will not display dialog boxes. Designed to be used together with -S switch.

start "" /B "C:\Program Files\IDA 7.7\ida64.exe" -A -c main
