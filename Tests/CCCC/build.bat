@echo off

:CCCC_FIND
set CCCC_HOME=%ProgramFiles%\CCCC
if exist "%CCCC_HOME%" goto FILELIST

:CCCC_NOT_FOUND
echo CCCC not found. Build failed!
goto END

:FILELIST
echo Generating filelist...
dir /s /b ..\..\Sources\*.cpp ..\..\Sources\*.h ..\..\Sources\*.c > files0.txt
find /v "EXTERNAL_INCLUDE" < files0.txt > files1.txt
find /v "BIN\Release" < files1.txt > files2.txt
find /v "~" < files2.txt > files.txt
del files0.txt
del files1.txt
del files2.txt

:BUILD
echo Using CCCC found in %CCCC_HOME%
set PATH=%PATH%;%CCCC_HOME%
cccc --outdir=report - < files.txt

:END
exit 0
