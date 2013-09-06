@echo off

:ASTYLE_FIND
set ASTYLE_HOME=%ProgramFiles%\AStyle
if exist "%ASTYLE_HOME%\bin\AStyle.exe" goto BUILD
set ASTYLE_HOME=%ProgramFiles(x86)%\AStyle
if exist "%ASTYLE_HOME%\bin\AStyle.exe" goto BUILD

:ASTYLE_NOT_FOUND
echo AStyle not found. Build failed!
goto END

:BUILD
echo Using AStyle found in %ASTYLE_HOME%
"%ASTYLE_HOME%\bin\AStyle.exe" --options=astyle.txt --exclude=EXTERNAL --recursive *.c *.cpp *.h *.hpp

:END
exit 0
