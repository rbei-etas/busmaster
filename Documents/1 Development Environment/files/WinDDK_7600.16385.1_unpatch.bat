@echo off

set PATH=%PATH%;C:\Program Files\Git\bin

patch -p1 -R -d C:/ < WinDDK_7600.16385.1.diff

pause
