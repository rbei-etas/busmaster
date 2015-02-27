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

!define MUI_WELCOMEFINISHPAGE_BITMAP "welcomefinishpage.bmp"
!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of BUSMASTER on your system."
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "headerimage.bmp"
!define MUI_ICON  "..\Sources\Application\Res\BUSMASTER.ico"
!define MUI_UNICON "..\Sources\Application\Res\Uninstaller.ico"

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

Function .onInit
  ; the plugins dir is automatically deleted when the installer exits
  InitPluginsDir
  File /oname=$PLUGINSDIR\Splsh16.bmp "..\Sources\BIN\Release\Splsh16.bmp"
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
  Delete "$INSTDIR\TXWindowFlexRay.dll"
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
  Delete "$INSTDIR\GCCDLLMakeTemplate_CAN"
  Delete "$INSTDIR\GCCDLLMakeTemplate_J1939"
  Delete "$INSTDIR\GCCDLLMakeTemplate_LIN"
  Delete "$INSTDIR\mhsbmcfg.dll"
  Delete "$INSTDIR\NodeSimEx.dll"
  Delete "$INSTDIR\ProjectConfiguration.dll"
  Delete "$INSTDIR\PSDI_CAN.dll"
  Delete "$INSTDIR\Replay.dll"
  Delete "$INSTDIR\SignalWatch.dll"
  Delete "$INSTDIR\TestSetupEditorGUI.dll"
  Delete "$INSTDIR\TestSuiteExecutorGUI.dll"
  Delete "$INSTDIR\TXWindow.dll"
  Delete "$INSTDIR\FormatConverter.exe"
  Delete "$INSTDIR\SigGrphWnd.dll"
  Delete "$INSTDIR\SignalDefiner.dll"
  RMDir /r "$INSTDIR\ConverterPlugins"
  Delete "$INSTDIR\BUSMASTER.chm"
  Delete "$INSTDIR\LDFEditor.chm"
  Delete "$INSTDIR\COPYING.LESSER.txt"
  Delete "$INSTDIR\COPYING.txt"
  Delete "$INSTDIR\FTL.txt"
  RMDir /r "$INSTDIR\SimulatedSystems"
  Delete "$INSTDIR\Readme.txt"
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

  ; BUSMASTER
  File ..\Sources\BIN\Release\BusEmulation.exe
  File ..\Sources\BIN\Release\BUSMASTER.exe
  File ..\Sources\BIN\Release\BUSMASTER.exe.manifest
  File ..\Sources\Application\BUSMASTER.tlb
  File ..\Sources\Application\BUSMASTER_Interface.c
  File ..\Sources\Application\BUSMASTER_Interface.h
  File ..\Sources\BIN\Release\CAN_STUB.dll
  File ..\Sources\BIN\Release\Changelog.txt
  File ..\Sources\BIN\Release\DIL_Interface.dll
  File ..\Sources\BIN\Release\DIL_J1939.dll
  File ..\Sources\BIN\Release\UDS_Protocol.dll
  File ..\Sources\BIN\Release\Filter.dll
  File ..\Sources\BIN\Release\FrameProcessor.dll
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
  File ..\Sources\BIN\Release\FLEXRAY_ETAS_BOA_1_4.dll
  File ..\Sources\BIN\Release\FLEXRAY_ETAS_BOA_1_5.dll
  File ..\Sources\BIN\Release\FLEXRAY_ETAS_BOA_2.dll
  File ..\Sources\BIN\Release\FlexRay_GIGATRONIK_flex-i.dll
  File ..\Sources\BIN\Release\LIN_ETAS_BOA.dll
  File ..\Sources\BIN\Release\TXWindowFlexRay.dll
  File ..\Sources\BIN\Release\Controller_0.dll
  File ..\Sources\BIN\Release\Controller_1.dll
  File ..\Sources\BIN\Release\LIN_Vector_XL.dll
  File ..\Sources\BIN\Release\LIN_PEAK_USB.dll
  File ..\Sources\BIN\Release\LDFEditor.exe
  File ..\Sources\BIN\Release\LDFViewer.exe
  File ..\Sources\BIN\Release\DBManager.dll
  File ..\Sources\BIN\Release\icudt52.dll
  File ..\Sources\BIN\Release\icuin52.dll
  File ..\Sources\BIN\Release\icuuc52.dll
  File ..\Sources\BIN\Release\qdds.dll
  File ..\Sources\BIN\Release\qgif.dll
  File ..\Sources\BIN\Release\qicns.dll
  File ..\Sources\BIN\Release\qico.dll
  File ..\Sources\BIN\Release\qjp2.dll
  File ..\Sources\BIN\Release\qjpeg.dll
  File ..\Sources\BIN\Release\qminimal.dll
  File ..\Sources\BIN\Release\qmng.dll
  File ..\Sources\BIN\Release\qoffscreen.dll
  File ..\Sources\BIN\Release\qsvg.dll
  File ..\Sources\BIN\Release\Qt5Core.dll
  File ..\Sources\BIN\Release\Qt5Gui.dll
  File ..\Sources\BIN\Release\Qt5Widgets.dll
  File ..\Sources\BIN\Release\qtaccessiblewidgets.dll
  File ..\Sources\BIN\Release\qtga.dll
  File ..\Sources\BIN\Release\qtiff.dll
  File ..\Sources\BIN\Release\qwbmp.dll
  File ..\Sources\BIN\Release\qwebp.dll

  ; Converters
  File /r ..\Sources\BIN\Release\ConverterPlugins

  ; Japanese lib files
  File /r ..\Sources\BIN\Release\JPN

  ; Japanese Localization folder
  File /r ..\Sources\Localization

  ; Help
  File /oname=BUSMASTER.chm "..\Documents\4 Help_new\out\help.chm"

  ; LDF Editor Help
  File /oname=LDFEditor.chm "..\Documents\4 Help_new\out\ldfeditor.chm"
  
  ; Oxygen icons resource Dll
  File ..\Sources\BIN\Release\AdvancedUIPlugIn.dll

  ; License
  File ..\COPYING.LESSER.txt
  File ..\COPYING.txt
  File ..\FTL.txt

  ; Readme
  File ..\Readme.txt
  
  File /r ..\Sources\BIN\Release\platforms

  ; Simulated Systems Include files
  SetOutPath "$INSTDIR\SimulatedSystems\include\"
  File ..\Sources\Application\SimulatedSystems\include\CANIncludes.h
  File ..\Sources\Application\SimulatedSystems\include\LINIncludes.h
  File ..\Sources\Application\SimulatedSystems\include\CAPLWrapper.h
  File ..\Sources\Application\SimulatedSystems\include\Common.h
  File ..\Sources\Application\SimulatedSystems\include\Wrapper_CAN.h
  File ..\Sources\Application\SimulatedSystems\include\Wrapper_LIN.h
  File ..\Sources\Application\SimulatedSystems\include\Wrapper_J1939.h
  File ..\Sources\Application\SimulatedSystems\include\J1939Includes.h

  ; Simulated Systems Library files
  SetOutPath "$INSTDIR\SimulatedSystems\OBJ\"
  File ..\Sources\Application\SimulatedSystems\OBJ\libWrapper_CAN.a
  File ..\Sources\Application\SimulatedSystems\OBJ\libWrapper_LIN.a
  File ..\Sources\Application\SimulatedSystems\OBJ\libWrapper_J1939.a

  ; GCC Make Files
  SetOutPath $INSTDIR
  File ..\Sources\Application\GCCDLLMakeTemplate_CAN
  File ..\Sources\Application\GCCDLLMakeTemplate_LIN
  File ..\Sources\Application\GCCDLLMakeTemplate_J1939

  ; Check if Visual Studio 2012 redistributable is already installed
  ReadRegStr $1 HKLM "Software\Microsoft\DevDiv\vc\Servicing\11.0\RuntimeMinimum" Install
  StrCmp $1 "1" NoInstall Install

