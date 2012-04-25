@echo off

"%ProgramFiles%\AStyle\bin\AStyle.exe" --options=..\Tests\AStyle\astyle.txt --exclude=EXTERNAL_INCLUDE --recursive *.cpp *.h

pause
