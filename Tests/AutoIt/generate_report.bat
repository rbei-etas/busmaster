@echo off

REM --- Initial setup ---
set PATH=%PATH%;%ProgramFiles%\AutoIt3
call:htmlheader
cd Installer


REM --- List of tests to be executed ---
call:testgroup "Installer"
call:test "TS_XX_install_full"

call:testgroup "Format Converter"
call:test "TS_XX_DBC2DBF_001"

call:testgroup "Installer"
call:test "TS_XX_uninstall"


REM --- Final setup ---
cd ..
call:htmlfooter


REM --- Utility functions ---
:testgroup
set TESTGROUP=%~1
cd ..
echo %TESTGROUP%
cd %TESTGROUP%
goto:eof

:test
set TEST=%~1
echo   %TEST%
call autoit3 %TEST%.au3 > %TEST%_out.txt 2> %TEST%_err.txt
call:htmltestresult
goto:eof

:htmlheader
echo ^<html^>^<head^>                                                            >     report.html
echo ^<meta http-equiv="content-type" content="text/html; charset=ISO-8859-1"^>  >>    report.html
echo ^<title^>BUSMASTER AutoIt Test Report^</title^>                             >>    report.html
echo ^</head^>                                                                   >>    report.html
echo ^<body^>                                                                    >>    report.html
echo ^<img src="../../Documents/Logo/BUSMASTER_Logo_311x64.png"^>                >>    report.html
echo ^<h1^>BUSMASTER AutoIt test report^</h1^>                                   >>    report.html
echo ^<table border="1"^>^<tbody^>                                               >>    report.html
echo ^<tr^>                                                                      >>    report.html
echo ^<th^>Test group^</th^>                                                     >>    report.html
echo ^<th^>Test^</th^>                                                           >>    report.html
echo ^<th^>Output^</th^>                                                         >>    report.html
echo ^<th^>Errors^</th^>                                                         >>    report.html
echo ^<th^>Status^</th^>                                                         >>    report.html
echo ^</tr^>                                                                     >>    report.html
goto:eof

:htmltestresult
echo ^<tr^>                                                                      >> ..\report.html
echo ^<td^>%TESTGROUP%^</td^>                                                    >> ..\report.html
echo ^<td^>%TEST%^</td^>                                                         >> ..\report.html
echo ^<td^>^<a href="%TESTGROUP%/%TEST%_out.txt"^>X^</a^>^</td^>                 >> ..\report.html
echo ^<td^>^<a href="%TESTGROUP%/%TEST%_err.txt"^>X^</a^>^</td^>                 >> ..\report.html
for %%R in (%TEST%_err.txt) do if %%~zR equ 0 goto :pass
echo ^<td style='background-color:#FF7777'^>Fail^</a^>^</td^>                    >> ..\report.html
goto :done
:pass
echo ^<td style='background-color:#77FF77'^>Pass^</a^>^</td^>                    >> ..\report.html
:done
echo ^</tr^>                                                                     >> ..\report.html
goto:eof

:htmlfooter
echo ^</tbody^>^</table^>                                                        >>    report.html
echo ^</body^>^</html^>                                                          >>    report.html
goto:eof
