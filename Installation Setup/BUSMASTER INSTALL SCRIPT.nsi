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
Name "BUSMASTER"

; Do a CRC check when initializing setup
CRCCheck On

; Output filename
Outfile "BUSMASTER_Installer_Ver_1.6.6.exe"

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

; Pages
Page license
Page components
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

; Installation Types
InstType "Typical"
InstType "Full"
InstType "Minimal"

; License Text
LicenseData ../COPYING.LESSER.txt

SectionGroup "Main"
Section "BUSMASTER"
    SectionIn RO 1 2 3
    SetOutPath $INSTDIR
	
	; If the file exists delete it before installing
	; PTV

	; Deleting If BusEmulation.exe exists
	IfFileExists $INSTDIR\BusEmulation.exe bBusEmExists
	bBusEmExists:
			Delete "$INSTDIR\BusEmulation.exe"
			
	; Deleting If BUSMASTER.exe exists		
	IfFileExists $INSTDIR\BUSMASTER.exe bBsExists
	bBsExists:
			Delete "$INSTDIR\BUSMASTER.exe"
	
	; Deleting If BUSMASTER.tlb exists		
	IfFileExists $INSTDIR\BUSMASTER.tlb bBstlbExists
	bBstlbExists:
			Delete "$INSTDIR\BUSMASTER.tlb"
			
	; Deleting If BUSMASTER_Interface.c exists
	IfFileExists $INSTDIR\BUSMASTER_Interface.c bBsIntfcbExists
	bBsIntfcbExists:
			Delete "$INSTDIR\BUSMASTER_Interface.c"
			
	; Deleting If BUSMASTER_Interface.h exists
	IfFileExists $INSTDIR\BUSMASTER_Interface.h bBsIntfhbExists
	bBsIntfhbExists:
			Delete "$INSTDIR\BUSMASTER_Interface.h"
			
	; Deleting If CAN_ETAS_BOA.dll exists
	IfFileExists $INSTDIR\CAN_ETAS_BOA.dll bCanEtasbExists
	bCanEtasbExists:
			Delete "$INSTDIR\CAN_ETAS_BOA.dll"
			
	; Deleting If CAN_ICS_neoVI.dll exists
	IfFileExists $INSTDIR\CAN_ICS_neoVI.dll bCanneoVIbExists
	bCanneoVIbExists:
			Delete "$INSTDIR\CAN_ICS_neoVI.dll"
	
	; Deleting If CAN_Kvaser_CAN.dll exists
	IfFileExists $INSTDIR\CAN_Kvaser_CAN.dll bCanKvaserIbExists
	bCanKvaserIbExists:
			Delete "$INSTDIR\CAN_Kvaser_CAN.dll"
			
	; Deleting If CAN_MHS.dll exists
	IfFileExists $INSTDIR\CAN_MHS.dll bCanMHSbExists
	bCanMHSbExists:
			Delete "$INSTDIR\CAN_MHS.dll"
			
	; Deleting If CAN_PEAK_USB.dll exists
	IfFileExists $INSTDIR\CAN_PEAK_USB.dll bCanPeakbExists
	bCanPeakbExists:
			Delete "$INSTDIR\CAN_PEAK_USB.dll"
			
	; Deleting If CAN_STUB.dll exists
	IfFileExists $INSTDIR\CAN_STUB.dll bCanSTBbExists
	bCanSTBbExists:
			Delete "$INSTDIR\CAN_STUB.dll"
			
	; Deleting If CAN_Vector_XL.dll exists
	IfFileExists $INSTDIR\CAN_Vector_XL.dll bCanVctrbExists
	bCanVctrbExists:
			Delete "$INSTDIR\CAN_Vector_XL.dll"
			
	; Deleting If CAN_IXXAT_VCI.dll exists
	IfFileExists $INSTDIR\CAN_IXXAT_VCI.dll bCanIXXATbExists
	bCanIXXATbExists:
			Delete "$INSTDIR\CAN_IXXAT_VCI.dll"			
			
	; Deleting If Changelog.txt exists
	IfFileExists $INSTDIR\Changelog.txt bChngLogbExists
	bChngLogbExists:
			Delete "$INSTDIR\Changelog.txt"
	
	; Deleting If DIL_Interface.dll exists
	IfFileExists $INSTDIR\DIL_Interface.dll bDilIntrfcbExists
	bDilIntrfcbExists:
			Delete "$INSTDIR\DIL_Interface.dll"	
			
	; Deleting If DIL_J1939.dll exists
	IfFileExists $INSTDIR\DIL_J1939.dll bDILJbExists
	bDILJbExists:
			Delete "$INSTDIR\DIL_J1939.dll"
			
	; Deleting If Filter.dll exists
	IfFileExists $INSTDIR\Filter.dll bFilterbExists
	bFilterbExists:
			Delete "$INSTDIR\Filter.dll"
			
	; Deleting If FrameProcessor.dll exists
	IfFileExists $INSTDIR\FrameProcessor.dll bFPbExists
	bFPbExists:
			Delete "$INSTDIR\FrameProcessor.dll"
			
	; Deleting If GCCDLLMakeTemplate_CAN exists
	IfFileExists $INSTDIR\GCCDLLMakeTemplate_CAN bGCCCbExists
	bGCCCbExists:
			Delete "$INSTDIR\GCCDLLMakeTemplate_CAN"
			
	; Deleting If GCCDLLMakeTemplate_J1939 exists
	IfFileExists $INSTDIR\GCCDLLMakeTemplate_J1939 bGCCJbExists
	bGCCJbExists:
			Delete "$INSTDIR\GCCDLLMakeTemplate_J1939"
			
	; Deleting If mhsbmcfg.dll exists
	IfFileExists $INSTDIR\mhsbmcfg.dll bmhscfgbExists
	bmhscfgbExists:
			Delete "$INSTDIR\mhsbmcfg.dll"
			
	; Deleting If NodeSimEx.dll exists
	IfFileExists $INSTDIR\NodeSimEx.dll bNdeSimbExists
	bNdeSimbExists:
			Delete "$INSTDIR\NodeSimEx.dll"
			
	; Deleting If ProjectConfiguration.dll exists
	IfFileExists $INSTDIR\ProjectConfiguration.dll bPrjConfigbExists
	bPrjConfigbExists:
			Delete "$INSTDIR\ProjectConfiguration.dll"
			
	; Deleting If PSDI_CAN.dll exists
	IfFileExists $INSTDIR\PSDI_CAN.dll bPSDICanbExists
	bPSDICanbExists:
			Delete "$INSTDIR\PSDI_CAN.dll"
	
	; Deleting If Replay.dll exists
	IfFileExists $INSTDIR\Replay.dll bReplaybExists
	bReplaybExists:
			Delete "$INSTDIR\Replay.dll"
			
	; Deleting If SignalWatch.dll exists
	IfFileExists $INSTDIR\SignalWatch.dll bSWbExists
	bSWbExists:
			Delete "$INSTDIR\SignalWatch.dll"
			
	; Deleting If TestSetupEditorGUI.dll exists
	IfFileExists $INSTDIR\TestSetupEditorGUI.dll bTSEGbExists
	bTSEGbExists:
			Delete "$INSTDIR\TestSetupEditorGUI.dll"
			
	; Deleting If TestSuiteExecutorGUI.dll exists
	IfFileExists $INSTDIR\TestSuiteExecutorGUI.dll bTSExGbExists
	bTSExGbExists:
			Delete "$INSTDIR\TestSuiteExecutorGUI.dll"
			
	; Deleting If TXWindow.dll exists
	IfFileExists $INSTDIR\TXWindow.dll bTSWbExists
	bTSWbExists:
			Delete "$INSTDIR\TXWindow.dll"
			
	; Deleting If FormatConverter.exe exists
	IfFileExists $INSTDIR\FormatConverter.exe bFCbExists
	bFCbExists:
			Delete "$INSTDIR\FormatConverter.exe"
			
	; Deleting If SigGrphWnd.dll exists
	IfFileExists $INSTDIR\SigGrphWnd.dll bSGWbExists
	bSGWbExists:
			Delete "$INSTDIR\SigGrphWnd.dll"
			
	; Deleting If SignalDefiner.dll exists
	IfFileExists $INSTDIR\SignalDefiner.dll bSDbExists
	bSDbExists:
			Delete "$INSTDIR\SignalDefiner.dll"

	; Deleting files within ConverterPlugins
	; Deleting If AscLogConverter.dll exists
	IfFileExists $INSTDIR\ConverterPlugins\AscLogConverter.dll bCVALbExists
	bCVALbExists:
			Delete "$INSTDIR\ConverterPlugins\AscLogConverter.dll"

	; Deleting If CAPL2CConverter.dll exists
	IfFileExists $INSTDIR\ConverterPlugins\CAPL2CConverter.dll.dll bCVCCbExists
	bCVCCbExists:
			Delete "$INSTDIR\ConverterPlugins\CAPL2CConverter.dll"
			
	; Deleting If CAPL2Converterer.dll exists
	IfFileExists $INSTDIR\ConverterPlugins\CAPL2Converterer.dll bCVCCbrExists
	bCVCCbrExists:
			Delete "$INSTDIR\ConverterPlugins\CAPL2Converterer.dll"
			
	; Deleting If DBC2DBFConverter.dll exists
	IfFileExists $INSTDIR\ConverterPlugins\DBC2DBFConverter.dll bCVDDbExists
	bCVDDbExists:
			Delete "$INSTDIR\ConverterPlugins\DBC2DBFConverter.dll"
			
	; Deleting If DBF2DBCConverter.dll exists
	IfFileExists $INSTDIR\ConverterPlugins\DBF2DBCConverter.dll bCVDfDbExists
	bCVDfDbExists:
			Delete "$INSTDIR\ConverterPlugins\DBF2DBCConverter.dll"
	
	; Deleting If DBC2DBFConverterd.dll exists
	IfFileExists $INSTDIR\ConverterPlugins\DBC2DBFConverterd.dll bCVDCDbExists
	bCVDCDbExists:
			Delete "$INSTDIR\ConverterPlugins\DBC2DBFConverterd.dll"
						
	; Deleting If LogAscConverter.dll exists
	IfFileExists $INSTDIR\ConverterPlugins\LogAscConverter.dll bCVLAbExists
	bCVLAbExists:
			Delete "$INSTDIR\ConverterPlugins\LogAscConverter.dll"
			
	; Deleting If LogToExcelConverter.dll exists
	IfFileExists $INSTDIR\ConverterPlugins\LogToExcelConverter.dll bCVALEExists
	bCVALEExists:
			Delete "$INSTDIR\ConverterPlugins\LogToExcelConverter.dll"
			
	; Deleting If BUSMASTER.chm exists
	IfFileExists $INSTDIR\BUSMASTER.chm bchmbExists
	bchmbExists:
			Delete "$INSTDIR\BUSMASTER.chm"
			
	; Deleting If SimulatedSystems exists
	IfFileExists $INSTDIR\SimulatedSystems bSSExists
	bSSExists:
			RMDir /r "$INSTDIR\SimulatedSystems"
			
	; Deleting If COPYING.LESSER.txt exists
	IfFileExists $INSTDIR\COPYING.LESSER.txt bCLExists
	bCLExists:
			Delete "$INSTDIR\COPYING.LESSER.txt"
			
	; Deleting If COPYING.txt exists
	IfFileExists $INSTDIR\COPYING.txt bCpyingExists
	bCpyingExists:
			Delete "$INSTDIR\COPYING.txt"
			
	; Deleting If Readme.txt exists
	IfFileExists $INSTDIR\Readme.txt bRdMExists
	bRdMExists:
			Delete "$INSTDIR\Readme.txt"
			
	; Deleting If DMGraph.dll exists
	IfFileExists $INSTDIR\DMGraph.dll bDMGExists
	bDMGExists:
			Delete "$INSTDIR\DMGraph.dll"
			
	; Deleting If ETASneo40.dll exists
	IfFileExists $INSTDIR\ETASneo40.dll bETExists
	bETExists:
			Delete "$INSTDIR\ETASneo40.dll"
			
	; Deleting If icsneo40.dll exists
	IfFileExists $INSTDIR\icsneo40.dll bicExists
	bicExists:
			Delete "$INSTDIR\icsneo40.dll"
			
	; Deleting If canlib32.dll exists
	IfFileExists $INSTDIR\canlib32.dll biclibExists
	biclibExists:
			Delete "$INSTDIR\canlib32.dll"
			
	; Deleting If mhstcan.dll exists
	IfFileExists $INSTDIR\mhstcan.dll bmhsExists
	bmhsExists:
			Delete "$INSTDIR\mhstcan.dll"
			
	; Deleting If CanApi2.dll exists
	IfFileExists $INSTDIR\CanApi2.dll bCApExists
	bCApExists:
			Delete "$INSTDIR\CanApi2.dll"
			
	; Deleting If vxlapi.dll exists
	IfFileExists $INSTDIR\vxlapi.dll bvxlExists
	bvxlExists:
			Delete "$INSTDIR\vxlapi.dll"
			
	; Deleting If iconv.dll exists
	IfFileExists $INSTDIR\iconv.dll biconvExists
	biconvExists:
			Delete "$INSTDIR\iconv.dll"
			
	; Deleting If libxml2.dll exists
	IfFileExists $INSTDIR\libxml2.dll blibxml2Exists
	blibxml2Exists:
			Delete "$INSTDIR\libxml2.dll"
			
	; Deleting If zlib1.dll exists
	IfFileExists $INSTDIR\zlib1.dll bzlibExists
	bzlibExists:
			Delete "$INSTDIR\zlib1.dll"
			
	; Deleting If MinGW exists
	IfFileExists $INSTDIR\MinGW bMinGWExists
	bMinGWExists:
			RMDir /r "$INSTDIR\MinGW"
			
	; Deleting If uninst.exe exists
	IfFileExists $INSTDIR\uninst.exe bUninstallExists
	bUninstallExists:
			Delete "$INSTDIR\uninst.exe"
			
	; Deleting If BUSMASTER_Cleanup_Registry.exe exists
	IfFileExists $INSTDIR\BUSMASTER_Cleanup_Registry.exe bRegCleanupExists
	bRegCleanupExists:
			Delete "$INSTDIR\BUSMASTER_Cleanup_Registry.exe"
	; PTV END
	
	
    ; BUSMASTER
    File ..\Sources\BIN\Release\BusEmulation.exe
    File ..\Sources\BIN\Release\BUSMASTER.exe
    File ..\Sources\BIN\Release\BUSMASTER.tlb
    File ..\Sources\Application\BUSMASTER_Interface.c
    File ..\Sources\Application\BUSMASTER_Interface.h
    File ..\Sources\BIN\Release\CAN_ETAS_BOA.dll
    File ..\Sources\BIN\Release\CAN_ICS_neoVI.dll
    File ..\Sources\BIN\Release\CAN_Kvaser_CAN.dll
    File ..\Sources\BIN\Release\CAN_MHS.dll
    File ..\Sources\BIN\Release\CAN_PEAK_USB.dll
    File ..\Sources\BIN\Release\CAN_STUB.dll
    File ..\Sources\BIN\Release\CAN_Vector_XL.dll
	File ..\Sources\BIN\Release\CAN_IXXAT_VCI.dll	
    File ..\Sources\BIN\Release\Changelog.txt
    File ..\Sources\BIN\Release\DIL_Interface.dll
    File ..\Sources\BIN\Release\DIL_J1939.dll
    File ..\Sources\BIN\Release\Filter.dll
    File ..\Sources\BIN\Release\FrameProcessor.dll
    File ..\Sources\BIN\Release\GCCDLLMakeTemplate_CAN
    File ..\Sources\BIN\Release\GCCDLLMakeTemplate_J1939
    File ..\Sources\BIN\Release\mhsbmcfg.dll
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
	File ..\Sources\BIN\Release\iconv.dll
	File ..\Sources\BIN\Release\libxml2.dll
	File ..\Sources\BIN\Release\zlib1.dll

    ; Converters
    File /r ..\Sources\BIN\Release\ConverterPlugins
	
    ; Help
    File /oname=BUSMASTER.chm "..\Documents\4 Help\out\help.chm"

	SetOutPath "$INSTDIR\SimulatedSystems\include\"
	; Simulated Systems Include files
	File ..\Sources\BIN\Release\SimulatedSystems\include\CANIncludes.h 
	File ..\Sources\BIN\Release\SimulatedSystems\include\CAPLWrapper.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\Common.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\Wrapper_CAN.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\Wrapper_J1939.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\J1939Includes.h
	
	SetOutPath "$INSTDIR\SimulatedSystems\OBJ\"
	; Simulated Systems Library files
	File ..\Sources\BIN\Release\SimulatedSystems\OBJ\libWrapper_CAN.a
	File ..\Sources\BIN\Release\SimulatedSystems\OBJ\libWrapper_J1939.a

	SetOutPath $INSTDIR\
	
    ; License
    File ..\COPYING.LESSER.txt
    File ..\COPYING.txt
    
    ; Readme
    File ..\Readme.txt
	
	; create desktop shortcut
	CreateShortCut "$DESKTOP\BUSMASTER.lnk" "$INSTDIR\BUSMASTER.exe" ""
	

    ; Registry entries
    WriteRegStr HKLM "Software\BUSMASTER" "Install_Dir" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER" "DisplayName" "BUSMASTER (remove only)"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER" "UninstallString" '"$INSTDIR\uninst.exe"'


    ; Compatibility settings for Windows 7
    ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
    StrCmp $1 "6.1" 0 lbl ;StrCmp str1 str2 jump_if_equal [jump_if_not_equal]
    WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe" "WIN98"
    WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe" "WIN98"
    WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\FormatConverter.exe" "WIN98"
    lbl:	
	
    ; Server registration
    ExecWait 'BusEmulation.exe /regserver'
    ExecWait 'BUSMASTER.exe /regserver'

	SetShellVarContext all
	
	; Start menu entries
    CreateDirectory "$SMPROGRAMS\BUSMASTER"
    CreateShortCut "$SMPROGRAMS\BUSMASTER\BUSMASTER.lnk" "$INSTDIR\BUSMASTER.exe" "" "$INSTDIR\BUSMASTER.exe" 0
	CreateShortCut "$SMPROGRAMS\BUSMASTER\BUSMASTER_Cleanup_Registry.lnk" "$INSTDIR\BUSMASTER_Cleanup_Registry.exe" "" "$INSTDIR\BUSMASTER_Cleanup_Registry.exe" 0
    CreateShortCut "$SMPROGRAMS\BUSMASTER\Uninstall.lnk" "$INSTDIR\uninst.exe" "" "$INSTDIR\uninst.exe" 0
	
    ; Uninstaller
    WriteUninstaller "uninst.exe"
