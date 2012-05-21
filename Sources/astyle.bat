@echo off

"%ProgramFiles%\AStyle\bin\AStyle.exe" --options=..\Tests\AStyle\astyle.txt --exclude=EXTERNAL_INCLUDE --recursive *.c *.cpp *.h *.hpp

pause
