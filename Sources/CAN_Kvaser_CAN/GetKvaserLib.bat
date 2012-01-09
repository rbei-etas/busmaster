@ECHO OFF
:: delims is a TAB followed by a space
FOR /F "tokens=2* delims=	 " %%A IN ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\KVASER AB\CanlibSDK"') DO SET Country=%%B
set str=%Country%
set str=%str:~7%
set str=%str%\Lib\MS\canlib32.lib
echo.%str%
copy "%str%" "%TMP%\canlib32.lib"
