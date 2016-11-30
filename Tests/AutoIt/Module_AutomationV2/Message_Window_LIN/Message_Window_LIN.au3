#cs ----------------------------------------------------------------------------#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Srinibas Das

 Script Function:
	Parent File containing 'LIN Message Window' module test scripts.

#ce ----------------------------------------------------------------------------
ConsoleWrite("include files..")
#include <IncludeFiles.au3>
ConsoleWrite("called Run()..")
_RunMe()

;Run all the CAN Message Window module scripts

; Create the HTML table to save the result
_StartoHTMLTable("LIN Message Window Module Test Report")

;------------------P1 test cases-----------------------------

;#include "TS_LIN_MSGWIN_01.au3"
;----#include "TS_LIN_MSGWIN_02.au3"
;#include "TS_LIN_MSGWIN_04.au3"              ; Menu state has to be checked manually
;#include "TS_LIN_MSGWIN_06.au3"
#include "TS_LIN_MSGWIN_07.au3"
;#include "TS_LIN_MSGWIN_12.au3"				 ; Menu state has to be checked manually
;#include "TS_LIN_MSGWIN_13.au3"              ; Menu state has to be checked manually
;#include "TS_LIN_MSGWIN_16.au3"
;#include "TS_LIN_MSGWIN_17.au3"
;------Covers TS_MSGWIN_J1939_04.au3
;-------#include "TS_MSGWIN_J1939_05.au3"

;--------------P2 test cases---------------------------------

;#include "TS_MSGWIN_J1939_06.au3"
;#include "TS_MSGWIN_J1939_07.au3"



_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)