@echo off

:SETTINGS
PATH=%PATH;..\..\..\Tools
call ..\..\proxy.bat

:CHECKS
if exist vxlapi.h goto END
if exist "C:\XL Driver Library" goto COPY
if exist Setup.exe goto SETUP
if exist xl_lib75.zip goto UNPACK

:DOWNLOAD
wget http://www.vector.com/downloads/drivers/xl_lib75.zip

:UNPACK
unzip xl_lib75.zip

:SETUP
echo "Please install in the standard location 'C:\XL Driver Library'"
Setup.exe

:COPY
copy "C:\XL Driver Library\bin\vxlapi.h" .

:END
echo Ready.
pause
