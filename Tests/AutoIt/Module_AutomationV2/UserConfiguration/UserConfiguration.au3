#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.8.1
 Author:         Anita Nadgir

 Script Function:
	Parent File containing 'CAN Node Simulation' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

_StartoHTMLTable("User Configuration Test Report")

;Run all the NodeSimulationJ1939 module scripts

 #include "TS_UserCfx_01.au3"
 #include "TS_UserCfx_02.au3"
 #include "TS_UserCfx_04.au3"
 #include "TS_UserCfx_05.au3"
 #include "TS_UserCfx_06.au3"
 #include "TS_UserCfx_07.au3"
 #include "TS_UserCfx_08.au3"
;~  ;---------P2 test cases-------------
;~ ;;;~  #include "TS_UserCfx_10.au3"
;~ ;;;~  #include "TS_UserCfx_11.au3"
 #include "TS_UserCfx_13.au3"
 #include "TS_UserCfx_14.au3"
 #include "TS_UserCfx_16.au3"
;~ ;;;~  #include "TS_UserCfx_21.au3"

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)