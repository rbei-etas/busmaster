#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'CAN Message Window' module test scripts.

#ce ----------------------------------------------------------------------------


;Run all the CAN Message Window module scripts

; Create the HTML table to save the result
_StartoHTMLTable("CAN Message Window Module Test Report")

;---------------P1 test cases-----------------------------
#include "TS_MSGWIN_01.au3"
#include "TS_MSGWIN_02.au3"
#include "TS_MSGWIN_03.au3"

;---------------P2 test cases-------------------------------

;"TS_MSGWIN_12" is covered in test case TS_MSGWIN_08 and "TS_MSGWIN_08"
#include "TS_MSGWIN_04.au3"
#include "TS_MSGWIN_05.au3"
#include "TS_MSGWIN_06.au3"
#include "TS_MSGWIN_08.au3"
#include "TS_MSGWIN_010.au3"
#include "TS_MSGWIN_011.au3"
#include "TS_MSGWIN_013.au3"
#include "TS_MSGWIN_014.au3"
#include "TS_MSGWIN_019.au3"
#include "TS_MSGWIN_020.au3"
#include "TS_MSGWIN_021.au3"
#include "TS_MSGWIN_025.au3"
#include "TS_MSGWIN_026.au3"
_EndoHTMLTable()