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
 * \brief     Source script for BUSMASTER Clean Registry
 * \author    Saravanan K S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 * \date      27/03/2013
 *
 * Source script for BUSMASTER Clean Registry
 */
 
!include "NSISHeaders.nsh"

RequestExecutionLevel user

; Title of this installation
Name "BUSMASTER Registry Cleanup"

; Do a CRC check when initializing setup
CRCCheck On

; Output filename
Outfile "BUSMASTER_Cleanup_Registry.exe"

SectionGroup "Main"

Section "BUSMASTER"
	IfFileExists Software\RBEI-ETAS\BUSMASTER_v${VERSION} bRegEntryMruExists
	 bRegEntryMruExists:
	 DeleteRegKey HKCU "Software\RBEI-ETAS\BUSMASTER_v${VERSION}\BUSMASTER"
	 DeleteRegKey HKCU "Software\RBEI-ETAS\BUSMASTER_v${VERSION}\Files"
	 DeleteRegKey HKCU "Software\RBEI-ETAS\BUSMASTER_v${VERSION}\MainWnd"	
SectionEnd
SectionGroupEnd