SectionEnd
Section "DMGraph"
    SectionIn RO 1 2 3
    SetOutPath $INSTDIR
    File ..\Sources\BIN\ReleaseUMinSize\DMGraph.dll
    ExecWait 'regsvr32 DMGraph.dll /s'
SectionEnd
SectionGroupEnd
SectionGroup "Hardware Libraries"
Section "ETAS ES581"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\ETASneo40.dll
SectionEnd
Section "Intrepid neoVI"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\icsneo40.dll
SectionEnd
Section "Kvaser CAN"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\canlib32.dll
SectionEnd
Section "MHS-Elektronik Tiny-CAN"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\mhstcan.dll
SectionEnd
Section "Peak USB"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\CanApi2.dll
SectionEnd
Section "Vector XL"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\vxlapi.dll
SectionEnd
SectionGroupEnd
SectionGroup "Node Simulation"
Section "MinGW"
    SectionIn 2
    SetOutPath $INSTDIR
    ; MinGW
    File /r ..\EXTERNAL_SOURCE\MinGW
SectionEnd
SectionGroupEnd
SectionGroup "Registry Cleanup"
Section "BUSMASTER Reg Cleanup"
    SectionIn RO 1 2 3
    SetOutPath $INSTDIR
    File BUSMASTER_Cleanup_Registry.exe
