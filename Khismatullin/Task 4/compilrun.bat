@echo off
setlocal enabledelayedexpansion
set TPC=C:\compilers\tp7\BIN\TPC.EXE
set BCPP=C:\compilers\BCC55\
set TASMDIR=C:\compilers\TASM\BIN\
set MINGW=C:\MinGW\

if exist prov.exe (
		del prov.exe
	)
	@echo on
	g++.exe prov.cpp -o prov.exe
	@echo off
	if /I "%2"=="-exec" (
		@echo on
		prov.exe
		@echo off
	)
echo.
pause