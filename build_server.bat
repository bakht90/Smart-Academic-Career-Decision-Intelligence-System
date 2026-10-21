@echo off
setlocal

echo Building Smart Academic & Career Decision Intelligence System server...
g++ -std=c++17 server.cpp src\Student.cpp src\Academic.cpp src\Career.cpp src\Dashboard.cpp -Iinclude -lws2_32 -o server.exe

if errorlevel 1 (
    echo.
    echo Build failed. Make sure MinGW g++ is installed and available in PATH.
    pause
    exit /b 1
)

echo.
echo Server build successful.
echo Run server.exe and open http://localhost:18080
pause
