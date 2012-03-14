@ECHO OFF
:: delims is a TAB followed by a space
FOR /F "tokens=2* delims=   " %%A IN ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\KVASER AB\CanlibSDK"') DO SET DirPath=%%B
set str=%DirPath%
::check for Windows Version 
systeminfo | find "OS Name" > %TEMP%\osname.txt
FOR /F "usebackq delims=: tokens=2" %%i IN (%TEMP%\osname.txt) DO set vers=%%i

echo %vers% | find "Windows XP" > nul
if %ERRORLEVEL% == 0 set str=%str:~7%

copy "%str%\Lib\MS\canlib32.lib" "%TMP%\canlib32.lib"