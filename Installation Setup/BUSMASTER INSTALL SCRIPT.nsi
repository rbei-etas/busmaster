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

!include "NSISHeaders.nsh"
 
!macro StrStr ResultVar String SubString
  Push `${String}`
  Push `${SubString}`
  Call StrStr
  Pop `${ResultVar}`
!macroend

 
Function StrStr
/*After this point:
  ------------------------------------------
  $R0 = SubString (input)
  $R1 = String (input)
  $R2 = SubStringLen (temp)
  $R3 = StrLen (temp)
  $R4 = StartCharPos (temp)
  $R5 = TempStr (temp)*/
 
  ;Get input from user
  Exch $R0
  Exch
  Exch $R1
  Push $R2
  Push $R3
  Push $R4
  Push $R5
 
  ;Get "String" and "SubString" length
  StrLen $R2 $R0
  StrLen $R3 $R1
  ;Start "StartCharPos" counter
  StrCpy $R4 0
 
  ;Loop until "SubString" is found or "String" reaches its end
  loop:
    ;Remove everything before and after the searched part ("TempStr")
    StrCpy $R5 $R1 $R2 $R4
 
    ;Compare "TempStr" with "SubString"
    StrCmp $R5 $R0 done
    ;If not "SubString", this could be "String"'s end
    IntCmp $R4 $R3 done 0 done
    ;If not, continue the loop
    IntOp $R4 $R4 + 1
    Goto loop
  done:
 
/*After this point:
  ------------------------------------------
  $R0 = ResultVar (output)*/
 
  ;Remove part before "SubString" on "String" (if there has one)
  StrCpy $R0 $R1 `` $R4
 
  ;Return output to user
  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Pop $R1
  Exch $R0
FunctionEnd

; StrContains
; This function does a case sensitive searches for an occurrence of a substring in a string. 
; It returns the substring if it is found. 
; Otherwise it returns null(""). 
; Written by kenglish_hi
; Adapted from StrReplace written by dandaman32
 
 
Var STR_HAYSTACK
Var STR_NEEDLE
Var STR_CONTAINS_VAR_1
Var STR_CONTAINS_VAR_2
Var STR_CONTAINS_VAR_3
Var STR_CONTAINS_VAR_4
Var STR_RETURN_VAR
 
Function StrContains
  Exch $STR_NEEDLE
  Exch 1
  Exch $STR_HAYSTACK
  ; Uncomment to debug
  ;MessageBox MB_OK 'STR_NEEDLE = $STR_NEEDLE STR_HAYSTACK = $STR_HAYSTACK '
    StrCpy $STR_RETURN_VAR ""
    StrCpy $STR_CONTAINS_VAR_1 -1
    StrLen $STR_CONTAINS_VAR_2 $STR_NEEDLE
    StrLen $STR_CONTAINS_VAR_4 $STR_HAYSTACK
    loop:
      IntOp $STR_CONTAINS_VAR_1 $STR_CONTAINS_VAR_1 + 1
      StrCpy $STR_CONTAINS_VAR_3 $STR_HAYSTACK $STR_CONTAINS_VAR_2 $STR_CONTAINS_VAR_1
      StrCmp $STR_CONTAINS_VAR_3 $STR_NEEDLE found
      StrCmp $STR_CONTAINS_VAR_1 $STR_CONTAINS_VAR_4 done
      Goto loop
    found:
      StrCpy $STR_RETURN_VAR $STR_NEEDLE
      Goto done
    done:
   Pop $STR_NEEDLE ;Prevent "invalid opcode" errors and keep the
   Exch $STR_RETURN_VAR  
FunctionEnd
 
!macro _StrContainsConstructor OUT NEEDLE HAYSTACK
  Push "${HAYSTACK}"
  Push "${NEEDLE}"
  Call StrContains
  Pop "${OUT}"
!macroend
 
!define StrContains '!insertmacro "_StrContainsConstructor"'

; Show gradient background
BGGradient 8080C0 0000FF FFFFFF

; Title of this installation
Name "BUSMASTER"

; Do a CRC check when initializing setup
CRCCheck On

; Output filename
Outfile "BUSMASTER_Installer_Ver_${VERSION}.exe"

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
InstallDir "$PROGRAMFILES\BUSMASTER_v${VERSION}"
; Uninstall info registry location
;InstallDirRegKey HKLM "SOFTWARE\BUSMASTER" "Install_Dir"

