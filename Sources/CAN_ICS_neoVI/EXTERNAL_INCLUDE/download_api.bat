@echo off

:SETTINGS
PATH=%PATH;..\..\..\Tools
set http_proxy=proxy:8080
set http_user=
set http_password=

:CHECKS_H1
if exist icsnVC40.h goto CHECKS_H2

:DOWNLOAD_H1
wget http://intrepidcs.com/support/ICSDocumentation/neoVIDLL/icsnVC40.h

:CHECKS_H2
if exist icsneo40_API.h goto CHECKS_H3

:DOWNLOAD_H2
wget http://intrepidcs.com/support/ICSDocumentation/neoVIDLL/icsneo40_API.h

:CHECKS_H3
if exist icsSpyData.h goto END

:DOWNLOAD_H3
wget http://intrepidcs.com/support/ICSDocumentation/neoVIDLL/icsSpyData.h

:END
echo Ready.
pause
