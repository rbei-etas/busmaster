@echo off

:SETTINGS
REM --- Settings ---
PATH=%PATH;..\..\..\Tools
set http_proxy=proxy:8080
set http_user=
set http_password=

:CHECKS
if exist vxlapi.h goto END
if exist "C:\XL Driver Library" goto COPY
if exist Setup.exe goto SETUP
if exist xl_lib75.zip goto UNZIP

:DOWNLOAD
REM --- Download ---
wget http://www.vector.com/downloads/drivers/xl_lib75.zip

:UNZIP
REM --- Unzip ---
unzip xl_lib75.zip

:SETUP
REM --- Setup ---
echo "Please install in the standard location 'C:\XL Driver Library'"
Setup.exe

:COPY
REM --- Copy to BUSMASTER ---
copy "C:\XL Driver Library\bin\vxlapi.h" .

:END
