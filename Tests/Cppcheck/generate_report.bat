@echo off

:CPPCHECK_FIND
set CPPCHECK_HOME=%ProgramFiles%\Cppcheck
if exist "%CPPCHECK_HOME%" goto PYTHON_FIND

:CPPCHECK_NOT_FOUND
echo Cppcheck not found. Build failed!
goto END

:PYTHON_FIND
set PYTHON_HOME=C:\Python27
if exist "%PYTHON_HOME%" goto BUILD
set PYTHON_HOME=C:\Python32
if exist "%PYTHON_HOME%" goto BUILD

:PYTHON_NOT_FOUND
echo Python not found. Build failed!
goto END

:BUILD
echo Using Cppcheck found in %CPPCHECK_HOME%
set PATH=%PATH%;%CPPCHECK_HOME%
cppcheck --enable=all --force --suppressions-list=suppressions.txt --xml ../../Sources 2> cppcheck.xml

:HTMLREPORT
set PATH=%PATH%;%PYTHON_HOME%
python "%CPPCHECK_HOME%\htmlreport\cppcheck-htmlreport" --file=cppcheck.xml --report-dir=report --source-dir=.

:END
exit 0
