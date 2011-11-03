@echo off

:SETTINGS
PATH=%PATH;..\..\..\Tools

:CHECKS
if exist ..\..\BIN\Release\canlib32.dll goto END
if exist "C:\Program Files\Kvaser\Canlib" goto COPY
if exist canlib.exe goto SETUP

:DOWNLOAD
echo "Please go to the following site and download the 'Kvaser CANlib SDK'"
echo "http://www.kvaser.com/en/developer/canlib.html"
echo "Press enter when you have copied canlib.exe to 'BUSMASTER\Sources\CAN_Kvaser_CAN\EXTERNAL_INCLUDE'"
pause

:SETUP
echo "Please install in the standard location 'C:\Program Files\Kvaser\Canlib'"
canlib.exe

:COPY
copy "C:\Program Files\Kvaser\Canlib\Bin\canlib32.dll" ..\..\BIN\Release\

:END
echo Ready.
pause
