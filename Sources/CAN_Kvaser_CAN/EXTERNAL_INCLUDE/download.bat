@echo off

:SETTINGS
REM --- Settings ---
PATH=%PATH;..\..\..\Tools

:CHECKS
if exist canlib.h goto END
if exist "C:\Program Files\Kvaser\Canlib" goto COPY
if exist canlib.exe goto SETUP

:DOWNLOAD
REM --- Download ---
echo "Please go to the following site and download canlib.exe"
echo "http://www.kvaser.com/index.php?option=com_php&Itemid=288&swprod=fb35b0d68f9075389577881fa741ce63"
echo "Press enter when you have copied canlib.exe to 'BUSMASTER\Sources\CAN_Kvaser_CAN\EXTERNAL_INCLUDE'"
pause

:SETUP
REM --- Setup ---
echo "Please install in the standard location 'C:\Program Files\Kvaser\Canlib'"
canlib.exe

:COPY
REM --- Copy to BUSMASTER ---
copy "C:\Program Files\Kvaser\Canlib\INC\canevt.h" .
copy "C:\Program Files\Kvaser\Canlib\INC\canlib.h" .
copy "C:\Program Files\Kvaser\Canlib\INC\canstat.h" .
copy "C:\Program Files\Kvaser\Canlib\INC\predef.h" .
copy "C:\Program Files\Kvaser\Canlib\INC\extras\poppack.h" .
copy "C:\Program Files\Kvaser\Canlib\INC\extras\pshpack1.h" .

:END
