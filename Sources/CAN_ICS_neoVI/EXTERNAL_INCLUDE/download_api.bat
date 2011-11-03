@echo off

:SETTINGS
PATH=%PATH;..\..\..\Tools
set http_proxy=proxy:8080
set http_user=
set http_password=

:CHECKS
if exist icsnVC40.h goto END
if exist VCneoVI.zip goto UNPACK

:DOWNLOAD
wget http://intrepidcs.com/support/ICSDocumentation/neoVIDLL/VCneoVI.zip

:UNPACK
cmd
unzip VCneoVI.zip icsnVC40.h

:END
echo Ready.
pause