SectionEnd
SectionGroupEnd

; Uninstall section here...
Section "Uninstall"
    ; Unregister server
    SetOutPath $INSTDIR
    ExecWait 'BusEmulation.exe /unregserver'
    ExecWait 'BUSMASTER.exe /unregserver'
    ExecWait 'regsvr32 /u DMGraph.dll /s'	
	
    ; Delete registration entries
	; PTV
	DeleteRegKey HKCU "Software\RBIN\BUSMASTER"
	
	DeleteRegValue HKLM "Software\BUSMASTER" "Install_Dir"
    ; PTV END
	
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER"

    ; Compatibility settings
    DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe"
    DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe"
    DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\FormatConverter.exe"
    ; Delete installation folder
	; PTV
	 SetOutPath "$TEMP"
     RMDir /r "$INSTDIR"	
	 
	 ; Delete desktop shortcut
	Delete "$DESKTOP\BUSMASTER.lnk"
	; Delete shortcut from start menu
	Delete "$SMPROGRAMS\BUSMASTER.lnk"
	
	 SetShellVarContext all
	
	; Delete start menu entries
    Delete "$SMPROGRAMS\BUSMASTER\Uninstall.lnk"
    Delete "$SMPROGRAMS\BUSMASTER\BUSMASTER.lnk"
	Delete "$SMPROGRAMS\BUSMASTER\BUSMASTER_Cleanup_Registry.lnk"
	
	; Deleting If StartPrograms BUSMASTER dir exists
	IfFileExists $SMPROGRAMS\BUSMASTER bSSBMDirExists
	bSSBMDirExists:
			RMDir /r "$SMPROGRAMS\BUSMASTER"
  
SectionEnd
