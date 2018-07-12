
;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_07
; TestCases			:		Saving Node simulation detiails
; Test Data			:		cfx_UserCfx_07.cfx
; Test Setup		:		1. Add a simulated system to simulation.
;                           2. Add a node.
;                           3  Save the configuration.
;                           4. Close and open the Busmaster.
;                           5. Check the node simulation details
; Expected Results  : 		1. The node simulation will show the details of all nodes saved in step3
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_07.au3****"&@CRLF)
Local $path=_TestDataPath()
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_07.cfx")

		 WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNodeSimMenu,$CANNodeSimConfigMenu)
		 sleep(2000)
		 $Handle=ControlGetHandle($WIN_BUSMASTER,"",$TVC_NodeSim_ConfigCANNodeSim)                 ;Get the handle of the control for tree view
		  ConsoleWrite("$Handle" & $Handle & @CRLF)
		  $ParentHandle=_GUICtrlTreeView_GetItemHandle($Handle)                                      ;Get handle for parent node


		  $ParentText=_GUICtrlTreeView_GetText($Handle,$ParentHandle)

		  $nextItem=_GUICtrlTreeView_GetNext($Handle,$ParentHandle)                                 ;Get handle of its child item

		  _GUICtrlTreeView_ClickItem($Handle,$nextItem)
		  $nextText=_GUICtrlTreeView_GetText($Handle,$nextItem)
		  ConsoleWrite("$nextText" & $nextText & @CRLF)
		  $ChildHandle=_GUICtrlTreeView_GetNext($Handle,$nextItem)                                   ;Get the handle of next child item

		   $ChildItemText=_GUICtrlTreeView_GetText($Handle,$ChildHandle)
		   ConsoleWrite("$ChildItemText" & $ChildItemText & @CRLF)

		    _GUICtrlTreeView_ClickItem($Handle,$nextItem)
		  _GUICtrlTreeView_Expand($Handle,$nextItem)
		   send("^s")
		   send("^{F4}")
		   sleep(1000)
		   WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								; Select File->Exit from menu to close Application
	       sleep(1000)
	      if WinWaitActive("",$SaveChangesConfigTXT,2) Then									; wait for save configuration dialog
		 ControlClick("",$SaveChangesConfigTXT,$BTN_Yes_SaveConfig)						; Click on Yes button
	    EndIf

		 _launchApp()
		 sleep(500)
          WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNodeSimMenu,$CANNodeSimConfigMenu)
		   sleep(2000)
		 $Handle=ControlGetHandle($WIN_BUSMASTER,"",$TVC_NodeSim_ConfigCANNodeSim)                 ;Get the handle of the control for tree view

		  $ParentHandle=_GUICtrlTreeView_GetItemHandle($Handle)                                      ;Get handle for parent node

		  $ParentText=_GUICtrlTreeView_GetText($Handle,$ParentHandle)
		  ConsoleWrite("$ParentText" & $ParentText & @CRLF)
		  $nextItem=_GUICtrlTreeView_GetNext($Handle,$ParentHandle)                                 ;Get handle of its child item

		     _GUICtrlTreeView_ClickItem($Handle,$nextItem)
		  _GUICtrlTreeView_Expand($Handle,$nextItem)
		  $nextText=_GUICtrlTreeView_GetText($Handle,$nextItem)
		  ConsoleWrite("$nextText" & $nextText & @CRLF)
		  $ChildHandle=_GUICtrlTreeView_GetNext($Handle,$nextItem)                                   ;Get the handle of next child item

		   $ChildItemText=_GUICtrlTreeView_GetText($Handle,$ChildHandle)
		   ConsoleWrite("$ChildItemText" & $ChildItemText & @CRLF)


          If($nextText=$path&"\TestUserConfig.sim") Then
			_WriteResult("Pass","TS_UserCfx_07")
		Else
			_WriteResult("Fail","TS_UserCfx_07")
		EndIf
    EndIf


	$isAppNotRes=_CloseApp()																			; Close the app

	if $isAppNotRes=1 Then
         _WriteResult("Warning","TS_UserCfx_07")

	   EndIf
