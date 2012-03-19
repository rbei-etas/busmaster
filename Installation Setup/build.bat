@echo off

set PATH=%PATH%;%ProgramFiles%\NSIS;%ProgramFiles%\Git\bin

git log > ..\Sources\BIN\Release\Changelog.txt
dos2unix --u2d ..\Sources\BIN\Release\Changelog.txt

makensis "BUSMASTER INSTALL SCRIPT.nsi"
