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

/**
 * \brief     Source script for BUSMASTER installer
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 * \date      11/08/2011
 *
 * Source script for BUSMASTER installer.
 */

; Show gradient background
BGGradient 8080C0 0000FF FFFFFF

; Title of this installation
Name "BUSMASTER Installer"

BrandingText "RBEI BUSMASTER Installation"

; Do a CRC check when initializing setup
CRCCheck On

; Output filename
Outfile "BUSMASTER_Installer_Ver_1.5.0.exe"

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

; Section Default: This emptily named section will always run
Section ""
    SetOutPath $INSTDIR

    ; BUSMASTER
    File ..\Sources\BIN\Release\BusEmulation.exe
    File ..\Sources\BIN\Release\BUSMASTER.exe
    File ..\Sources\BIN\Release\BUSMASTER.tlb
    File ..\Sources\BIN\Release\BUSMASTER_Interface.c
    File ..\Sources\BIN\Release\BUSMASTER_Interface.h
    File ..\Sources\BIN\Release\CAN_ETAS_BOA.dll
    File ..\Sources\BIN\Release\CAN_ICS_neoVI.dll
    File ..\Sources\BIN\Release\CAN_Kvaser_CAN.dll
    File ..\Sources\BIN\Release\CAN_PEAK_USB.dll
    File ..\Sources\BIN\Release\CAN_STUB.dll
    File ..\Sources\BIN\Release\CAN_Vector_XL.dll
    File ..\Sources\BIN\Release\Changelog.txt
    File ..\Sources\BIN\Release\DIL_Interface.dll
    File ..\Sources\BIN\Release\Filter.dll
    File ..\Sources\BIN\Release\FrameProcessor.dll
    File ..\Sources\BIN\Release\GCCDLLMakeTemplate_CAN
    File ..\Sources\BIN\Release\NodeSimEx.dll
    File ..\Sources\BIN\Release\ProjectConfiguration.dll
    File ..\Sources\BIN\Release\PSDI_CAN.dll
    File ..\Sources\BIN\Release\Replay.dll
    File ..\Sources\BIN\Release\SignalWatch.dll
    File ..\Sources\BIN\Release\TestSetupEditorGUI.dll
    File ..\Sources\BIN\Release\TestSuiteExecutorGUI.dll
    File ..\Sources\BIN\Release\TXWindow.dll
    File ..\Sources\BIN\Release\FormatConverter.exe
    File ..\Sources\BIN\Release\SigGrphWnd.dll
    File ..\Sources\BIN\Release\SignalDefiner.dll
    File ..\Sources\BIN\ReleaseUMinSize\DMGraph.dll

	; Converters
    File /r ..\Sources\BIN\Release\ConverterPlugins
	
    ; Help
    File /oname=BUSMASTER.chm "..\Documents\4 Help\out\help.chm"

    ; MinGW
    File /r ..\Sources\BIN\Release\MinGW

    ; Drivers
    File ..\Sources\BIN\Release\CanApi2.dll	; PEAK USB
    File ..\Sources\BIN\Release\canlib32.dll	; Kvaser CAN
    File ..\Sources\BIN\Release\icsneo40.dll	; ICS neoVI
    File ..\Sources\BIN\Release\vxlapi.dll	; Vector XL

    ; License
    File ..\COPYING.LESSER.txt
    File ..\COPYING.txt

    ; Start menu entries
    CreateDirectory "$SMPROGRAMS\BUSMASTER"
    CreateShortCut "$SMPROGRAMS\BUSMASTER\BUSMASTER.lnk" "$INSTDIR\BUSMASTER.exe" "" "$INSTDIR\BUSMASTER.exe" 0
    CreateShortCut "$SMPROGRAMS\BUSMASTER\Uninstall.lnk" "$INSTDIR\uninst.exe" "" "$INSTDIR\uninst.exe" 0

    ; Registry entries
    WriteRegStr HKLM "Software\BUSMASTER" "Install_Dir" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER" "DisplayName" "BUSMASTER (remove only)"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER" "UninstallString" '"$INSTDIR\uninst.exe"'

    ; Compatibiliy settings
    DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe"
    DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe"

    ; Server registration
    SetOutPath $INSTDIR
    ExecWait 'BusEmulation.exe /regserver'
    ExecWait 'BUSMASTER.exe /regserver'
    ExecWait 'regsvr32 DMGraph.dll /s'

    ; Uninstaller
    WriteUninstaller "uninst.exe"
SectionEnd

; Uninstall section here...
UninstallText "This will uninstall BUSMASTER Installer. Press NEXT to continue."
Section "Uninstall"
    ; Unregister server
    SetOutPath $INSTDIR
    ExecWait 'BusEmulation.exe /unregserver'
    ExecWait 'BUSMASTER.exe /unregserver'
	ExecWait 'regsvr32 /u DMGraph.dll /s'

    ; Delete registration entries
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER"

    ; Delete installation folder
    RMDir /r "$INSTDIR"

    ; Delete start menu entries
    Delete "$SMPROGRAMS\BUSMASTER\Uninstall.lnk"
    Delete "$SMPROGRAMS\BUSMASTER\BUSMASTER.lnk"
SectionEnd

