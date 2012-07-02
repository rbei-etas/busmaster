; Title of this installation
Name "BUSMASTER Registry Cleanup"

; Do a CRC check when initializing setup
CRCCheck On

; Output filename
Outfile "BUSMASTER_Cleanup_Registry.exe"

SectionGroup "Main"

Section "BUSMASTER"
	IfFileExists Software\RBIN\BUSMASTER\Files\ConfigFile bRegEntryExists
	 bRegEntryExists:
	 WriteRegStr HKCU "Software\RBIN\BUSMASTER\Files" "ConfigFile" ""
	 
	IfFileExists Software\RBIN\BUSMASTER bRegEntryMruExists
	 bRegEntryMruExists:
	 DeleteRegKey HKCU "Software\RBIN\BUSMASTER\MRU List"
	
SectionEnd
SectionGroupEnd