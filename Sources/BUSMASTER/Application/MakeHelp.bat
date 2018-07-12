@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CAN_MONITOR.HPJ. >"hlp\CAN_Monitor.hm"
echo. >>"hlp\CAN_Monitor.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\CAN_Monitor.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\CAN_Monitor.hm"
echo. >>"hlp\CAN_Monitor.hm"
echo // Prompts (IDP_*) >>"hlp\CAN_Monitor.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\CAN_Monitor.hm"
echo. >>"hlp\CAN_Monitor.hm"
echo // Resources (IDR_*) >>"hlp\CAN_Monitor.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\CAN_Monitor.hm"
echo. >>"hlp\CAN_Monitor.hm"
echo // Dialogs (IDD_* ) >>"hlp\CAN_Monitor.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\CAN_Monitor.hm"
echo. >>"hlp\CAN_Monitor.hm"
echo // Frame Controls (IDW_*) >>"hlp\CAN_Monitor.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\CAN_Monitor.hm"
echo // Dlg Controls (IDC_*) >>"hlp\CAN_Monitor.hm"
makehm HIDC_,HIDC_,0x0 resource.hm >>"hlp\CAN_Monitor.hm"
echo. >>"hlp\CAN_Monitor.hm"
REM -- Make help for Project CAN_MONITOR


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\CAN_Monitor.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\CAN_Monitor.hlp" goto :Error
if not exist "hlp\CAN_Monitor.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\CAN_Monitor.hlp" Debug
if exist Debug\nul copy "hlp\CAN_Monitor.cnt" Debug
if exist Release\nul copy "hlp\CAN_Monitor.hlp" Release
if exist Release\nul copy "hlp\CAN_Monitor.cnt" Release
echo.
goto :done

:Error
echo hlp\CAN_Monitor.hpj(1) : error: Problem encountered creating help file

:done
echo.