Install:
  ; Install Visual Studio 2012 Redistributable
  File "..\Tools\VC++ 2012 Redistributable\vcredist_x86.exe"
  ExecWait '"vcredist_x86.exe" /s /v" /qn"'
  Delete "$INSTDIR\vcredist_x86.exe"

NoInstall:

  ; create desktop shortcut
  CreateShortCut "$DESKTOP\BUSMASTER v${VERSION}.lnk" "$INSTDIR\BUSMASTER.exe" ""

  ; Registry entries for uninstaller
  WriteRegStr HKLM "Software\BUSMASTER_v${VERSION}" "Install_Dir" "$INSTDIR"
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
  WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe" "WINXPSP3"
  WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\FormatConverter.exe" "WINXPSP3"
  WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe" "RUNASADMIN"

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
  CreateDirectory "$SMPROGRAMS\BUSMASTER v${VERSION}\Tools"
  CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\Tools\LIN Database Editor.lnk" "$INSTDIR\LDFEditor.exe" "" "$INSTDIR\LDFEditor.exe" 0
  CreateShortCut "$SMPROGRAMS\BUSMASTER v${VERSION}\Tools\Format Converter.lnk" "$INSTDIR\FormatConverter.exe" "" "$INSTDIR\FormatConverter.exe" 0

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
  File ..\Sources\BIN\Release\CAN_ETAS_BOA_2.dll
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
  ; File ..\Sources\BIN\Release\canlib32.dll
SectionEnd

Section "MHS-Elektronik Tiny-CAN"
  SectionIn 1 2
  SetOutPath $INSTDIR
  File ..\Sources\BIN\Release\CAN_MHS.dll
  File ..\Sources\CAN_MHS\EXTERNAL\mhstcan.dll
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

; Uninstall section here...
Section "Uninstall"
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
  DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSMASTER.exe"
  DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\BUSEmulation.exe"
  DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\FormatConverter.exe"

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
SectionEnd
