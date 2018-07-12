#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Hardware selection script

#ce ----------------------------------------------------------------------------

; Script Start - Add your code below here
Sleep(1500)
WinActivate("Hardware Selection")
if winexists("Hardware Selection") Then
	ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
	sleep(1000)
	ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
	sleep(1000)
	ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
	sleep(1000)
Endif
