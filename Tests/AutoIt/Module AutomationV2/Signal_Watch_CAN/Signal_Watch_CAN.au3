#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'CAN Signal Watch' module test scripts.

#ce ----------------------------------------------------------------------------


;Run all the CAN Signal Watch module scripts

; Create the HTML table to save the result
_StartoHTMLTable("CAN Signal Watch Module Test Report")

;#include "TS_CANSigWatch_01.au3"
;~ "TS_CANSigWatch_02" is a P2 case
 #include "TS_CANSigWatch_03.au3"
 #include "TS_CANSigWatch_04.au3"
 #include "TS_CANSigWatch_05.au3"
 ;  #include "TS_CANSigWatch_02.au3"
    #include "TS_CANSigWatch_06.au3"
	  #include "TS_CANSigWatch_07.au3"
	#include "TS_CANSigWatch_11.au3"

_EndoHTMLTable()