; Folder selection prompt
DirText "Please select an installation folder."

Var LABEL
Function onClickMinGWLink
  ExecShell "open" "http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/"
FunctionEnd

Function onClickBMHelpLink
 StrCpy $0 $WINDIR\hh.exe
 Exec '"$0" mk:@MSITStore:$INSTDIR\BUSMASTER.chm::/topics/MinGW%20Installation.html'
FunctionEnd

Function information
nsDialogs::Create 1018

	; Check for .NET installation	
	 ${If} ${HasDotNet3.5}
		Goto NETFrameworkInstalled
	 ${EndIf}
	 
	 ${If} ${HasDotNet4.0}	 
		Goto NETFrameworkInstalled
	 ${EndIf}	
	  	
	; Execution will come here if .NET framework is not installed. Inform User of the necessary actions.
	${NSD_CreateLabel} 0 20 100% 40  "Microsoft .NET Framework is not installed! Please install from below link:"
	${NSD_CreateLink} 0 75 100% 12   "http://www.microsoft.com/en-in/download/details.aspx?id=17851"
	Goto FinishedShow
	
	NETFrameworkInstalled:
	; Check for MINGW installation
	
	ReadEnvStr $R0 "PATH"	  
	  
	${StrStr} $0 $R0 "MinGW"	
	  
	${StrContains} $0 "MinGW" $R0
	  
	StrCmp $0 "" notfound
	${NSD_CreateLabel} 0 20 100% 40 "The necessary .Net framework and MinGW packages are available in your PC."		
	Goto FinishedShow
	notfound:	  		
	${NSD_CreateLabel} 0 20 100% 40 "-> The following steps should be followed to install MinGW folder for Node simulation. $\n Download latest mingw executable from the following link:"	
	Pop $Label
	${NSD_CreateLink} 0 75 100% 12 "http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/"
	Pop $Label
	${NSD_OnClick} $Label onClickMinGWLink 
	${NSD_CreateLabel}  0 100 100% 20 "use it to download and install required GCC (C,C++) compilers."		
	${NSD_CreateLink}   0 120 100% 30 "Refer BUSMASTER Help file MinGW section for detailed information."
	Pop $Label
	${NSD_OnClick} $Label onClickBMHelpLink
	FinishedShow:
	nsDialogs::Show	
FunctionEnd

Function UpdateIcsneo40
    ; install icsneo40.dll unless the file is already installed and is newer
    IfFileExists "$SYSDIR\icsneo40.dll" icsneo40_upgrade icsneo40_install

    ; compare versions
    icsneo40_upgrade:
    GetDLLVersion "$SYSDIR\icsneo40.dll" $R0 $R1
    GetDLLVersionLocal "..\Sources\BIN\Release\icsneo40.dll" $R2 $R3
    IntCmpU $R0 $R2 +1 icsneo40_install icsneo40_done ; compare major version
    IntCmpU $R1 $R3 +1 icsneo40_install icsneo40_done ; compare minor version
    Goto icsneo40_done

    ; install file
    icsneo40_install:
    SetOutPath $SYSDIR
    File ..\Sources\BIN\Release\icsneo40.dll

    icsneo40_done:
FunctionEnd

;Checks for the version if it is already installed
Function CheckVersion
	;Read the BUSMASTER version installed
	ReadRegStr $1 HKLM "SOFTWARE\Wow6432Node" BUSMASTER_v${VERSION}"
	StrCmp $1 "v${VERSION}" 0 Confirm ;StrCmp str1 str2 jump_if_equal [jump_if_not_equal]	
Confirm:
	;If the version is already installed, get the the installtion path
	ReadRegStr $1 HKLM "SOFTWARE\Wow6432Node\BUSMASTER_v${VERSION}" Install_Dir		
	StrCmp $1 $INSTDIR 0 Exit
	MessageBox MB_YESNO|MB_ICONEXCLAMATION  'BUSMASTER_v${VERSION} is already installed. Do you wish to overwrite ?' IDYES Install
	Quit
Exit:
	StrCmp $1 "" Install Stop
Stop:	
	MessageBox MB_OK|MB_ICONSTOP 'BUSMASTER_v${VERSION} is already installed in a different path. Please uninstall the previous version. Installation will be aborted.'
	Quit
Install:	
FunctionEnd

