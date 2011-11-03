@echo off

:SETTINGS
PATH=%PATH;..\..\..\Tools
call ..\..\proxy.bat

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
