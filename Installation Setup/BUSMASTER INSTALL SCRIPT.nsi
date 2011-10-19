/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/******************************************************************************
  Project       :  BUSMASTER
  FileName      :  BUSMASTER INSTALL SCRIPT.nsi
  Description   :  Source script for BUSMASTER installer.
  Author(s)     :  Arunkumar Karri
  Date Created  :  11/08/2011
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

; Show gradient background
BGGradient 8080C0 0000FF FFFFFF

; Title of this installation
Name "BUSMASTER Installer"

BrandingText "RBEI BUSMASTER Installation"

; Do a CRC check when initializing setup
CRCCheck On

; Output filename
Outfile "BUSMASTER Installer.exe"

Function .onInit
    # the plugins dir is automatically deleted when the installer exits
    InitPluginsDir  
    File /oname=$PLUGINSDIR\Splsh16.bmp "..\Sources\BIN\Release\Splsh16.bmp"
    #optional
    #File /oname=$PLUGINSDIR\splash.wav "C:\myprog\sound.wav"

    advsplash::show 1500 800 700 -1 $PLUGINSDIR\Splsh16

    Pop $0 ; $0 has '1' if the user closed the splash screen early,
           ;        '0' if everything closed normally, and
           ;       '-1' if some error occurred.
FunctionEnd

; The default installation folder
InstallDir "$PROGRAMFILES\BUSMASTER"
; Uninstall info registry location
InstallDirRegKey HKLM "SOFTWARE\BUSMASTER" "Install_Dir"

; Folder selection prompt
DirText "Please select an installation folder."

; Section Default
Section ""

    SetOutPath $INSTDIR\MinGW\bin
    File ..\Sources\BIN\Release\MinGW\bin\*.*

    SetOutPath $INSTDIR\MinGW\Include
    File ..\Sources\BIN\Release\MinGW\Include\*.*

    SetOutPath $INSTDIR\MinGW\Include\g++-3
    File ..\Sources\BIN\Release\MinGW\Include\g++-3\*.*

    SetOutPath $INSTDIR\MinGW\Include\g++-3\std
    File ..\Sources\BIN\Release\MinGW\Include\g++-3\std\*.*

    SetOutPath $INSTDIR\MinGW\Include\sys
    File ..\Sources\BIN\Release\MinGW\Include\sys\*.*

    SetOutPath $INSTDIR\MinGW\lib
    File ..\Sources\BIN\Release\MinGW\lib\*.*

    SetOutPath $INSTDIR\MinGW\lib\gcc-lib\mingw32\2.95.3-6
    File ..\Sources\BIN\Release\MinGW\lib\gcc-lib\mingw32\2.95.3-6\*.*

    SetOutPath $INSTDIR\MinGW\lib\gcc-lib\mingw32\2.95.3-6\include
    File ..\Sources\BIN\Release\MinGW\lib\gcc-lib\mingw32\2.95.3-6\include\*.*

    SetOutPath $INSTDIR\MinGW\lib\gcc-lib\mingw32\2.95.3-6\include\objc
    File ..\Sources\BIN\Release\MinGW\lib\gcc-lib\mingw32\2.95.3-6\include\objc\*.*

    SetOutPath $INSTDIR\MinGW\man\man1
    File ..\Sources\BIN\Release\MinGW\man\man1\*.*

    SetOutPath $INSTDIR\MinGW\mingw32\include
    File ..\Sources\BIN\Release\MinGW\mingw32\include\*.*

    SetOutPath $INSTDIR\MinGW\mingw32\lib
    File ..\Sources\BIN\Release\MinGW\mingw32\lib\*.*

    SetOutPath $INSTDIR\MinGW\mingw32\lib\ldscripts
    File ..\Sources\BIN\Release\MinGW\mingw32\lib\ldscripts\*.*

    SetOutPath $INSTDIR\MinGW\OBJ
    File ..\Sources\BIN\Release\MinGW\OBJ\*.*

    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\*.exe
    File ..\Sources\BIN\Release\*.dll
    File /oname=BUSMASTER.chm "..\Documents\4 Help\out\help.chm"
    File ..\Sources\BIN\Release\BUSMASTER.tlb
    File ..\Sources\BIN\Release\BUSMASTER_Interface.c
    File ..\Sources\BIN\Release\BUSMASTER_Interface.h
    File ..\Sources\BIN\Release\GCCDLLMakeTemplate_CAN	
    File ..\Sources\BIN\Release\Language_Converter.exe
    File ..\Sources\BIN\Release\CANDBConverter_Import.exe
    File ..\Sources\BIN\Release\CANDBConverter_Export.exe
	File ..\Sources\BIN\Release\Changelog.txt	
    Delete "$INSTDIR\Splsh16.bmp"

    CreateDirectory "$SMPROGRAMS\BUSMASTER"
    CreateShortCut "$SMPROGRAMS\BUSMASTER\BUSMASTER.lnk" "$INSTDIR\BUSMASTER.exe" "" "$INSTDIR\BUSMASTER.exe" 0
    CreateShortCut "$SMPROGRAMS\BUSMASTER\Uninstall.lnk" "$INSTDIR\uninst.exe" "" "$INSTDIR\uninst.exe" 0

SectionEnd

; This emptily named section will always run
Section ""
    WriteRegStr HKLM "Software\BUSMASTER" "Install_Dir" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER" "DisplayName" "BUSMASTER (remove only)"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER" "UninstallString" '"$INSTDIR\uninst.exe"'

    ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion

    ;This key should be written in case of Windows 7 OS
    StrCmp $1 "6.1" 0 lbl            ;StrCmp str1 str2 jump_if_equal [jump_if_not_equal]
    WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe" "WIN98"
    WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe" "WIN98"

    lbl: SetOutPath $INSTDIR
    ExecWait 'BusEmulation.exe /regserver'
    ExecWait 'BUSMASTER.exe /regserver'
    WriteUninstaller "uninst.exe"
SectionEnd

; Uninstall section here...
UninstallText "This will uninstall BUSMASTER Installer. Press NEXT to continue."
Section "Uninstall"
    SetOutPath $INSTDIR
    ExecWait 'BusEmulation.exe /unregserver'
    ExecWait 'BUSMASTER.exe /unregserver'
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER"
    Delete "$INSTDIR\ConfigDialogsDIL.dll"
    Delete "$INSTDIR\DIL_Interface.dll"
    Delete "$INSTDIR\Filter.dll"
    Delete "$INSTDIR\FrameProcessor.dll"
    Delete "$INSTDIR\NodeSimEx.dll"
    Delete "$INSTDIR\ProjectConfiguration.dll"
    Delete "$INSTDIR\PSDI_CAN.dll"
    Delete "$INSTDIR\Replay.dll"
    Delete "$INSTDIR\SignalWatch.dll"
    Delete "$INSTDIR\TestSetupEditorGUI.dll"
    Delete "$INSTDIR\TestSuiteExecutorGUI.dll"
    Delete "$INSTDIR\TXWindow.dll"
    Delete "$INSTDIR\BUSMASTER.exe"
    Delete "$INSTDIR\BusEmulation.exe"
    Delete "$INSTDIR\CAN_ETAS_BOA.dll"
    Delete "$INSTDIR\CAN_ICS_neoVI.dll"
    Delete "$INSTDIR\CAN_Kvaser_CAN.dll"
    Delete "$INSTDIR\CAN_PEAK_USB.dll"
    Delete "$INSTDIR\CAN_STUB.dll"
    Delete "$INSTDIR\CAN_Vector_XL.dll"
    Delete "$INSTDIR\BUSMASTER.tlb"
    Delete "$INSTDIR\BUSMASTER_Interface.h"
    Delete "$INSTDIR\BUSMASTER_Interface.c"
    Delete "$INSTDIR\GCCDLLMakeTemplate_CAN"
    Delete "$INSTDIR\CANDBConverter_Import.exe"
    Delete "$INSTDIR\CANDBConverter_Export.exe"
    Delete "$INSTDIR\Language_Converter.exe"
    Delete "$INSTDIR\BUSMASTER.chm"
    Delete "$INSTDIR\MinGW\bin\*.*"
    Delete "$INSTDIR\MinGW\Include\*.*"
    Delete "$INSTDIR\MinGW\Include\g++-3\*.*"
    Delete "$INSTDIR\MinGW\Include\g++-3\std\*.*"
    Delete "$INSTDIR\MinGW\Include\sys\*.*"	
    Delete "$INSTDIR\MinGW\lib\*.*"
    Delete "$INSTDIR\MinGW\lib\gcc-lib\mingw32\2.95.3-6\*.*"	
    Delete "$INSTDIR\MinGW\lib\gcc-lib\mingw32\2.95.3-6\include\*.*"
    Delete "$INSTDIR\MinGW\lib\gcc-lib\mingw32\2.95.3-6\include\objc\*.*"	
    Delete "$INSTDIR\MinGW\man\man1\*.*"
    Delete "$INSTDIR\MinGW\mingw32\include\*.*"
    Delete "$INSTDIR\MinGW\mingw32\lib\*.*"
    Delete "$INSTDIR\MinGW\mingw32\lib\ldscripts\*.*"	
    Delete "$INSTDIR\MinGW\OBJ\*.*"
    Delete "$INSTDIR\uninst.exe"
    Delete "$INSTDIR\CanApi2.dll"
	Delete "$INSTDIR\Changelog.txt"	

    Delete "$SMPROGRAMS\BUSMASTER\Uninstall.lnk"
    Delete "$SMPROGRAMS\BUSMASTER\BUSMASTER.lnk"

    RmDir "$INSTDIR\MinGW\bin"
    RmDir "$INSTDIR\MinGW\Include\g++-3\std"
    RmDir "$INSTDIR\MinGW\Include\g++-3"
    RmDir "$INSTDIR\MinGW\Include\sys"	
    RmDir "$INSTDIR\MinGW\Include"
    RmDir "$INSTDIR\MinGW\lib\gcc-lib\mingw32\2.95.3-6\include\objc"	
    RmDir "$INSTDIR\MinGW\lib\gcc-lib\mingw32\2.95.3-6\include"
    RmDir "$INSTDIR\MinGW\lib\gcc-lib\mingw32\2.95.3-6"	
    RmDir "$INSTDIR\MinGW\lib\gcc-lib\mingw32"
    RmDir "$INSTDIR\MinGW\lib\gcc-lib"
    RmDir "$INSTDIR\MinGW\lib"
    RmDir "$INSTDIR\MinGW\man\man1"
    RmDir "$INSTDIR\MinGW\man"
    RmDir "$INSTDIR\MinGW\mingw32\include"	
    RmDir "$INSTDIR\MinGW\mingw32\lib\ldscripts"
    RmDir "$INSTDIR\MinGW\mingw32\lib"
    RmDir "$INSTDIR\MinGW\mingw32"
    RmDir "$INSTDIR\MinGW\OBJ"
    RmDir "$INSTDIR\MinGW"
    RmDir "$INSTDIR"
SectionEnd