; Pages
Page license
Page components
Page directory
Page instfiles
Page Custom information
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
	Call CheckVersion
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
			
	; Deleting If BUSMASTER.exe.manifest exists		
	IfFileExists $INSTDIR\BUSMASTER.exe.manifest bBsManifestExists
	bBsManifestExists:
			Delete "$INSTDIR\BUSMASTER.exe.manifest"			
	
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

	; Deleting If CAN_ETAS_BOA_1_4.dll exists
	IfFileExists $INSTDIR\CAN_ETAS_BOA_1_4.dll bCanEtas14bExists
	bCanEtas14bExists:
			Delete "$INSTDIR\CAN_ETAS_BOA_1_4.dll"

	; Deleting If CAN_ETAS_BOA_1_5.dll exists
	IfFileExists $INSTDIR\CAN_ETAS_BOA_1_5.dll bCanEtas15bExists
	bCanEtas15bExists:
			Delete "$INSTDIR\CAN_ETAS_BOA_1_5.dll"
			
	; Deleting If FLEXRAY_ETAS_BOA.dll exists
	IfFileExists $INSTDIR\FLEXRAY_ETAS_BOA.dll bFlexEtasbExists
	bFlexEtasbExists:
			Delete "$INSTDIR\FLEXRAY_ETAS_BOA.dll"

			; Deleting If TXWindowFlexRay.dll exists
	IfFileExists $INSTDIR\TXWindowFlexRay.dll bTxWndFlexEtasbExists
	bTxWndFlexEtasbExists:
			Delete "$INSTDIR\TXWindowFlexRay.dll"
	
	; Deleting If LIN_Vector_XL.dll exists
	IfFileExists $INSTDIR\LIN_Vector_XL.dll bTxWndLINEtasbExists
	bTxWndLINEtasbExists:
			Delete "$INSTDIR\LIN_Vector_XL.dll"	
				
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
	; Deleting If CAN_VSCOM.dll exists
	IfFileExists $INSTDIR\CAN_VSCOM.dll bCanVSCOMbExists
	bCanVSCOMbExists:
			Delete "$INSTDIR\CAN_VSCOM.dll"	

	; Deleting If CAN_ISOLAR_EVE_VCAN.dll exists
	IfFileExists $INSTDIR\CAN_ISOLAR_EVE_VCAN.dll bCanETASIsolarExists
	bCanETASIsolarExists:
			Delete "$INSTDIR\CAN_ISOLAR_EVE_VCAN.dll"	
			
	; Deleting If CAN_i-VIEW.dll exists
	IfFileExists $INSTDIR\CAN_i-VIEW.dll biViewCANExists
	biViewCANExists:
			Delete "$INSTDIR\CAN_i-VIEW.dll"

    ; Deleting If CAN_NSI.dll exists
	IfFileExists $INSTDIR\CAN_NSI.dll biNSICANExists
	biNSICANExists:
			Delete "$INSTDIR\CAN_NSI.dll"			
			
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
			
	; Deleting If GCCDLLMakeTemplate_LIN exists
	IfFileExists $INSTDIR\GCCDLLMakeTemplate_LIN bGCCLbExists
	bGCCLbExists:
			Delete "$INSTDIR\GCCDLLMakeTemplate_LIN"
			
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
			
	; Deleting If intl.dll exists
	IfFileExists $INSTDIR\intl.dll bintlExists
	bintlExists:
			Delete "$INSTDIR\intl.dll"
	
	; Deleting files within JPN
	; Deleting If TXWindowJPN.dll exists
	IfFileExists $INSTDIR\JPN\TXWindowJPN.dll bJPNTxWinbExists
	bJPNTxWinbExists:
			Delete "$INSTDIR\JPN\TXWindowJPN.dll"
	
	; Deleting If TestSuiteExecutorGUIJPN.dll exists
	IfFileExists $INSTDIR\JPN\TestSuiteExecutorGUIJPN.dll bJPNTestStbExists
	bJPNTestStbExists:
			Delete "$INSTDIR\JPN\TestSuiteExecutorGUIJPN.dll"
	
	; Deleting If TestSetupEditorGUIJPN.dll exists
	IfFileExists $INSTDIR\JPN\TestSetupEditorGUIJPN.dll bJPNTestStupbExists
	bJPNTestStupbExists:
			Delete "$INSTDIR\JPN\TestSetupEditorGUIJPN.dll"
			
	; Deleting If SignalWatchJPN.dll exists
	IfFileExists $INSTDIR\JPN\SignalWatchJPN.dll bJPNSigWatchbExists
	bJPNSigWatchbExists:
			Delete "$INSTDIR\JPN\SignalWatchJPN.dll"
			
	; Deleting If SignalDefinerJPN.dll exists
	IfFileExists $INSTDIR\JPN\SignalDefinerJPN.dll bJPNSigDefbExists
	bJPNSigDefbExists:
			Delete "$INSTDIR\JPN\SignalDefinerJPN.dll"
			
	; Deleting If SigGrphWndJPN.dll exists
	IfFileExists $INSTDIR\JPN\SigGrphWndJPN.dll bJPNSigGrpbExists
	bJPNSigGrpbExists:
			Delete "$INSTDIR\JPN\SigGrphWndJPN.dll"
			
	; Deleting If NodeSimExJPN.dll exists
	IfFileExists $INSTDIR\JPN\NodeSimExJPN.dll bJPNNdSimbExists
	bJPNNdSimbExists:
			Delete "$INSTDIR\JPN\NodeSimExJPN.dll"
			
	; Deleting If mhsbmcfgJPN.dll exists
	IfFileExists $INSTDIR\JPN\mhsbmcfgJPN.dll bJPNmhsbExists
	bJPNmhsbExists:
			Delete "$INSTDIR\JPN\mhsbmcfgJPN.dll"
			
	; Deleting If FilterJPN.dll exists
	IfFileExists $INSTDIR\JPN\FilterJPN.dll bJPNFilterbExists
	bJPNFilterbExists:
			Delete "$INSTDIR\JPN\FilterJPN.dll"
			
	; Deleting If DMGraphJPN.dll exists
	IfFileExists $INSTDIR\JPN\DMGraphJPN.dll bJPNDMGraphbExists
	bJPNDMGraphbExists:
			Delete "$INSTDIR\JPN\DMGraphJPN.dll"
			
	; Deleting If CAN_Vector_XLJPN.dll exists
	IfFileExists $INSTDIR\JPN\CAN_Vector_XLJPN.dll bJPNCANVectorXLbExists
	bJPNCANVectorXLbExists:
			Delete "$INSTDIR\JPN\CAN_Vector_XLJPN.dll"
			
	; Deleting If CAN_MHSJPN.dll exists
	IfFileExists $INSTDIR\JPN\CAN_MHSJPN.dll bJPNCANMHSJPNbExists
	bJPNCANMHSJPNbExists:
			Delete "$INSTDIR\JPN\CAN_MHSJPN.dll"
			
	; Deleting If CAN_Kvaser_CANJPN.dll exists
	IfFileExists $INSTDIR\JPN\CAN_Kvaser_CANJPN.dll bJPNKvsrbExists
	bJPNKvsrbExists:
			Delete "$INSTDIR\JPN\CAN_Kvaser_CANJPN.dll"
			
	; Deleting If SignalDefinerJPN.dll exists
	IfFileExists $INSTDIR\JPN\ReplayJPN.dll bJPNReplaybExists
	bJPNReplaybExists:
			Delete "$INSTDIR\JPN\ReplayJPN.dll"
			
	; Deleting If CAN_IXXAT_VCIJPN.dll exists
	IfFileExists $INSTDIR\JPN\CAN_IXXAT_VCIJPN.dll bJPNIXXATbExists
	bJPNIXXATbExists:
			Delete "$INSTDIR\JPN\CAN_IXXAT_VCIJPN.dll"
			
	; Deleting If CAN_ICS_neoVIJPN.dll exists
	IfFileExists $INSTDIR\JPN\CAN_ICS_neoVIJPN.dll bJPNNEObExists
	bJPNNEObExists:
			Delete "$INSTDIR\JPN\CAN_ICS_neoVIJPN.dll"
			
	; Deleting If CAN_ETAS_BOAJPN.dll exists
	IfFileExists $INSTDIR\JPN\CAN_ETAS_BOAJPN.dll bJPNBOAbExists
	bJPNBOAbExists:
			Delete "$INSTDIR\JPN\CAN_ETAS_BOAJPN.dll"
			
	; Deleting If BUSMASTERJPN.dll exists
	IfFileExists $INSTDIR\JPN\BUSMASTERJPN.dll bJPNBUSMASTERbExists
	bJPNBUSMASTERbExists:
			Delete "$INSTDIR\JPN\BUSMASTERJPN.dll"
			
	; Deleting If FormatConverterJPN.dll exists
	IfFileExists $INSTDIR\JPN\FormatConverterJPN.dll bJPNFormatConvExists
	bJPNFormatConvExists:
			Delete "$INSTDIR\JPN\FormatConverterJPN.dll"

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
			
	; Deleting If AdvancedUIPlugIn.dll exists
	IfFileExists $INSTDIR\AdvancedUIPlugIn.dll bAdvancedUIDllExists
	bAdvancedUIDllExists:
			Delete "$INSTDIR\AdvancedUIPlugIn.dll"			
			
	IfFileExists $INSTDIR\LINiSolarEveDIL.dll bLINiSolarEveDILExists
	bLINiSolarEveDILExists:
			Delete "$INSTDIR\LIN_ISOLAR_EVE_VLIN.dll"			
	
	IfFileExists $INSTDIR\TXWindowLIN.dll bLinTxDllExists
	bLinTxDllExists:
			Delete "$INSTDIR\TXWindowLIN.dll"		

	; Deleting If Controller_0.dll exists
	IfFileExists $INSTDIR\Controller_0.dll bController0Exists
	bController0Exists:
			Delete "$INSTDIR\Controller_0.dll"	

	; Deleting If Controller_1.dll exists
	IfFileExists $INSTDIR\Controller_1.dll bController1Exists
	bController1Exists:
			Delete "$INSTDIR\Controller_1.dll"	
			
	; Deleting If LIN_Vector_XL.dll exists
	IfFileExists $INSTDIR\LIN_Vector_XL.dll bLinVctrbExists
	bLinVctrbExists:
			Delete "$INSTDIR\LIN_Vector_XL.dll"

			; PTV END
	
	
    ; BUSMASTER
    File ..\Sources\BIN\Release\BusEmulation.exe
    File ..\Sources\BIN\Release\BUSMASTER.exe
	File ..\Sources\BIN\Release\BUSMASTER.exe.manifest	
    File ..\Sources\BIN\Release\BUSMASTER.tlb
    File ..\Sources\Application\BUSMASTER_Interface.c
    File ..\Sources\Application\BUSMASTER_Interface.h
    File ..\Sources\BIN\Release\CAN_STUB.dll	
    File ..\Sources\BIN\Release\Changelog.txt
    File ..\Sources\BIN\Release\DIL_Interface.dll
    File ..\Sources\BIN\Release\DIL_J1939.dll
    File ..\Sources\BIN\Release\Filter.dll
    File ..\Sources\BIN\Release\FrameProcessor.dll
    File ..\Sources\BIN\Release\GCCDLLMakeTemplate_CAN
	File ..\Sources\BIN\Release\GCCDLLMakeTemplate_LIN
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
	File ..\Sources\BIN\Release\intl.dll
	File ..\Sources\BIN\Release\LIN_ISOLAR_EVE_VLIN.dll
	File ..\Sources\BIN\Release\FLEXRAY_ETAS_BOA.dll
	File ..\Sources\BIN\Release\TXWindowFlexRay.dll
	File ..\Sources\BIN\Release\Controller_0.dll
	File ..\Sources\BIN\Release\Controller_1.dll
	File ..\Sources\BIN\Release\LIN_Vector_XL.dll

    ; Converters
    File /r ..\Sources\BIN\Release\ConverterPlugins
	
	; Japanese lib files
	File /r ..\Sources\BIN\Release\JPN
	
	; Japanese Localization folder
	File /r ..\Sources\Localization
	
    ; Help
    File /oname=BUSMASTER.chm "..\Documents\4 Help\out\help.chm"

	SetOutPath "$INSTDIR\SimulatedSystems\include\"
	; Simulated Systems Include files
	File ..\Sources\BIN\Release\SimulatedSystems\include\CANIncludes.h 
	File ..\Sources\BIN\Release\SimulatedSystems\include\LINIncludes.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\CAPLWrapper.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\Common.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\Wrapper_CAN.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\Wrapper_LIN.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\Wrapper_J1939.h
	File ..\Sources\BIN\Release\SimulatedSystems\include\J1939Includes.h
	
	SetOutPath "$INSTDIR\SimulatedSystems\OBJ\"
	; Simulated Systems Library files
	File ..\Sources\BIN\Release\SimulatedSystems\OBJ\libWrapper_CAN.a
	File ..\Sources\BIN\Release\SimulatedSystems\OBJ\libWrapper_LIN.a
	File ..\Sources\BIN\Release\SimulatedSystems\OBJ\libWrapper_J1939.a

	SetOutPath $INSTDIR\
	
    ; License
    File ..\COPYING.LESSER.txt
    File ..\COPYING.txt
    
    ; Readme
    File ..\Readme.txt
	
	
	
	; Check if Visual Studio 2012 redistributable is already installed
	ReadRegStr $1 HKLM "Software\Microsoft\DevDiv\vc\Servicing\11.0\RuntimeMinimum" Install
	StrCmp $1 "1" NoInstall Install
