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
 * \file      Utility.cpp
 * \brief     Definiton of the Utility Functions.
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the LogAscConverter class.
 */
#include "stdafx.h"
#include <string.h>

extern "C" int nGetDayFromDate(char* pchDate, char* pchMonth, char* chWeekDay)
{
	if(pchDate == NULL || chWeekDay == NULL || pchMonth == NULL)
	{
		return -1;
	}
	int nDate, nMonth, nYear;
	char chTemp;
	char chDelim[] = ":. ";
	
	char* pchTemp = strtok(pchDate, chDelim);
	nDate = atoi(pchTemp);
	pchTemp = strtok(NULL, chDelim);
	nMonth = atoi(pchTemp);
	pchTemp = strtok(NULL, chDelim);
	nYear = atoi(pchTemp);
	
	/*sscanf(pchDate, "%d%c%d%c%d", &nDate, &chTemp, &nMonth,
		&chTemp, &nYear);*/

	CTime omTime(nYear, nMonth, nDate, 0, 0, 0, 0);
	//Day
	int nDay = omTime.GetDayOfWeek();
	switch(nDay)
	{
		case 1:
			strncpy(chWeekDay, _T("Sun"), sizeof(chWeekDay));
			break;
		case 2:
			strncpy(chWeekDay, _T("Mon"), sizeof(chWeekDay));
			break;
		case 3:
			strncpy(chWeekDay, _T("Tue"), sizeof(chWeekDay));
			break;
		case 4:
			strncpy(chWeekDay, _T("Wed"), sizeof(chWeekDay));
			break;
		case 5:
			strncpy(chWeekDay, _T("Thu"), sizeof(chWeekDay));
			break;
		case 6:
			strncpy(chWeekDay, _T("Fri"), sizeof(chWeekDay));
			break;
		case 7:
			strncpy(chWeekDay, _T("Sat"), sizeof(chWeekDay));
			break;
	}

	//Month
	switch(nMonth)
	{
		case 1:
			strncpy(pchMonth, "Jan", sizeof(chWeekDay));
			break;
		case 2:
			strncpy(pchMonth, "Feb", sizeof(chWeekDay));
			break;
		case 3:
			strncpy(pchMonth, "Mar", sizeof(chWeekDay));
			break;
		case 4:
			strncpy(pchMonth, "Apr", sizeof(chWeekDay));
			break;
		case 5:
			strncpy(pchMonth, "May", sizeof(chWeekDay));
			break;
		case 6:
			strncpy(pchMonth, "Jun", sizeof(chWeekDay));
			break;
		case 7:
			strncpy(pchMonth, "Jul", sizeof(chWeekDay));
			break;
		case 8:
			strncpy(pchMonth, "Aug", sizeof(chWeekDay));
			break;
		case 9:
			strncpy(pchMonth, "Sep", sizeof(chWeekDay));
			break;
		case 10:
			strncpy(pchMonth, "Oct", sizeof(chWeekDay));
			break;
		case 11:
			strncpy(pchMonth, "Nov", sizeof(chWeekDay));
			break;
		case 12:
			strncpy(pchMonth, "Dec", sizeof(chWeekDay));
			break;
	}
	return 0;
}