@echo off

set PATH=%PATH%;%ProgramFiles%\AutoIt3


:INSTALL
set TESTGROUP=Installer
echo %TESTGROUP%
cd %TESTGROUP%

set TEST=TS_XX_install_full
echo   %TEST%
call autoit3 %TEST%.au3 > %TEST%_out.txt 2> %TEST%_err.txt

cd ..


:TESTS
set TESTGROUP=Format Converter
echo %TESTGROUP%
cd %TESTGROUP%

set TEST=TS_XX_DBC2DBF_001
echo   %TEST%
call autoit3 %TEST%.au3 > %TEST%_out.txt 2> %TEST%_err.txt

cd ..


:UNINSTALL
set TESTGROUP=Installer
echo %TESTGROUP%
cd %TESTGROUP%

set TEST=TS_XX_uninstall
echo   %TEST%
call autoit3 %TEST%.au3 > %TEST%_out.txt 2> %TEST%_err.txt

cd ..


:END