Install:
	; Install Visual Studio 2012 Redistributable
	File "..\Tools\VC++ 2012 Redistributable\vcredist_x86.exe"
	ExecWait '"vcredist_x86.exe" /s /v" /qn"'

NoInstall:	
	IfFileExists $INSTDIR\vcredist_x86.exe bVS2012Exists
	bVS2012Exists:
			Delete "$INSTDIR\vcredist_x86.exe"
	
	; create desktop shortcut
	CreateShortCut "$DESKTOP\BUSMASTER v${VERSION}.lnk" "$INSTDIR\BUSMASTER.exe" ""
	
    ; Registry entries
    WriteRegStr HKLM "Software\BUSMASTER_v${VERSION}" "Install_Dir" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "DisplayName" "BUSMASTER Ver ${VERSION}(remove only)"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "UninstallString" '"$INSTDIR\uninst.exe"'


    ; Compatibility settings for Windows 7
    ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
    StrCmp $1 "6.1" 0 lbl ;StrCmp str1 str2 jump_if_equal [jump_if_not_equal]
    ; WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe" "WINXPSP3"
    WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe" "WINXPSP3"
    WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\FormatConverter.exe" "WINXPSP3"
	WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe" "RUNASADMIN"
	
	; Admin Rights
	; AccessControl::GrantOnFile "$INSTDIR\BUSMASTER.exe" "(BU)" "FullAccess"
	
    lbl:	
	
    ; Server registration
    ExecWait 'BusEmulation.exe /regserver'
    ExecWait 'BUSMASTER.exe /regserver'

	
	
	SetShellVarContext all
		
	; Start menu entries
    CreateDirectory "$SMPROGRAMS\BUSMASTER v${VERSION}"
    CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\BUSMASTER v${VERSION}.lnk" "$INSTDIR\BUSMASTER.exe" "" "$INSTDIR\BUSMASTER.exe" 0
	CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\BUSMASTER_Cleanup_Registry.lnk" "$INSTDIR\BUSMASTER_Cleanup_Registry.exe" "" "$INSTDIR\BUSMASTER_Cleanup_Registry.exe" 0
    CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\Uninstall.lnk" "$INSTDIR\uninst.exe" "" "$INSTDIR\uninst.exe" 0
	
    ; Uninstaller
    WriteUninstaller "uninst.exe"
