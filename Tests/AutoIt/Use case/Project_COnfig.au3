Run("C:\Program Files\BUSMASTER\BUSMASTER.exe")
Sleep(3000)

if winexists("BUSMASTER") Then
$sMenu=WinMenuSelectItem("BUSMASTER","","&File","Confi&guration","&Load")								;select menu : file->load
sleep(1000)
	if $sMenu=0 Then
		msgbox(0,"","Test execution Failed...'Load' Menu not found!!!can't proceed further")
		Exit
	EndIf
	if WinWait("BUSMASTER","Configuration File is modified",2) Then
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
	EndIf
sleep(150)
	if winexists("Load Configuration Filename...") Then
		ControlSend("Load Configuration Filename...","","[CLASS:Edit; INSTANCE:1]",@ScriptDir&"\newcfx.cfx")				;load cfx file
		sleep(1000)
		ControlClick("Load Configuration Filename...","Load","[CLASS:Button; INSTANCE:2]","")
		sleep(1000)
			if winexists("Load Configuration Filename...",@ScriptDir&"\abc.cfx") Then									;if the cfx file doesn't exists then create a new one
				ControlClick("Load Configuration Filename...","","[CLASS:Button; INSTANCE:1]","left")
				sleep(1000)
				ControlClick("Load Configuration Filename...","","[CLASS:Button; INSTANCE:3]","left")
				sleep(1000)
				$sMenu=WinMenuSelectItem("BUSMASTER","","&File","Confi&guration","&New")				;select menu : file->new
				sleep(1000)
				if $sMenu=0 Then
					msgbox(0,"","Test execution Failed...'New' Menu not found!!!can't proceed further")
					Exit
				Endif
				if WinWait("","Configuration File is modified",2) Then
					ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
					sleep(1000)
				EndIf
				sleep(500)
				if winexists("New Configuration Filename...") Then
					ControlSend("New Configuration Filename...","","[CLASS:Edit; INSTANCE:1]",@ScriptDir&"\newcfx.cfx")
					sleep(1000)
					$cConfig=ControlClick("New Configuration Filename...","","[CLASS:Button; INSTANCE:2]","left")
					sleep(2000)
					if winexists("Hardware Selection") Then
						ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
						sleep(1000)
						ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
						sleep(1000)
						ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
						sleep(1000)
					EndIf
				EndIf
			EndIf
		EndIf
	send("!fgs")
EndIf