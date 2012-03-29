@echo off

:CPPCHECK_FIND
set CPPCHECK_HOME=%ProgramFiles%\Cppcheck
if exist "%CPPCHECK_HOME%" goto BUILD

:CPPCHECK_NOT_FOUND
echo Cppcheck not found. Build failed!
goto END

:BUILD
echo Using Cppcheck found in %CPPCHECK_HOME%
set PATH=%PATH%;%CPPCHECK_HOME%
cppcheck --xml ../../Sources 2> cppcheck.xml

:END
exit 0
