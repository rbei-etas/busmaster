#cs ----------------------------------------------------------------------------#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'CAN Message Window' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

;Run all the CAN Message Window module scripts

; Create the HTML table to save the result
_StartoHTMLTable("J1939 Message Window Module Test Report")

;------------------P1 test cases-----------------------------

#include "TS_MSGWIN_J1939_03.au3"    ;Covers TS_MSGWIN_J1939_04.au3
#include "TS_MSGWIN_J1939_05.au3"

;--------------P2 test cases---------------------------------

#include "TS_MSGWIN_J1939_06.au3"
#include "TS_MSGWIN_J1939_07.au3"




_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)