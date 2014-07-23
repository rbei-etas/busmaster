#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.8.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'CAN Node Simulation' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

_StartoHTMLTable("J1939 Functionality Test Report")

;Run all the NodeSimulationJ1939 module scripts

#include "TS_J1939_Func_001.au3"
#include "TS_J1939_Func_002.au3"

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)