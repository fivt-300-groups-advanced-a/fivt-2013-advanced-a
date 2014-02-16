@echo off
:start
edit_dist_gen.exe 1000 1000 5 > input.txt
edit_dist.exe
edit_dist_checker.exe
if errorlevel 1 goto fail
echo OK
goto start
:fail
echo FAIL :(