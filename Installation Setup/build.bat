@echo off

set PATH=%PATH%;%ProgramFiles%\NSIS;%ProgramFiles%\Git\bin

git log > ..\Sources\BIN\Release\Changelog.log
dos2unix --u2d ..\Sources\BIN\Release\Changelog.log

makensis "BUSMASTER INSTALL SCRIPT.nsi"