SectionEnd
Section "DMGraph"
    SectionIn RO 1 2 3
    SetOutPath $INSTDIR
    File ..\Sources\BIN\ReleaseUMinSize\DMGraph.dll    
SectionEnd
SectionGroupEnd
SectionGroup "Hardware Interfaces"
Section "ETAS ES581"
    SectionIn 1 2
    SetOutPath $INSTDIR    
    File ..\Sources\BIN\Release\CAN_ICS_neoVI.dll    
    Call UpdateIcsneo40
SectionEnd
Section "ETAS BOA"
    SectionIn 1 2
    SetOutPath $INSTDIR    
    File ..\Sources\BIN\Release\CAN_ETAS_BOA_1_4.dll
    File ..\Sources\BIN\Release\CAN_ETAS_BOA_1_5.dll
SectionEnd
Section "Intrepid neoVI"
    SectionIn 1 2
    SetOutPath $INSTDIR     
    File ..\Sources\BIN\Release\CAN_ICS_neoVI.dll
    Call UpdateIcsneo40
SectionEnd
Section "Kvaser CAN"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\CAN_Kvaser_CAN.dll
;	File ..\Sources\BIN\Release\canlib32.dll
SectionEnd
Section "MHS-Elektronik Tiny-CAN"
    SectionIn 1 2
    SetOutPath $INSTDIR    
    File ..\Sources\BIN\Release\CAN_MHS.dll
	File ..\Sources\BIN\Release\mhstcan.dll
