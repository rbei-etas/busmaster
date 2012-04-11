@echo off

set PATH=%PATH%;%ProgramFiles%\AutoIt3

REM Installer
cd Installer
set TEST=TS_XX_install_full
echo %TEST%
call autoit3 %TEST%.au3 > %TEST%_out.txt 2> %TEST%_err.txt
cd ..

REM Other Tests


REM Uninstaller
cd Installer
set TEST=TS_XX_uninstall
echo %TEST%
call autoit3 %TEST%.au3 > %TEST%_out.txt 2> %TEST%_err.txt
cd ..
