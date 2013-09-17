/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \brief     Source script for BUSMASTER installer
 * \author    Saravanan K S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 * \date      27/03/2013
 *
 * Source script for DBC 2 DBF Converter installer.
 */

!include "NSISHeaders.nsh"

; Show gradient background
BGGradient 8080C0 0000FF FFFFFF

; Title of this installation
Name "BUSMASTER Patch Installer"

; Do a CRC check when initializing setup
CRCCheck On

; Output filename
Outfile "BUSMASTER_Patch_v${VERSION}.exe"

SectionGroup "Main"
Section "BUSMASTER PATCH"

	ReadRegStr $1 HKLM "SOFTWARE\BUSMASTER_v${VERSION}\" Install_Dir
	
    SetOutPath $1\ConverterPlugins
	
	; Deleting If DBF2DBCConverter.dll exists
	IfFileExists $1\ConverterPlugins\DBF2DBCConverter.dll bAppFileExists
	bAppFileExists:
			Delete "$INSTDIR\ConverterPlugins\DBF2DBCConverter.dll"
			File ..\Sources\BIN\Release\ConverterPlugins\DBF2DBCConverter.dll
			
SectionEnd
SectionGroupEnd