SectionEnd
Section "Peak USB"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\CAN_PEAK_USB.dll
	File ..\Sources\BIN\Release\CanApi2.dll
SectionEnd
Section "Vector XL"
    SectionIn 1 2
    SetOutPath $INSTDIR    
    File ..\Sources\BIN\Release\CAN_Vector_XL.dll
	File ..\Sources\BIN\Release\vxlapi.dll	
SectionEnd
Section "IXXAT VCI"
    SectionIn 1 2
    SetOutPath $INSTDIR    
    File ..\Sources\BIN\Release\CAN_IXXAT_VCI.dll	
SectionEnd
Section "Vision Systems GmbH VSCAN API"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\CAN_VSCOM.dll
    File ..\Sources\BIN\Release\vs_can_api.dll
SectionEnd

Section "ETAS ISOLAR-CAN"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\CAN_ISOLAR_EVE_VCAN.dll
SectionEnd


Section "SPX DS API"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\CAN_i-VIEW.dll
SectionEnd
Section "NSI API"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BIN\Release\CAN_NSI.dll
SectionEnd

SectionGroupEnd
SectionGroup "Registry Cleanup"
Section "BUSMASTER Reg Cleanup"
    SectionIn RO 1 2 3
    SetOutPath $INSTDIR
    File BUSMASTER_Cleanup_Registry.exe
