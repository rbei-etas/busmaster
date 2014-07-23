#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'CAN Signal Watch' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

;Run all the CAN Signal Watch module scripts

; Create the HTML table to save the result
_StartoHTMLTable("CAN Signal Watch Module Test Report")

;----------------P1 test cases-------------------------

#include "TS_CANSigWatch_01.au3"

;-----------------P2 test cases----------------------------
 #include "TS_CANSigWatch_02.au3"
 #include "TS_CANSigWatch_03.au3"
 #include "TS_CANSigWatch_04.au3"
 #include "TS_CANSigWatch_05.au3"
#include "TS_CANSigWatch_06.au3"
#include "TS_CANSigWatch_07.au3"
#include "TS_CANSigWatch_11.au3"

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)