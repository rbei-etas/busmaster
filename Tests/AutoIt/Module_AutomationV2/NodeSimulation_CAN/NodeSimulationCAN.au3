#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.8.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'CAN Node Simulation' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

;~ ; Copy below Node simulation files from installed Busmaster folder to the script directory to avoid build,load errors
FileCopy ( $BusMasterExeFPath & "\GCCDLLMakeTemplate_CAN", @ScriptDir, 1)
DirCreate( @ScriptDir & "\SimulatedSystems")
DirCopy ( $BusMasterExeFPath & "\SimulatedSystems", @ScriptDir& "\SimulatedSystems", 1)

_RunMe()

_StartoHTMLTable("CAN Node Simulation Test Report")

;Run all the NodeSimulationCAN module scripts

#include "TS_NS_01.au3"
#include "TS_NS_02.au3"
#include "TS_NS_03.au3"
#include "TS_NS_04.au3"
; TS_NS_05 tc included in TS_NS_03 so deleted
; TS_NS_06 tc deleted
; TS_NS_07-TS_NS_11 tc's are error handler case's which cant be tested without hardware
#include "TS_NS_12.au3"
#include "TS_NS_14.au3"
#include "TS_NS_129.au3"
#include "TS_NS_139.au3"
; TS_NS_15 tc included in TS_NS_03 so deleted
_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)