@echo off

:SETTINGS
PATH=%PATH;..\..\..\Tools
call ..\..\proxy.bat

:CHECKS
if exist ..\..\BIN\Release\icsneo40.dll goto END
if exist C:\Windows\System32\icsneo40.dll goto COPY
if exist vspy3_Free.EXE goto SETUP
if exist vspy3_Free.zip goto UNPACK

:DOWNLOAD
wget http://intrepidcs.com/modules/CmodsDownload/upload/Software/vspy3_Free.zip

:UNPACK
unzip vspy3_Free.zip

:SETUP
echo "Please install in the standard location 'C:\Program Files\Vehicle Spy 3'"
vspy3_Free.exe

:COPY
copy C:\Windows\System32\icsneo40.dll ..\..\BIN\Release

:END
echo Ready.
pause