SectionEnd
SectionGroupEnd
SectionGroup "New Icon Set"
Section "Oxygen Icons"
    SectionIn 1 2
    SetOutPath $INSTDIR
	; Oxygen icons resource Dll
	File ..\Sources\BIN\Release\AdvancedUIPlugIn.dll
	 ; Help
    File /oname=BUSMASTER.chm "..\Documents\4 Help_new\out\help.chm"
SectionEnd
SectionGroupEnd
SectionGroup "Examples"
Section "COM Examples"
    SectionIn 1 2
    SetOutPath "$INSTDIR\Examples\COM\"
	; COM API example files
	File ..\Examples\COM\VC++COM_Client.zip
SectionEnd
SectionGroupEnd

; Uninstall section here...
Section "Uninstall"
    ; Unregister server
    SetOutPath $INSTDIR
    ExecWait 'BusEmulation.exe /unregserver'
    ExecWait 'BUSMASTER.exe /unregserver'    
	
    ; Delete registration entries
	; PTV
	DeleteRegKey HKCU "Software\RBEI-ETAS\BUSMASTER_v${VERSION}"
	DeleteRegKey HKCU "Software\RBEI-ETAS"
	
	DeleteRegValue HKLM "Software\BUSMASTER_v${VERSION}" "Install_Dir"
	DeleteRegKey   HKLM "Software\BUSMASTER_v${VERSION}"
    ; PTV END
	
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}"

    ; Compatibility settings
    DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe"
    DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe"
    DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\FormatConverter.exe"
    ; Delete installation folder
	; PTV
	 SetOutPath "$TEMP"
     RMDir /r "$INSTDIR"	
	 
	 ; Delete desktop shortcut
	Delete "$DESKTOP\BUSMASTER v${VERSION}.lnk"
	; Delete shortcut from start menu
	Delete "$SMPROGRAMS\BUSMASTER v${VERSION}.lnk"
	
	 SetShellVarContext all
	
	; Delete start menu entries
    Delete "$SMPROGRAMS\BUSMASTER v${VERSION}\Uninstall.lnk"
    Delete "$SMPROGRAMS\BUSMASTER v${VERSION}\BUSMASTER v${VERSION}.lnk"
	Delete "$SMPROGRAMS\BUSMASTER v${VERSION}\BUSMASTER_Cleanup_Registry.lnk"
	
	; Deleting If StartPrograms BUSMASTER dir exists
	IfFileExists "$SMPROGRAMS\BUSMASTER v${VERSION}" bSSBMDirExists
	bSSBMDirExists:
			RMDir /r "$SMPROGRAMS\BUSMASTER v${VERSION}"
SectionEnd