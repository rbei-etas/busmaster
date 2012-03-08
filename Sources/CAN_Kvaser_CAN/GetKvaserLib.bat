@ECHO OFF
:: delims is a TAB followed by a space
FOR /F "tokens=2* delims=   " %%A IN ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\KVASER AB\CanlibSDK"') DO SET dir=%%B

copy "%dir:~7%\Lib\MS\canlib32.lib" "%TMP%\canlib32.lib"