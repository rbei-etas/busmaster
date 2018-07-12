#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.8.1
 Author:         Srinibas Das

 Script Function:
	Parent File containing 'LIN Node Simulation' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

;~ ; Copy below Node simulation files from installed Busmaster folder to the script directory to avoid build,load errors
FileCopy ( $BusMasterExeFPath & "\GCCDLLMakeTemplate_LIN", @ScriptDir, 1)
DirCreate( @ScriptDir & "\SimulatedSystems")
DirCopy ( $BusMasterExeFPath & "\SimulatedSystems", @ScriptDir& "\SimulatedSystems", 1)

_RunMe()

_StartoHTMLTable("LIN Node Simulation Test Report")

;Run all the NodeSimulationCAN module scripts
;If(@OSVersion = "WIN_7") Then
;-----P1 test Cases-----------------------------
;#include "TS_NS_LIN_02.au3"
#include "TS_NS_LIN_05.au3"
#include "TS_NS_LIN_06.au3"
#include "TS_NS_LIN_08.au3"
#include "TS_NS_LIN_10.au3"
#include "TS_NS_LIN_11.au3"
#include "TS_NS_LIN_14.au3"
#include "TS_NS_LIN_17.au3"
#include "TS_NS_LIN_19.au3"
#include "TS_NS_LIN_23.au3"
;~ #include "TS_NS_LIN_30.au3" -  Covered in "TS_NS_LIN_31.au3"
;#include "TS_NS_LIN_31.au3"
;#include "TS_NS_LIN_36.au3"
#include "TS_NS_LIN_40.au3"
#include "TS_NS_LIN_46.au3"
;#include "TS_NS_LIN_48.au3"; -- Application crashed
;TS_NS_LIN_51 = Need to execute mannualy
;TS_NS_LIN_52 = Need to execute mannualy
;#include "TS_NS_LIN_63.au3"-Issue
;#include "TS_NS_LIN_64.au3"-Issue
#include "TS_NS_LIN_73.au3"
#include "TS_NS_LIN_79.au3"
#include "TS_NS_LIN_81.au3"
#include "TS_NS_LIN_88.au3"
#include "TS_NS_LIN_97.au3"
#include "TS_NS_LIN_31.au3"
;#include "TS_NS_LIN_97.au3"
;-----------------------------------------------


;#include "TS_NS_03.au3"
;#include "TS_NS_04.au3"
; TS_NS_05 tc included in TS_NS_03 so deleted
;#include "TS_NS_LIN_05.au3"
; TS_NS_06 tc deleted
; TS_NS_07-TS_NS_11 tc's are error handler case's which cant be tested without hardware
;#include "TS_NS_12.au3"
;#include "TS_NS_14.au3"
;#include "TS_NS_129.au3"
;#include "TS_NS_139.au3"
; TS_NS_15 tc included in TS_NS_03 so deleted
;EndIf

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)