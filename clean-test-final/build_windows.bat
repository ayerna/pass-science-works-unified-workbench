@echo off
REM Native Windows build (run in the project folder, no admin rights needed).
REM Requires: CMake and a C++ compiler (Visual Studio Build Tools or MinGW) on PATH.
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b 1
cmake --build build --config Release -j 4
if errorlevel 1 exit /b 1
REM Copy the built binaries next to the frontend for the one-click package.
copy /Y build\Release\workbench.exe . 2>nul
copy /Y build\workbench.exe . 2>nul
copy /Y build\Release\Launch.exe . 2>nul
copy /Y build\Launch.exe . 2>nul
workbench.exe all
echo.
echo Done. Double-click Launch.exe to open the workbench.
echo To package: powershell -ExecutionPolicy Bypass -File scripts\package_windows.ps1
