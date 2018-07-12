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
!include "MUI2.nsh"

!include "Sections.nsh"

!define MUI_WELCOMEFINISHPAGE_BITMAP "welcomefinishpage.bmp"
!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of BUSMASTER on your system."
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "headerimage.bmp"
!define MUI_ICON  "..\Sources\BUSMASTER\Application\Res\BUSMASTER.ico"
!define MUI_UNICON "..\Sources\BUSMASTER\Application\Res\Uninstaller.ico"

!define Flexray_Key "1"
!define Instruments_Key "2"
!define CANFD_Key "2"

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

var COMP_BM
var COMP_ADD_ONS
var COMP_FLEXRAY
var COMP_Instruments
var COMP_CANFD
var COMP_PAGE_INIT
var DeleteLicenseFiles

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

; Title of this installation
Name "BUSMASTER"

; Version Information
; (see http://msdn.microsoft.com/en-us/library/windows/desktop/aa381058%28v=vs.85%29.aspx)
VIProductVersion "${VERSION}.0" ; windows apps require four numbers. let's just set the fourth to zero.
VIAddVersionKey "Comments" "Bus Monitoring and Analysis Tool"
VIAddVersionKey "CompanyName" "Robert Bosch Engineering and Business Solutions (RBEI)"
VIAddVersionKey "FileDescription" "BUSMASTER"
VIAddVersionKey "FileVersion" "${VERSION}"
VIAddVersionKey "InternalName" "BUSMASTER_Installer_Ver_${VERSION}.exe"
VIAddVersionKey "LegalCopyright" "Copyright Robert Bosch Engineering and Business Solutions (RBEI)"
VIAddVersionKey "LegalTrademarks" "BUSMASTER is a registered trademark of ETAS GmbH"
VIAddVersionKey "PrivateBuild" "" ; e.g. Built by TESTER1 on \TESTBED
VIAddVersionKey "ProductName" "BUSMASTER"
VIAddVersionKey "ProductVersion" "${VERSION}"
VIAddVersionKey "SpecialBuild" "" ; e.g. Private build for TESTER1 solving mouse problems on M250 and M250E computers

; Do a CRC check when initializing setup
CRCCheck On

; Output filename
Outfile "BUSMASTER_Installer_Ver_${VERSION}.exe"
RequestExecutionLevel admin ; Request for admin rights of installation and uninstallation 

Function .onInit
  ; the plugins dir is automatically deleted when the installer exits
  InitPluginsDir
  File /oname=$PLUGINSDIR\Splsh16.bmp "..\Sources\BUSMASTER\BIN\Release\Splsh16.bmp"
  ;optional
  ;File /oname=$PLUGINSDIR\splash.wav "C:\myprog\sound.wav"

  advsplash::show 1500 800 700 -1 $PLUGINSDIR\Splsh16

  Pop $0 ; $0 has '1' if the user closed the splash screen early,
         ;        '0' if everything closed normally, and
         ;       '-1' if some error occurred.
		 

FunctionEnd

; The default installation folder
InstallDir "$PROGRAMFILES\BUSMASTER_v${VERSION}"

; Folder selection prompt
DirText "Please select an installation folder."

Var LABEL
Function onClickMinGWLink
  ExecShell "open" "http://sourceforge.net/projects/tdm-gcc/files/TDM-GCC%20Installer/Previous/1.1309.0/tdm-gcc-4.8.1.exe/download"
FunctionEnd

Function onClickBMHelpLink
  StrCpy $0 $WINDIR\hh.exe
  Exec '"$0" mk:@MSITStore:$INSTDIR\BUSMASTER.chm::/topics/MinGW_Installation_TDM.html'
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
  ${NSD_CreateLabel} 0 60 100% 40  "Microsoft .NET Framework is not installed! Please install from below link:"
  ${NSD_CreateLink} 0 115 100% 12   "http://www.microsoft.com/en-in/download/details.aspx?id=17851"

NETFrameworkInstalled:





  ${NSD_CreateLabel} 0 60 100% 40 "-> The following steps should be followed to install MinGW folder for Node simulation. $\n Download latest mingw executable from the following link:"
  Pop $Label
  ${NSD_CreateLink} 0 115 100% 12 "MinGW tdm-gcc-4.8.1"
  Pop $Label
  ${NSD_OnClick} $Label onClickMinGWLink
  ${NSD_CreateLabel}  0 140 100% 20 "use it to download and install required GCC (C,C++) compilers."
  ${NSD_CreateLink}   0 160 100% 30 "Refer BUSMASTER Help file MinGW section for detailed information."
  Pop $Label
  ${NSD_OnClick} $Label onClickBMHelpLink


  nsDialogs::Show
FunctionEnd

Function UpdateIcsneo40
  ; install icsneo40.dll unless the file is already installed and is newer
  IfFileExists "$SYSDIR\icsneo40.dll" icsneo40_upgrade icsneo40_install

  ; compare versions
icsneo40_upgrade:
  GetDLLVersion "$SYSDIR\icsneo40.dll" $R0 $R1
  GetDLLVersionLocal "..\Sources\BUSMASTER\BIN\Release\icsneo40.dll" $R2 $R3
  IntCmpU $R0 $R2 +1 icsneo40_install icsneo40_done ; compare major version
  IntCmpU $R1 $R3 +1 icsneo40_install icsneo40_done ; compare minor version
  Goto icsneo40_done

  ; install file
icsneo40_install:
  SetOutPath $SYSDIR
  File ..\Sources\BUSMASTER\BIN\Release\icsneo40.dll

icsneo40_done:
FunctionEnd



; Checks for the version if it is already installed
Function CheckVersion
  ; Read the BUSMASTER version installed
  ReadRegStr $1 HKLM "SOFTWARE\Wow6432Node" BUSMASTER_v${VERSION}"
  StrCmp $1 "v${VERSION}" 0 Confirm ;StrCmp str1 str2 jump_if_equal [jump_if_not_equal]

Confirm:
  ; If the version is already installed, get the the installtion path
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
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ../COPYING.LESSER.txt
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
Page Custom information
!insertmacro MUI_PAGE_FINISH

!define MUI_PAGE_CUSTOMFUNCTION_PRE un.ComponentsPage_Pre
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE un.ComponentsPage_Leave

!insertmacro MUI_UNPAGE_COMPONENTS
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE English

; Installation Types
InstType "Typical"
InstType "Full"
InstType "Minimal"

SectionGroup "Main"

Section "BUSMASTER"
  Call CheckVersion
  SectionIn RO 1 2 3
  SetOutPath $INSTDIR

  ; If the file exists delete it before installing
  
  ;DBCache
  ReadEnvStr $R7 "ALLUSERSPROFILE"
  StrCpy $1 "\BUSMASTER\General\DBCache"
  StrCpy $R7 $R7$1
  RMDir /r  "$R7"
  
  ; Kernel
  Delete "$INSTDIR\BusmasterKernel.dll"
  
  ; Driver Interface
  Delete "$INSTDIR\BusmasterDriverInterface.dll"
  
  Delete "$INSTDIR\BusEmulation.exe"
  Delete "$INSTDIR\BUSMASTER.exe"
  Delete "$INSTDIR\BUSMASTER.exe.manifest"
  Delete "$INSTDIR\BUSMASTER.tlb"
  Delete "$INSTDIR\BUSMASTER_Interface.c"
  Delete "$INSTDIR\BUSMASTER_Interface.h"
  Delete "$INSTDIR\CAN_ETAS_BOA.dll"
  Delete "$INSTDIR\CAN_ETAS_BOA_1_4.dll"
  Delete "$INSTDIR\CAN_ETAS_BOA_1_5.dll"
  Delete "$INSTDIR\CAN_ETAS_BOA_2.dll"
  Delete "$INSTDIR\FLEXRAY_ETAS_BOA_1_4.dll"
  Delete "$INSTDIR\FLEXRAY_ETAS_BOA_1_5.dll"
  Delete "$INSTDIR\FLEXRAY_ETAS_BOA_2.dll"
  Delete "$INSTDIR\FlexRay_GIGATRONIK_flex-i.dll"
  Delete "$INSTDIR\LIN_ETAS_BOA.dll"
  Delete "$INSTDIR\LIN_Kvaser.dll"
  Delete "$INSTDIR\linlib.dll"
  Delete "$INSTDIR\LIN_Vector_XL.dll"
  Delete "$INSTDIR\LIN_PEAK_USB.dll"
  Delete "$INSTDIR\CAN_ICS_neoVI.dll"
  Delete "$INSTDIR\CAN_Kvaser_CAN.dll"
  Delete "$INSTDIR\CAN_MHS.dll"
  Delete "$INSTDIR\CAN_PEAK_USB.dll"
  Delete "$INSTDIR\CAN_STUB.dll"
  Delete "$INSTDIR\CAN_Vector_XL.dll"
  Delete "$INSTDIR\CAN_IXXAT_VCI.dll"
  Delete "$INSTDIR\CAN_VSCOM.dll"
  Delete "$INSTDIR\CAN_ISOLAR_EVE_VCAN.dll"
  Delete "$INSTDIR\CAN_i-VIEW.dll"
  Delete "$INSTDIR\CAN_NSI.dll"
  Delete "$INSTDIR\Changelog.txt"
  Delete "$INSTDIR\DIL_Interface.dll"
  Delete "$INSTDIR\DIL_J1939.dll"
  Delete "$INSTDIR\UDS_Protocol.dll"
  Delete "$INSTDIR\Filter.dll"
  Delete "$INSTDIR\FrameProcessor.dll"
  Delete "$INSTDIR\GCCDLLMakeTemplate"
  Delete "$INSTDIR\GCCDLLMakeTemplate_CAN"
  Delete "$INSTDIR\GCCDLLMakeTemplate_J1939"
  Delete "$INSTDIR\GCCDLLMakeTemplate_LIN"
  Delete "$INSTDIR\GCCDLLMakeTemplate_FlexRay"
  Delete "$INSTDIR\mhsbmcfg.dll"
  Delete "$INSTDIR\NodeSimEx.dll"
  Delete "$INSTDIR\ProjectConfiguration.dll"
  Delete "$INSTDIR\PSDI_CAN.dll"
  Delete "$INSTDIR\Replay.dll"
  Delete "$INSTDIR\SignalWatch.dll"
  Delete "$INSTDIR\TestSetupEditorGUI.dll"
  Delete "$INSTDIR\TestSetupEditorGUI.exe"
  Delete "$INSTDIR\TestSuiteExecutorGUI.dll"
  Delete "$INSTDIR\TXWindow.dll"
  Delete "$INSTDIR\FormatConverter.exe"
  Delete "$INSTDIR\SigGrphWnd.dll"
  Delete "$INSTDIR\SignalDefiner.dll"
  ;RMDir /r "$INSTDIR\ConverterPlugins"
  Delete "$INSTDIR\BUSMASTER.chm"
  Delete "$INSTDIR\LDFEditor.chm"
  Delete "$INSTDIR\COPYING.LESSER.txt"
  Delete "$INSTDIR\COPYING.txt"
  Delete "$INSTDIR\FTL.txt"
  Delete "$INSTDIR\ReleaseNotes.txt"
  RMDir /r "$INSTDIR\SimulatedSystems"
  Delete "$INSTDIR\Readme.txt"
  Delete "$INSTDIR\DBManager_License.txt"
  Delete "$INSTDIR\DMGraph.dll"
  Delete "$INSTDIR\ETASneo40.dll"
  Delete "$INSTDIR\icsneo40.dll"
  Delete "$INSTDIR\canlib32.dll"
  Delete "$INSTDIR\mhstcan.dll"
  Delete "$INSTDIR\CanApi2.dll"
  Delete "$INSTDIR\vxlapi.dll"
  Delete "$INSTDIR\iconv.dll"
  Delete "$INSTDIR\libxml2.dll"
  Delete "$INSTDIR\zlib1.dll"
  Delete "$INSTDIR\intl.dll"
  RMDir /r "$INSTDIR\JPN"
  RMDir /r "$INSTDIR\MinGW"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\BUSMASTER_Cleanup_Registry.exe"
  Delete "$INSTDIR\AdvancedUIPlugIn.dll"
  Delete "$INSTDIR\LIN_ISOLAR_EVE_VLIN.dll"
  Delete "$INSTDIR\TXWindowLIN.dll"
  Delete "$INSTDIR\Controller_0.dll"
  Delete "$INSTDIR\Controller_1.dll"
  Delete "$INSTDIR\LDFEditor.exe"
  Delete "$INSTDIR\LDFViewer.exe"
  Delete "$INSTDIR\DBManager.dll"
  RMDir /r "$INSTDIR\platforms"
  Delete "$INSTDIR\icudt52.dll"
  Delete "$INSTDIR\icuin52.dll"
  Delete "$INSTDIR\icuuc52.dll"
  Delete "$INSTDIR\qdds.dll"
  Delete "$INSTDIR\qgif.dll"
  Delete "$INSTDIR\qicns.dll"
  Delete "$INSTDIR\qico.dll"
  Delete "$INSTDIR\qjp2.dll"
  Delete "$INSTDIR\qjpeg.dll"
  Delete "$INSTDIR\qminimal.dll"
  Delete "$INSTDIR\qmng.dll"
  Delete "$INSTDIR\qoffscreen.dll"
  Delete "$INSTDIR\qsvg.dll"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\Qt5Gui.dll"
  Delete "$INSTDIR\Qt5Widgets.dll"
  Delete "$INSTDIR\qtaccessiblewidgets.dll"
  Delete "$INSTDIR\qtga.dll"
  Delete "$INSTDIR\qtiff.dll"
  Delete "$INSTDIR\qwbmp.dll"
  Delete "$INSTDIR\qwebp.dll"
  Delete "$INSTDIR\Application.version"
  
  
  Delete "$INSTDIR\ConverterPlugins\AscLogConverter.dll"
  Delete "$INSTDIR\ConverterPlugins\BlfLibrary.dll"
  Delete "$INSTDIR\ConverterPlugins\BlfLogConverter.dll"
  Delete "$INSTDIR\ConverterPlugins\CAPL2CConverter.dll"
  Delete "$INSTDIR\ConverterPlugins\CAPL2CConverterJPN.dll"
  Delete "$INSTDIR\ConverterPlugins\DBC2DBFConverter.dll"
  Delete "$INSTDIR\ConverterPlugins\DBC2DBFConverterLibrary.dll"
  Delete "$INSTDIR\ConverterPlugins\DBF2DBCConverter.dll"
  Delete "$INSTDIR\ConverterPlugins\intl.dll"
  Delete "$INSTDIR\ConverterPlugins\J1939DBC2DBFConverter.dll"
  Delete "$INSTDIR\ConverterPlugins\libxml2.dll"
  Delete "$INSTDIR\ConverterPlugins\LogAscConverter.dll"
  Delete "$INSTDIR\ConverterPlugins\LogToExcelConverter.dll"
  Delete "$INSTDIR\ConverterPlugins\LogToExcelConverterJPN.dll"
  Delete "$INSTDIR\ConverterPlugins\zlib1.dll"
  
  ; Delete schema file
  Delete "$INSTDIR\BusMasterPluginSchema.xsd"
  
  
  ; Kernel
  File ..\Sources\BUSMASTER\BIN\Release\BusmasterKernel.dll
  
  ; Driver Interface
  File ..\Sources\BUSMASTER\BIN\Release\BusmasterDriverInterface.dll

  ; BUSMASTER
  File ..\Sources\BUSMASTER\BIN\Release\BusEmulation.exe
  File ..\Sources\BUSMASTER\BIN\Release\BUSMASTER.exe
  File ..\Sources\BUSMASTER\BIN\Release\BUSMASTER.exe.manifest
  File ..\Sources\BUSMASTER\Application\BUSMASTER.tlb
  File ..\Sources\BUSMASTER\Application\BUSMASTER_Interface.c
  File ..\Sources\BUSMASTER\Application\BUSMASTER_Interface.h
  File ..\Sources\BUSMASTER\BIN\Release\CAN_STUB.dll
  File ..\Sources\BUSMASTER\BIN\Release\Changelog.txt
  File ..\Sources\BUSMASTER\BIN\Release\DIL_J1939.dll
  File ..\Sources\BUSMASTER\BIN\Release\UDS_Protocol.dll
  File ..\Sources\BUSMASTER\BIN\Release\Filter.dll
  File ..\Sources\BUSMASTER\BIN\Release\FrameProcessor.dll
  File ..\Sources\BUSMASTER\BIN\Release\mhsbmcfg.dll
  File ..\Sources\BUSMASTER\BIN\Release\NodeSimEx.dll
  File ..\Sources\BUSMASTER\BIN\Release\ProjectConfiguration.dll
  File ..\Sources\BUSMASTER\BIN\Release\PSDI_CAN.dll
  File ..\Sources\BUSMASTER\BIN\Release\Replay.dll
  File ..\Sources\BUSMASTER\BIN\Release\SignalWatch.dll
  File ..\Sources\BUSMASTER\BIN\Release\TestSetupEditorGUI.exe
  File ..\Sources\BUSMASTER\BIN\Release\TestSuiteExecutorGUI.dll
  File ..\Sources\BUSMASTER\BIN\Release\TXWindow.dll
  File ..\Sources\BUSMASTER\BIN\Release\FormatConverter.exe
  File ..\Sources\BUSMASTER\BIN\Release\SigGrphWnd.dll
  File ..\Sources\BUSMASTER\BIN\Release\SignalDefiner.dll
  ;File ..\Sources\BUSMASTER\BIN\Release\iconv.dll
  File ..\Sources\BUSMASTER\BIN\Release\libxml2.dll
  File ..\Sources\BUSMASTER\BIN\Release\zlib1.dll
  File ..\Sources\BUSMASTER\BIN\Release\intl.dll
  File ..\Sources\BUSMASTER\BIN\Release\LIN_ISOLAR_EVE_VLIN.dll
  File ..\Sources\BUSMASTER\BIN\Release\LIN_ETAS_BOA.dll
  File ..\Sources\BUSMASTER\BIN\Release\LIN_Kvaser.dll
  File ..\Sources\BUSMASTER\BIN\Release\linlib.dll
  File ..\Sources\BUSMASTER\BIN\Release\Controller_0.dll
  File ..\Sources\BUSMASTER\BIN\Release\Controller_1.dll
  File ..\Sources\BUSMASTER\BIN\Release\LIN_Vector_XL.dll
  File ..\Sources\BUSMASTER\BIN\Release\LIN_PEAK_USB.dll
  File ..\Sources\BUSMASTER\BIN\Release\LDFEditor.exe
  File ..\Sources\BUSMASTER\BIN\Release\LDFViewer.exe
  File ..\Sources\BUSMASTER\BIN\Release\DBManager.dll
  File ..\Sources\BUSMASTER\BIN\Release\icudt52.dll
  File ..\Sources\BUSMASTER\BIN\Release\icuin52.dll
  File ..\Sources\BUSMASTER\BIN\Release\icuuc52.dll
  File ..\Sources\BUSMASTER\BIN\Release\qdds.dll
  File ..\Sources\BUSMASTER\BIN\Release\qgif.dll
  File ..\Sources\BUSMASTER\BIN\Release\qicns.dll
  File ..\Sources\BUSMASTER\BIN\Release\qico.dll
  File ..\Sources\BUSMASTER\BIN\Release\qjp2.dll
  File ..\Sources\BUSMASTER\BIN\Release\qjpeg.dll
  File ..\Sources\BUSMASTER\BIN\Release\qminimal.dll
  File ..\Sources\BUSMASTER\BIN\Release\qmng.dll
  File ..\Sources\BUSMASTER\BIN\Release\qoffscreen.dll
  File ..\Sources\BUSMASTER\BIN\Release\qsvg.dll
  File ..\Sources\BUSMASTER\BIN\Release\Qt5Core.dll
  File ..\Sources\BUSMASTER\BIN\Release\Qt5Gui.dll
  File ..\Sources\BUSMASTER\BIN\Release\Qt5Widgets.dll
  File ..\Sources\BUSMASTER\BIN\Release\qtaccessiblewidgets.dll
  File ..\Sources\BUSMASTER\BIN\Release\qtga.dll
  File ..\Sources\BUSMASTER\BIN\Release\qtiff.dll
  File ..\Sources\BUSMASTER\BIN\Release\qwbmp.dll
  File ..\Sources\BUSMASTER\BIN\Release\qwebp.dll
  File ..\Sources\BUSMASTER\BIN\Release\Application.version

  ; PlugIn schema file
  File ..\Sources\BUSMASTER\BusMasterPluginSchema.xsd
  
  ; Converters
  ;File /r ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins
  SetOutPath "$INSTDIR\ConverterPlugins"
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\AscLogConverter.dll
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\BlfLibrary.dll
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\BlfLogConverter.dll
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\CAPL2CConverter.dll
  ;File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\CAPL2CConverterJPN.dll
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\DBC2DBFConverter.dll
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\DBC2DBFConverterLibrary.dll
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\DBF2DBCConverter.dll
  File ..\Sources\BUSMASTER\BIN\Release\intl.dll
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\J1939DBC2DBFConverter.dll
  File ..\Sources\BUSMASTER\BIN\Release\libxml2.dll
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\LogAscConverter.dll
  File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\LogToExcelConverter.dll
  ;File ..\Sources\BUSMASTER\BIN\Release\ConverterPlugins\LogToExcelConverterJPN.dll
  File ..\Sources\BUSMASTER\BIN\Release\zlib1.dll

  SetOutPath $INSTDIR
  ; Japanese lib files
  ;File /r ..\Sources\BUSMASTER\BIN\Release\JPN

  ; Japanese Localization folder
  ;File /r ..\Sources\BUSMASTER\Localization

  ; Help
  ;File /oname=BUSMASTER.chm "..\Documents\4 Help\out\help.chm"
  File ..\Sources\BUSMASTER\BIN\Release\BUSMASTER.chm
  
  ; LDF Editor Help
  File /oname=LDFEditor.chm "..\Documents\4 Help\out\ldfeditor.chm"
  
  ; Oxygen icons resource Dll
  File ..\Sources\BUSMASTER\BIN\Release\AdvancedUIPlugIn.dll

  ; License
  File ..\COPYING.LESSER.txt
  File ..\COPYING.txt
  File ..\FTL.txt

  ; Readme
  File ..\Readme.txt
  File ..\DBManager_License.txt
  File ..\ReleaseNotes.txt
  
  File /r ..\Sources\BUSMASTER\BIN\Release\platforms

  ;Add dlls to ConverterPlugins
  SetOutPath "$INSTDIR\ConverterPlugins"
  ;File ..\Sources\BUSMASTER\BIN\Release\iconv.dll
  File ..\Sources\BUSMASTER\BIN\Release\intl.dll
  File ..\Sources\BUSMASTER\BIN\Release\libxml2.dll
  File ..\Sources\BUSMASTER\BIN\Release\zlib1.dll
	
  ; Simulated Systems Include files
  SetOutPath "$INSTDIR\SimulatedSystems\include\"
  
  ; Common Files
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\BMSignal.h
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\BMUtility.h
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\Common.h
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\Wrapper_Common.h
  
  ; CAN Related 
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\CANIncludes.h
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\BMCANDefines.h
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\CANCAPLWrapper.h
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\CAPLWrapper.h
  
  ; J1939 Related
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\J1939Includes.h
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\BMJ1939Defines.h
  
  ; LIN Related
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\LINIncludes.h
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\include\BMLINDefines.h
  
  ; Simulated Systems Library files
  SetOutPath "$INSTDIR\SimulatedSystems\OBJ\"
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\OBJ\libWrapper_CAN.a
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\OBJ\libWrapper_LIN.a
  File ..\Sources\BUSMASTER\Application\SimulatedSystems\OBJ\libWrapper_J1939.a

  ; GCC Make Files
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\Application\GCCDLLMakeTemplate
  

  ; Check if Visual Studio 2012 redistributable is already installed
  ;ReadRegStr $1 HKLM "Software\Microsoft\DevDiv\vc\Servicing\11.0\RuntimeMinimum" Install
  ;StrCmp $1 "1" NoInstall Install

;Install:
  ; Install Visual Studio 2013 Redistributable
  File "..\Tools\VC++ 2013 Redistributable\vcredist_x86.exe"
  ExecWait '"vcredist_x86.exe" /s /v" /qn"'
  Delete "$INSTDIR\vcredist_x86.exe"

;NoInstall:

  ; create desktop shortcut
  CreateShortCut "$DESKTOP\BUSMASTER v${VERSION}.lnk" "$INSTDIR\BUSMASTER.exe" ""

  ; Registry entries for uninstaller
  WriteRegStr HKLM "Software\BUSMASTER_v${VERSION}" "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "Software\BUSMASTER_v${VERSION}" "Flexray_Key" "${Flexray_Key}"
  WriteRegStr HKLM "Software\BUSMASTER_v${VERSION}" "Instruments_Key" "${Instruments_Key}"
  WriteRegStr HKLM "Software\BUSMASTER_v${VERSION}" "CANFD_Key" "${CANFD_Key}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "DisplayName" "BUSMASTER Ver ${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "InstallLocation " "$INSTDIR\"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "DisplayIcon" "$INSTDIR\BUSMASTER.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "Publisher" "Robert Bosch Engineering and Business Solutions"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "HelpLink" "http://rbei-etas.github.io/busmaster/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "URLUpdateInfo" "http://rbei-etas.github.io/busmaster/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "URLInfoAbout" "http://rbei-etas.github.io/busmaster/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "DisplayVersion" "${VERSION}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "NoModify" "1"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}" "NoRepair" "1"

  ; Compatibility settings for Windows 7
  ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  StrCmp $1 "6.1" 0 lbl ;StrCmp str1 str2 jump_if_equal [jump_if_not_equal]
  ; WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe" "WINXPSP3"
  ; WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\FormatConverter.exe" "WINXPSP3"
  ; WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe" "RUNASADMIN"

lbl:

  ; Server registration
  ExecWait 'BusEmulation.exe /regserver'
  ExecWait 'BUSMASTER.exe /regserver'

  SetShellVarContext all

  ; Uninstaller
  WriteUninstaller "uninst.exe"	
  
  ; Start menu entries
  CreateDirectory "$SMPROGRAMS\BUSMASTER v${VERSION}"
  CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\BUSMASTER v${VERSION}.lnk" "$INSTDIR\BUSMASTER.exe" "" "$INSTDIR\BUSMASTER.exe" 0
  CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\BUSMASTER_Cleanup_Registry.lnk" "$INSTDIR\BUSMASTER_Cleanup_Registry.exe" "" "$INSTDIR\BUSMASTER_Cleanup_Registry.exe" 0
  CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\Uninstall.lnk" "$INSTDIR\uninst.exe" "" "$INSTDIR\uninst.exe" 0
  CreateDirectory "$SMPROGRAMS\BUSMASTER v${VERSION}\Tools"
  CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\Tools\LIN Database Editor.lnk" "$INSTDIR\LDFEditor.exe" "" "$INSTDIR\LDFEditor.exe" 0
  CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\Tools\Format Converter.lnk" "$INSTDIR\FormatConverter.exe" "" "$INSTDIR\FormatConverter.exe" 0

SectionEnd

Section "DMGraph"
    SectionIn RO 1 2 3
    SetOutPath $INSTDIR
    File ..\Sources\BUSMASTER\BIN\ReleaseUMinSize\DMGraph.dll
SectionEnd

SectionGroupEnd

SectionGroup "Hardware Interfaces"

Section "ETAS ES581"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_ICS_neoVI.dll
  Call UpdateIcsneo40
SectionEnd

Section "ETAS BOA"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_ETAS_BOA_1_4.dll
  File ..\Sources\BUSMASTER\BIN\Release\CAN_ETAS_BOA_1_5.dll
  File ..\Sources\BUSMASTER\BIN\Release\CAN_ETAS_BOA_2.dll
SectionEnd

Section "Intrepid neoVI"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_ICS_neoVI.dll
  Call UpdateIcsneo40
SectionEnd

Section "Kvaser CAN"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_Kvaser_CAN.dll
  ; File ..\Sources\BUSMASTER\BIN\Release\canlib32.dll
SectionEnd

Section "MHS-Elektronik Tiny-CAN"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_MHS.dll
  File ..\Sources\BUSMASTER\CAN_MHS\EXTERNAL\mhstcan.dll
SectionEnd

Section "Peak USB"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_PEAK_USB.dll
  File ..\Sources\BUSMASTER\BIN\Release\CanApi2.dll
SectionEnd

Section "Vector XL"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_Vector_XL.dll
  File ..\Sources\BUSMASTER\BIN\Release\vxlapi.dll
SectionEnd

Section "IXXAT VCI"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_IXXAT_VCI.dll
SectionEnd

Section "Vision Systems GmbH VSCAN API"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_VSCOM.dll
  File ..\Sources\BUSMASTER\BIN\Release\vs_can_api.dll
SectionEnd

Section "ETAS ISOLAR-CAN"
    SectionIn 1 2
    SetOutPath $INSTDIR
    File ..\Sources\BUSMASTER\BIN\Release\CAN_ISOLAR_EVE_VCAN.dll
SectionEnd

Section "SPX DS API"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_i-VIEW.dll
SectionEnd

Section "NSI API"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BUSMASTER\BIN\Release\CAN_NSI.dll
SectionEnd

SectionGroupEnd

SectionGroup "Registry Cleanup"

Section "BUSMASTER Reg Cleanup"
  SectionIn RO 1 2 3
  SetOutPath $INSTDIR
  File BUSMASTER_Cleanup_Registry.exe
SectionEnd

SectionGroupEnd

SectionGroup "Examples"

Section "COM Examples"
  SectionIn 1 2
  SetOutPath "$INSTDIR\Examples\COM\"
  File ..\Examples\COM\VC++COM_Client.zip
SectionEnd
Section "Test Automation"
  SectionIn 1 2
  SetOutPath "$INSTDIR\Examples\TestAutomation\"
  File ..\Examples\TestAutomation\CAN_Database.dbf
  File ..\Examples\TestAutomation\SampleTestSetupFile.xml
  File ..\Examples\TestAutomation\TestSetupFile.xsd
SectionEnd

SectionGroupEnd

Function un.onInit
StrCpy $COMP_PAGE_INIT "1"
StrCpy $DeleteLicenseFiles "0"
StrCpy $COMP_FLEXRAY "0"
StrCpy $COMP_Instruments "0"
StrCpy $COMP_ADD_ONS "0"
FunctionEnd

; Uninstall section here...
Section un.BUSMASTER Uninstall 
  
  IntOp $COMP_BM $COMP_BM & ${SF_SELECTED}
   ${If} $COMP_BM != "0"
 ; Prompt user to close all instances of BUSMASTER.
  ${Do}
  ${nsProcess::FindProcess} "BUSMASTER.exe" $R0
  ${If} $R0 == 0
  MessageBox MB_OK|MB_ICONSTOP "Please close all instances of BUSMASTER and Click 'OK'"
  ${EndIf}
  ${LoopUntil} $R0 != 0
  
  ; Kill process BusEmulation if active.
  ${Do}
  ${nsProcess::KillProcess} "BusEmulation.exe" $R0
  ${LoopUntil} $R0 != 0
  
  ; Unregister server
  SetOutPath $INSTDIR
  
  
    
  ExecWait 'BusEmulation.exe /unregserver'
  ExecWait 'BUSMASTER.exe /unregserver'

  ; Delete registration entries
  DeleteRegKey HKCU "Software\RBEI-ETAS\BUSMASTER_v${VERSION}"
  ;Remove HKCU "Software\RBEI-ETAS" if the current version is the last version under this key
  StrCpy $0 0
  loop:
  EnumRegKey $1 HKCU Software\RBEI-ETAS $0
  StrCmp $1 "" done
  IntOp $0 $0 + 1
  Goto loop
  done:
  IntCmp $0 0 is0 lessthan0 morethan0
  lessthan0: Goto Finish
  morethan0: Goto Finish
  is0: DeleteRegKey HKCU "Software\RBEI-ETAS"
  Finish:
  DeleteRegValue HKLM "Software\BUSMASTER_v${VERSION}" "Install_Dir"
  DeleteRegKey HKLM "Software\BUSMASTER_v${VERSION}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BUSMASTER_v${VERSION}"

  ; Compatibility settings
  ; DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe"
  ; DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe"
  ; DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\FormatConverter.exe"

  ; Delete installation folder
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
  
  Delete "$SMPROGRAMS\BUSMASTER v${VERSION}\Tools\LIN Database Editor.lnk"
  Delete "$SMPROGRAMS\BUSMASTER v${VERSION}\Tools\Format Converter.lnk"
  
  RMDir /r "$SMPROGRAMS\BUSMASTER v${VERSION}\Tools"
  
  ; Deleting If StartPrograms BUSMASTER dir exists
  RMDir /r "$SMPROGRAMS\BUSMASTER v${VERSION}" 
  ${EndIf}
  
SectionEnd

SectionGroup "un.AddOns" Section_AddOns
Section /o un.FlexRay Section_FlexRay

IntOp $COMP_FLEXRAY $COMP_FLEXRAY & ${SF_SELECTED}
${if} $COMP_FLEXRAY == "1"

  ; Prompt user to close all instances of BUSMASTER.
  ${Do}
  ${nsProcess::FindProcess} "BUSMASTER.exe" $R0
  ${If} $R0 == 0
  MessageBox MB_OK|MB_ICONSTOP "Please close all instances of BUSMASTER and Click 'OK'"
  ${EndIf}
  ${LoopUntil} $R0 != 0
  
  ; Kill process BusEmulation if active.
  ${Do}
  ${nsProcess::KillProcess} "BusEmulation.exe" $R0
  ${LoopUntil} $R0 != 0
  
  ; Unregister server
  SetOutPath $INSTDIR
  
${If} $DeleteLicenseFiles == "1"
  Delete "$INSTDIR\LicenseValidator.dll"
  Delete "$INSTDIR\MacIdLicenceManager.dll"
  ;RMDir /r "$INSTDIR\BusmasterBusPlugins"
  ;RMDir /r "$INSTDIR\BusmasterPlugins"
  ${EndIf}  
  
;ExecWait '"$PROGRAMFILES\BUSMASTER_v${VERSION}\BusmasterBusPlugins\FlexRay\uninst.exe" /S'
ExecWait '"$INSTDIR\BusmasterBusPlugins\FlexRay\uninst.exe" /S'

${EndIf}
SectionEnd

Section /o un.Instruments Section_Instruments

IntOp $COMP_Instruments $COMP_Instruments & ${SF_SELECTED}
${if} $COMP_Instruments == "1"
   
  ; Prompt user to close all instances of BUSMASTER.
  ${Do}
  ${nsProcess::FindProcess} "BUSMASTER.exe" $R0
  ${If} $R0 == 0
  MessageBox MB_OK|MB_ICONSTOP "Please close all instances of BUSMASTER and Click 'OK'"
  ${EndIf}
  ${LoopUntil} $R0 != 0
  
  ; Kill process BusEmulation if active.
  ${Do}
  ${nsProcess::KillProcess} "BusEmulation.exe" $R0
  ${LoopUntil} $R0 != 0
  
  ; Unregister server
  SetOutPath $INSTDIR
  
${If} $DeleteLicenseFiles == "1"
  Delete "$INSTDIR\LicenseValidator.dll"
  Delete "$INSTDIR\MacIdLicenceManager.dll"
  ;RMDir /r "$INSTDIR\BusmasterBusPlugins"
  ;RMDir /r "$INSTDIR\BusmasterPlugins"
${EndIf}
ExecWait '"$INSTDIR\BusmasterPlugins\InstrumentPlugin\uninst.exe" /S'
${EndIf}

SectionEnd

Section /o un.CANFD Section_CANFD

IntOp $COMP_CANFD $COMP_CANFD & ${SF_SELECTED}
${if} $COMP_CANFD == "1"
   
  ; Prompt user to close all instances of BUSMASTER.
  ${Do}
  ${nsProcess::FindProcess} "BUSMASTER.exe" $R0
  ${If} $R0 == 0
  MessageBox MB_OK|MB_ICONSTOP "Please close all instances of BUSMASTER and Click 'OK'"
  ${EndIf}
  ${LoopUntil} $R0 != 0
  
  ; Kill process BusEmulation if active.
  ${Do}
  ${nsProcess::KillProcess} "BusEmulation.exe" $R0
  ${LoopUntil} $R0 != 0
  
  ; Unregister server
  SetOutPath $INSTDIR
  
${If} $DeleteLicenseFiles == "1"
  Delete "$INSTDIR\LicenseValidator.dll"
  Delete "$INSTDIR\MacIdLicenceManager.dll"
  ;RMDir /r "$INSTDIR\BusmasterBusPlugins"
  ;RMDir /r "$INSTDIR\BusmasterPlugins"
${EndIf}
ExecWait '"$INSTDIR\BusmasterBusPlugins\CANFD\uninst.exe" /S' 
${EndIf}

SectionEnd


SectionGroupEnd

LangString DESC_BUSMASTER ${ENGLISH} "Select BusMaster to uninstall BusMaster including AddOns"
LangString DESC_AddOns ${ENGLISH} "Select AddOns to uninstall BusMaster AddOns"
LangString DESC_AddOns_FlexRay ${ENGLISH} "Select FlexRay AddOn to uninstall only FlexRay AddOn"
LangString DESC_AddOns_Instruments ${ENGLISH} "Select Instruments to uninstall only Instruments AddOn"
LangString DESC_AddOns_CANFD ${ENGLISH} "Select CAN FD to uninstall only CAN FD AddOn"

!insertmacro MUI_UNFUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${Section_AddOns} $(DESC_AddOns)
  !insertmacro MUI_DESCRIPTION_TEXT ${Section_FlexRay} $(DESC_AddOns_FlexRay)
  !insertmacro MUI_DESCRIPTION_TEXT ${Section_Instruments} $(DESC_AddOns_Instruments)
  !insertmacro MUI_DESCRIPTION_TEXT ${Section_CANFD} $(DESC_AddOns_CANFD)
  !insertmacro MUI_DESCRIPTION_TEXT ${Uninstall} $(DESC_BUSMASTER)
!insertmacro MUI_UNFUNCTION_DESCRIPTION_END

Function un.ComponentsPage_Pre
	Call un.CheckAddOnDetails
	SectionGetFlags ${Uninstall} $R5
FunctionEnd

Function un.ComponentsPage_Leave
SectionGetFlags ${Uninstall} $COMP_BM
SectionGetFlags ${Section_AddOns} $COMP_ADD_ONS
SectionGetFlags ${Section_FlexRay} $COMP_FLEXRAY
SectionGetFlags ${Section_Instruments} $COMP_Instruments
SectionGetFlags ${Section_CANFD} $COMP_CANFD

StrCpy $COMP_PAGE_INIT "0"
StrCpy $DeleteLicenseFiles "0"

IntOp $COMP_ADD_ONS $COMP_ADD_ONS & ${SF_SELECTED}
${If} $COMP_ADD_ONS == "1"
StrCpy $DeleteLicenseFiles "1"
${EndIf}

FunctionEnd

Function un.CheckAddOnDetails
StrCpy $0 0
StrCpy $COMP_ADD_ONS "0"
StrCpy $COMP_FLEXRAY "0"
StrCpy $COMP_Instruments "0"
StrCpy $COMP_CANFD "0"
loop:
  EnumRegKey $1 HKCU "SOFTWARE\RBEI-ETAS\BUSMASTER_v${VERSION}\Add-Ons" $0
  StrCmp $1 "" done
  StrCpy $COMP_ADD_ONS "1"
  ${If} $1 == "FlexRay"
  StrCpy $COMP_FLEXRAY "1"
  ${EndIf}
  ${If} $1 == "Instruments"
  StrCpy $COMP_Instruments "1"
  ${EndIf}
  ${If} $1 == "CANFD"
  StrCpy $COMP_CANFD "1"
  ${EndIf}
  
  ;StrCmp $1 "" disbleBM
  IntOp $0 $0 + 1
  goto loop
  
  done:   
  ${If} $COMP_PAGE_INIT == "1"
${If} $COMP_FLEXRAY == "0" 
${AndIf} $COMP_Instruments == "0"
${AndIf} $COMP_CANFD == "0"
	SectionSetText ${Section_AddOns} ""
	SectionSetText ${Section_FlexRay} ""
	SectionSetText ${Section_Instruments} "" 
	SectionSetText ${Section_CANFD} ""
	IntOp $0 ${SF_SELECTED} | ${SF_RO}
	SectionSetFlags ${Uninstall} $0
${EndIf}
${If} $COMP_FLEXRAY == "0"	
	SectionSetText ${Section_FlexRay} ""
${EndIf}
${If} $COMP_Instruments == "0" 
 SectionSetText ${Section_Instruments} "" 
${EndIf}
${If} $COMP_CANFD == "0" 
 SectionSetText ${Section_CANFD} "" 
${EndIf}

${If} $COMP_ADD_ONS != "0"
  ${AndIf} $Uninstall != "0"
	Call un.DisableAddOns
  ${EndIf}  
  ${EndIf}
	
; Read the BUSMASTER version installed
FunctionEnd

Function un.DisableAddOns
	IntOp $0 ${SF_SELECTED} | ${SF_RO}
	!insertmacro SetSectionFlag ${Section_AddOns} $0	
	!insertmacro SetSectionFlag ${Section_FlexRay} $0	
	!insertmacro SetSectionFlag ${Section_Instruments} $0	
	!insertmacro SetSectionFlag ${Section_CANFD} $0	
FunctionEnd

Function un.onSelChange
SectionGetFlags ${Uninstall} $R0

${If} $R5 != $R0
${If} $R0 == ${SF_SELECTED}    
	Call un.DisableAddOns
${ElseIf} $R0 != ${SF_SELECTED}    
	!insertmacro ClearSectionFlag ${Section_AddOns} ${SF_RO}
	!insertmacro ClearSectionFlag ${Section_FlexRay} ${SF_RO}
	!insertmacro ClearSectionFlag ${Section_Instruments} ${SF_RO}
	!insertmacro ClearSectionFlag ${Section_CANFD} ${SF_RO}
${EndIf}
 SectionGetFlags ${Uninstall} $R5
${EndIf}

FunctionEnd