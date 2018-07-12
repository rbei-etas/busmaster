#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.8.1
 Author:         Srinibas Das

 Script Function:
	Parent File containing 'Lin Cluster Configuration' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>


_RunMe()

_StartoHTMLTable("LIN Node Simulation Test Report")

;Run all the Lin Cluster Configuration module scripts

;-----P1 test Cases-----------------------------
#include "TS_LIN_CC_01.au3"
#include "TS_LIN_CC_02.au3"
#include "TS_LIN_CC_03.au3"


_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)