@echo off
setlocal enabledelayedexpansion
set MINGW=C:\MinGW\

if exist Gtest.exe (
		del Gtest.exe
	)
@echo on
g++ Gtest.cpp C:\MinGW\gtestlib\libgtest.dll -o Gtest.exe
Gtest.exe
@echo off

)
echo.
pause