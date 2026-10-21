@echo off
setlocal

echo Building console version...
g++ -std=c++17 main.cpp src\Student.cpp src\Academic.cpp src\Career.cpp src\Dashboard.cpp -Iinclude -o project.exe

if errorlevel 1 (
    echo.
    echo Build failed. Make sure MinGW g++ is installed and available in PATH.
    pause
    exit /b 1
)

echo.
echo Console build successful.
pause
