@echo off

:CCCC_FIND
set CCCC_HOME=%ProgramFiles%\CCCC
if exist "%CCCC_HOME%" goto BUILD

:CCCC_NOT_FOUND
echo CCCC not found. Build failed!
goto END

:BUILD
echo Using CCCC found in %CCCC_HOME%
set PATH=%PATH%;%CCCC_HOME%
dir /s /b ..\..\Sources\*.cpp ..\..\Sources\*.h ..\..\Sources\*.c > files.txt
cccc --outdir=report - < files.txt

:END