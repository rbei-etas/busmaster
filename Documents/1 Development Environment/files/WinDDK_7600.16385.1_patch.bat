@echo off

set PATH=%PATH%;%ProgramFiles (x86)%\Git\bin

patch -p1 -d C:/ < WinDDK_7600.16385.1.diff

pause
