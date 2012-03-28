@echo off

set PATH=%PATH%;%ProgramFiles%\CCCC

dir /s /b ..\..\Sources\*.cpp ..\..\Sources\*.h ..\..\Sources\*.c > files.txt

cccc --outdir=report - < files.txt
