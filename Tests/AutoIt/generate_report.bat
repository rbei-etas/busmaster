@echo off

set PATH=%PATH%;%ProgramFiles%\AutoIt3

cd Installer
autoit3 TS_XX_001.au3 > TS_XX_001_out.txt 2> TS_XX_001_err.txt
