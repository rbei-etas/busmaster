@echo off

:GIT_FIND
set GIT_HOME=%ProgramFiles%\Git
if exist "%GIT_HOME%\bin\git.exe" goto NSIS_FIND
set GIT_HOME=%ProgramFiles(x86)%\Git
if exist "%GIT_HOME%\bin\git.exe" goto NSIS_FIND

:GIT_NOT_FOUND
echo Git not found. Build failed!
goto END

:NSIS_FIND
set NSIS_HOME=%ProgramFiles%\NSIS
if exist "%NSIS_HOME%\makensis.exe" goto BUILD
set NSIS_HOME=%ProgramFiles(x86)%\NSIS
if exist "%NSIS_HOME%\makensis.exe" goto BUILD

:NSIS_NOT_FOUND
echo NSIS not found. Build failed!
goto END

:BUILD
echo Using git found in %GIT_HOME%
echo Using NSIS found in %NSIS_HOME%
set PATH=%PATH%;%GIT_HOME%\bin;%NSIS_HOME%
git log > ..\Sources\BIN\Release\Changelog.txt
dos2unix --u2d ..\Sources\BIN\Release\Changelog.txt
makensis "CleanRegistry.nsi"
makensis "BUSMASTER INSTALL SCRIPT.nsi"

:END
REM pause
exit 0
