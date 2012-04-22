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
 * \file      CExcApplication.h
 * \brief     This file contain the declaration of CExcApplication wrapper class
 * \author    Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the declaration of CExcApplication wrapper class
 */

class CExcApplication : public COleDispatchDriver
{
public:
	CExcApplication(){} // Calls COleDispatchDriver default constructor
	CExcApplication(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CExcApplication(const CExcApplication& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _Application methods
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ActiveCell()
	{
		LPDISPATCH result;
		InvokeHelper(0x131, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ActiveChart()
	{
		LPDISPATCH result;
		InvokeHelper(0xb7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ActiveDialog()
	{
		LPDISPATCH result;
		InvokeHelper(0x32f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ActiveMenuBar()
	{
		LPDISPATCH result;
		InvokeHelper(0x2f6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_ActivePrinter()
	{
		CString result;
		InvokeHelper(0x132, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ActivePrinter(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x132, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_ActiveSheet()
	{
		LPDISPATCH result;
		InvokeHelper(0x133, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ActiveWindow()
	{
		LPDISPATCH result;
		InvokeHelper(0x2f7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ActiveWorkbook()
	{
		LPDISPATCH result;
		InvokeHelper(0x134, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_AddIns()
	{
		LPDISPATCH result;
		InvokeHelper(0x225, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Assistant()
	{
		LPDISPATCH result;
		InvokeHelper(0x59e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Calculate()
	{
		InvokeHelper(0x117, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_Cells()
	{
		LPDISPATCH result;
		InvokeHelper(0xee, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Charts()
	{
		LPDISPATCH result;
		InvokeHelper(0x79, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Columns()
	{
		LPDISPATCH result;
		InvokeHelper(0xf1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_CommandBars()
	{
		LPDISPATCH result;
		InvokeHelper(0x59f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_DDEAppReturnCode()
	{
		long result;
		InvokeHelper(0x14c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void DDEExecute(long Channel, LPCTSTR String)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x14d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Channel, String);
	}
	long DDEInitiate(LPCTSTR App, LPCTSTR Topic)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x14e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, App, Topic);
		return result;
	}
	void DDEPoke(long Channel, VARIANT Item, VARIANT Data)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x14f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Channel, &Item, &Data);
	}
	VARIANT DDERequest(long Channel, LPCTSTR Item)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x150, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Channel, Item);
		return result;
	}
	void DDETerminate(long Channel)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x151, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Channel);
	}
	LPDISPATCH get_DialogSheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x2fc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Evaluate(VARIANT Name)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Name);
		return result;
	}
	VARIANT _Evaluate(VARIANT Name)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xfffffffb, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Name);
		return result;
	}
	VARIANT ExecuteExcel4Macro(LPCTSTR String)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x15e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, String);
		return result;
	}
	LPDISPATCH Intersect(LPDISPATCH Arg1, LPDISPATCH Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8, VARIANT Arg9, VARIANT Arg10, VARIANT Arg11, VARIANT Arg12, VARIANT Arg13, VARIANT Arg14, VARIANT Arg15, VARIANT Arg16, VARIANT Arg17, VARIANT Arg18, VARIANT Arg19, VARIANT Arg20, VARIANT Arg21, VARIANT Arg22, VARIANT Arg23, VARIANT Arg24, VARIANT Arg25, VARIANT Arg26, VARIANT Arg27, VARIANT Arg28, VARIANT Arg29, VARIANT Arg30)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x2fe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Arg1, Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
		return result;
	}
	LPDISPATCH get_MenuBars()
	{
		LPDISPATCH result;
		InvokeHelper(0x24d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Modules()
	{
		LPDISPATCH result;
		InvokeHelper(0x246, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Names()
	{
		LPDISPATCH result;
		InvokeHelper(0x1ba, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Range(VARIANT Cell1, VARIANT Cell2)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xc5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Cell1, &Cell2);
		return result;
	}
	LPDISPATCH get_Rows()
	{
		LPDISPATCH result;
		InvokeHelper(0x102, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Run(VARIANT Macro, VARIANT Arg1, VARIANT Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8, VARIANT Arg9, VARIANT Arg10, VARIANT Arg11, VARIANT Arg12, VARIANT Arg13, VARIANT Arg14, VARIANT Arg15, VARIANT Arg16, VARIANT Arg17, VARIANT Arg18, VARIANT Arg19, VARIANT Arg20, VARIANT Arg21, VARIANT Arg22, VARIANT Arg23, VARIANT Arg24, VARIANT Arg25, VARIANT Arg26, VARIANT Arg27, VARIANT Arg28, VARIANT Arg29, VARIANT Arg30)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x103, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Macro, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
		return result;
	}
	VARIANT _Run2(VARIANT Macro, VARIANT Arg1, VARIANT Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8, VARIANT Arg9, VARIANT Arg10, VARIANT Arg11, VARIANT Arg12, VARIANT Arg13, VARIANT Arg14, VARIANT Arg15, VARIANT Arg16, VARIANT Arg17, VARIANT Arg18, VARIANT Arg19, VARIANT Arg20, VARIANT Arg21, VARIANT Arg22, VARIANT Arg23, VARIANT Arg24, VARIANT Arg25, VARIANT Arg26, VARIANT Arg27, VARIANT Arg28, VARIANT Arg29, VARIANT Arg30)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x326, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Macro, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
		return result;
	}
	LPDISPATCH get_Selection()
	{
		LPDISPATCH result;
		InvokeHelper(0x93, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void SendKeys(VARIANT Keys, VARIANT Wait)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x17f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Keys, &Wait);
	}
	LPDISPATCH get_Sheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x1e5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ShortcutMenus(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x308, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH get_ThisWorkbook()
	{
		LPDISPATCH result;
		InvokeHelper(0x30a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Toolbars()
	{
		LPDISPATCH result;
		InvokeHelper(0x228, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Union(LPDISPATCH Arg1, LPDISPATCH Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8, VARIANT Arg9, VARIANT Arg10, VARIANT Arg11, VARIANT Arg12, VARIANT Arg13, VARIANT Arg14, VARIANT Arg15, VARIANT Arg16, VARIANT Arg17, VARIANT Arg18, VARIANT Arg19, VARIANT Arg20, VARIANT Arg21, VARIANT Arg22, VARIANT Arg23, VARIANT Arg24, VARIANT Arg25, VARIANT Arg26, VARIANT Arg27, VARIANT Arg28, VARIANT Arg29, VARIANT Arg30)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x30b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Arg1, Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
		return result;
	}
	LPDISPATCH get_Windows()
	{
		LPDISPATCH result;
		InvokeHelper(0x1ae, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Workbooks()
	{
		LPDISPATCH result;
		InvokeHelper(0x23c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_WorksheetFunction()
	{
		LPDISPATCH result;
		InvokeHelper(0x5a0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Worksheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x1ee, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Excel4IntlMacroSheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x245, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Excel4MacroSheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x243, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void ActivateMicrosoftApp(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x447, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void AddChartAutoFormat(VARIANT Chart, LPCTSTR Name, VARIANT Description)
	{
		static BYTE parms[] = VTS_VARIANT VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0xd8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Chart, Name, &Description);
	}
	void AddCustomList(VARIANT ListArray, VARIANT ByRow)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x30c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &ListArray, &ByRow);
	}
	BOOL get_AlertBeforeOverwriting()
	{
		BOOL result;
		InvokeHelper(0x3a2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AlertBeforeOverwriting(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3a2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_AltStartupPath()
	{
		CString result;
		InvokeHelper(0x139, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_AltStartupPath(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x139, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AskToUpdateLinks()
	{
		BOOL result;
		InvokeHelper(0x3e0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AskToUpdateLinks(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3e0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_EnableAnimations()
	{
		BOOL result;
		InvokeHelper(0x49c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EnableAnimations(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x49c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_AutoCorrect()
	{
		LPDISPATCH result;
		InvokeHelper(0x479, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Build()
	{
		long result;
		InvokeHelper(0x13a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_CalculateBeforeSave()
	{
		BOOL result;
		InvokeHelper(0x13b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_CalculateBeforeSave(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x13b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Calculation()
	{
		long result;
		InvokeHelper(0x13c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Calculation(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x13c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_Caller(VARIANT Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x13d, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &Index);
		return result;
	}
	BOOL get_CanPlaySounds()
	{
		BOOL result;
		InvokeHelper(0x13e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_CanRecordSounds()
	{
		BOOL result;
		InvokeHelper(0x13f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x8b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Caption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x8b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_CellDragAndDrop()
	{
		BOOL result;
		InvokeHelper(0x140, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_CellDragAndDrop(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x140, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double CentimetersToPoints(double Centimeters)
	{
		double result;
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x43e, DISPATCH_METHOD, VT_R8, (void*)&result, parms, Centimeters);
		return result;
	}
	BOOL CheckSpelling(LPCTSTR Word, VARIANT CustomDictionary, VARIANT IgnoreUppercase)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1f9, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Word, &CustomDictionary, &IgnoreUppercase);
		return result;
	}
	VARIANT get_ClipboardFormats(VARIANT Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x141, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &Index);
		return result;
	}
	BOOL get_DisplayClipboardWindow()
	{
		BOOL result;
		InvokeHelper(0x142, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayClipboardWindow(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x142, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ColorButtons()
	{
		BOOL result;
		InvokeHelper(0x16d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ColorButtons(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x16d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CommandUnderlines()
	{
		long result;
		InvokeHelper(0x143, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CommandUnderlines(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x143, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ConstrainNumeric()
	{
		BOOL result;
		InvokeHelper(0x144, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ConstrainNumeric(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x144, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT ConvertFormula(VARIANT Formula, long FromReferenceStyle, VARIANT ToReferenceStyle, VARIANT ToAbsolute, VARIANT RelativeTo)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x145, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Formula, FromReferenceStyle, &ToReferenceStyle, &ToAbsolute, &RelativeTo);
		return result;
	}
	BOOL get_CopyObjectsWithCells()
	{
		BOOL result;
		InvokeHelper(0x3df, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_CopyObjectsWithCells(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3df, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Cursor()
	{
		long result;
		InvokeHelper(0x489, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Cursor(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x489, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CustomListCount()
	{
		long result;
		InvokeHelper(0x313, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_CutCopyMode()
	{
		long result;
		InvokeHelper(0x14a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CutCopyMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x14a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DataEntryMode()
	{
		long result;
		InvokeHelper(0x14b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DataEntryMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x14b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT Dummy1(VARIANT Arg1, VARIANT Arg2, VARIANT Arg3, VARIANT Arg4)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6f6, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Arg1, &Arg2, &Arg3, &Arg4);
		return result;
	}
	VARIANT Dummy2(VARIANT Arg1, VARIANT Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6f7, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8);
		return result;
	}
	VARIANT Dummy3()
	{
		VARIANT result;
		InvokeHelper(0x6f8, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Dummy4(VARIANT Arg1, VARIANT Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8, VARIANT Arg9, VARIANT Arg10, VARIANT Arg11, VARIANT Arg12, VARIANT Arg13, VARIANT Arg14, VARIANT Arg15)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6f9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15);
		return result;
	}
	VARIANT Dummy5(VARIANT Arg1, VARIANT Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8, VARIANT Arg9, VARIANT Arg10, VARIANT Arg11, VARIANT Arg12, VARIANT Arg13)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6fa, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13);
		return result;
	}
	VARIANT Dummy6()
	{
		VARIANT result;
		InvokeHelper(0x6fb, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Dummy7()
	{
		VARIANT result;
		InvokeHelper(0x6fc, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Dummy8(VARIANT Arg1)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6fd, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Arg1);
		return result;
	}
	VARIANT Dummy9()
	{
		VARIANT result;
		InvokeHelper(0x6fe, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	BOOL Dummy10(VARIANT arg)
	{
		BOOL result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6ff, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, &arg);
		return result;
	}
	void Dummy11()
	{
		InvokeHelper(0x700, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString get__Default()
	{
		CString result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_DefaultFilePath()
	{
		CString result;
		InvokeHelper(0x40e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_DefaultFilePath(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x40e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void DeleteChartAutoFormat(LPCTSTR Name)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name);
	}
	void DeleteCustomList(long ListNum)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x30f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ListNum);
	}
	LPDISPATCH get_Dialogs()
	{
		LPDISPATCH result;
		InvokeHelper(0x2f9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_DisplayAlerts()
	{
		BOOL result;
		InvokeHelper(0x157, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayAlerts(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x157, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayFormulaBar()
	{
		BOOL result;
		InvokeHelper(0x158, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayFormulaBar(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x158, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayFullScreen()
	{
		BOOL result;
		InvokeHelper(0x425, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayFullScreen(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x425, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayNoteIndicator()
	{
		BOOL result;
		InvokeHelper(0x159, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayNoteIndicator(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x159, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DisplayCommentIndicator()
	{
		long result;
		InvokeHelper(0x4ac, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DisplayCommentIndicator(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4ac, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayExcel4Menus()
	{
		BOOL result;
		InvokeHelper(0x39f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayExcel4Menus(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x39f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayRecentFiles()
	{
		BOOL result;
		InvokeHelper(0x39e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayRecentFiles(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x39e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayScrollBars()
	{
		BOOL result;
		InvokeHelper(0x15a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayScrollBars(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x15a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayStatusBar()
	{
		BOOL result;
		InvokeHelper(0x15b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayStatusBar(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x15b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void DoubleClick()
	{
		InvokeHelper(0x15d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_EditDirectlyInCell()
	{
		BOOL result;
		InvokeHelper(0x3a1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EditDirectlyInCell(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3a1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_EnableAutoComplete()
	{
		BOOL result;
		InvokeHelper(0x49b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EnableAutoComplete(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x49b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EnableCancelKey()
	{
		long result;
		InvokeHelper(0x448, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EnableCancelKey(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x448, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_EnableSound()
	{
		BOOL result;
		InvokeHelper(0x4ad, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EnableSound(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4ad, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_EnableTipWizard()
	{
		BOOL result;
		InvokeHelper(0x428, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EnableTipWizard(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x428, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_FileConverters(VARIANT Index1, VARIANT Index2)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x3a3, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &Index1, &Index2);
		return result;
	}
	LPDISPATCH get_FileSearch()
	{
		LPDISPATCH result;
		InvokeHelper(0x4b0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_FileFind()
	{
		LPDISPATCH result;
		InvokeHelper(0x4b1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void _FindFile()
	{
		InvokeHelper(0x42c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_FixedDecimal()
	{
		BOOL result;
		InvokeHelper(0x15f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_FixedDecimal(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x15f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FixedDecimalPlaces()
	{
		long result;
		InvokeHelper(0x160, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FixedDecimalPlaces(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x160, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT GetCustomListContents(long ListNum)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x312, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, ListNum);
		return result;
	}
	long GetCustomListNum(VARIANT ListArray)
	{
		long result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x311, DISPATCH_METHOD, VT_I4, (void*)&result, parms, &ListArray);
		return result;
	}
	VARIANT GetOpenFilename(VARIANT FileFilter, VARIANT FilterIndex, VARIANT Title, VARIANT ButtonText, VARIANT MultiSelect)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x433, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &FileFilter, &FilterIndex, &Title, &ButtonText, &MultiSelect);
		return result;
	}
	VARIANT GetSaveAsFilename(VARIANT InitialFilename, VARIANT FileFilter, VARIANT FilterIndex, VARIANT Title, VARIANT ButtonText)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x434, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &InitialFilename, &FileFilter, &FilterIndex, &Title, &ButtonText);
		return result;
	}
	void Goto(VARIANT Reference, VARIANT Scroll)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1db, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Reference, &Scroll);
	}
	double get_Height()
	{
		double result;
		InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Height(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Help(VARIANT HelpFile, VARIANT HelpContextID)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x162, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &HelpFile, &HelpContextID);
	}
	BOOL get_IgnoreRemoteRequests()
	{
		BOOL result;
		InvokeHelper(0x164, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IgnoreRemoteRequests(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x164, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double InchesToPoints(double Inches)
	{
		double result;
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x43f, DISPATCH_METHOD, VT_R8, (void*)&result, parms, Inches);
		return result;
	}
	VARIANT InputBox(LPCTSTR Prompt, VARIANT Title, VARIANT Default, VARIANT Left, VARIANT Top, VARIANT HelpFile, VARIANT HelpContextID, VARIANT Type)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x165, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Prompt, &Title, &Default, &Left, &Top, &HelpFile, &HelpContextID, &Type);
		return result;
	}
	BOOL get_Interactive()
	{
		BOOL result;
		InvokeHelper(0x169, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Interactive(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x169, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_International(VARIANT Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x16a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &Index);
		return result;
	}
	BOOL get_Iteration()
	{
		BOOL result;
		InvokeHelper(0x16b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Iteration(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x16b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_LargeButtons()
	{
		BOOL result;
		InvokeHelper(0x16c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_LargeButtons(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x16c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Left()
	{
		double result;
		InvokeHelper(0x7f, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Left(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_LibraryPath()
	{
		CString result;
		InvokeHelper(0x16e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void MacroOptions(VARIANT Macro, VARIANT Description, VARIANT HasMenu, VARIANT MenuText, VARIANT HasShortcutKey, VARIANT ShortcutKey, VARIANT Category, VARIANT StatusBar, VARIANT HelpContextID, VARIANT HelpFile)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x46f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Macro, &Description, &HasMenu, &MenuText, &HasShortcutKey, &ShortcutKey, &Category, &StatusBar, &HelpContextID, &HelpFile);
	}
	void MailLogoff()
	{
		InvokeHelper(0x3b1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void MailLogon(VARIANT Name, VARIANT Password, VARIANT DownloadNewMail)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x3af, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Name, &Password, &DownloadNewMail);
	}
	VARIANT get_MailSession()
	{
		VARIANT result;
		InvokeHelper(0x3ae, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_MailSystem()
	{
		long result;
		InvokeHelper(0x3cb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_MathCoprocessorAvailable()
	{
		BOOL result;
		InvokeHelper(0x16f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	double get_MaxChange()
	{
		double result;
		InvokeHelper(0x170, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_MaxChange(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x170, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MaxIterations()
	{
		long result;
		InvokeHelper(0x171, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MaxIterations(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x171, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MemoryFree()
	{
		long result;
		InvokeHelper(0x172, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_MemoryTotal()
	{
		long result;
		InvokeHelper(0x173, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_MemoryUsed()
	{
		long result;
		InvokeHelper(0x174, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_MouseAvailable()
	{
		BOOL result;
		InvokeHelper(0x175, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_MoveAfterReturn()
	{
		BOOL result;
		InvokeHelper(0x176, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MoveAfterReturn(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x176, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MoveAfterReturnDirection()
	{
		long result;
		InvokeHelper(0x478, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MoveAfterReturnDirection(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x478, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_RecentFiles()
	{
		LPDISPATCH result;
		InvokeHelper(0x4b2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH NextLetter()
	{
		LPDISPATCH result;
		InvokeHelper(0x3cc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_NetworkTemplatesPath()
	{
		CString result;
		InvokeHelper(0x184, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ODBCErrors()
	{
		LPDISPATCH result;
		InvokeHelper(0x4b3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_ODBCTimeout()
	{
		long result;
		InvokeHelper(0x4b4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ODBCTimeout(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4b4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_OnCalculate()
	{
		CString result;
		InvokeHelper(0x271, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnCalculate(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x271, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_OnData()
	{
		CString result;
		InvokeHelper(0x275, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnData(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x275, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_OnDoubleClick()
	{
		CString result;
		InvokeHelper(0x274, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnDoubleClick(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x274, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_OnEntry()
	{
		CString result;
		InvokeHelper(0x273, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnEntry(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x273, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void OnKey(LPCTSTR Key, VARIANT Procedure)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0x272, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Key, &Procedure);
	}
	void OnRepeat(LPCTSTR Text, LPCTSTR Procedure)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x301, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Text, Procedure);
	}
	CString get_OnSheetActivate()
	{
		CString result;
		InvokeHelper(0x407, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnSheetActivate(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x407, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_OnSheetDeactivate()
	{
		CString result;
		InvokeHelper(0x439, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnSheetDeactivate(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x439, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void OnTime(VARIANT EarliestTime, LPCTSTR Procedure, VARIANT LatestTime, VARIANT Schedule)
	{
		static BYTE parms[] = VTS_VARIANT VTS_BSTR VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x270, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &EarliestTime, Procedure, &LatestTime, &Schedule);
	}
	void OnUndo(LPCTSTR Text, LPCTSTR Procedure)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x302, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Text, Procedure);
	}
	CString get_OnWindow()
	{
		CString result;
		InvokeHelper(0x26f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnWindow(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x26f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_OperatingSystem()
	{
		CString result;
		InvokeHelper(0x177, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_OrganizationName()
	{
		CString result;
		InvokeHelper(0x178, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Path()
	{
		CString result;
		InvokeHelper(0x123, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_PathSeparator()
	{
		CString result;
		InvokeHelper(0x179, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	VARIANT get_PreviousSelections(VARIANT Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x17a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &Index);
		return result;
	}
	BOOL get_PivotTableSelection()
	{
		BOOL result;
		InvokeHelper(0x4b5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_PivotTableSelection(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4b5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_PromptForSummaryInfo()
	{
		BOOL result;
		InvokeHelper(0x426, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_PromptForSummaryInfo(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x426, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Quit()
	{
		InvokeHelper(0x12e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RecordMacro(VARIANT BasicCode, VARIANT XlmCode)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x305, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &BasicCode, &XlmCode);
	}
	BOOL get_RecordRelative()
	{
		BOOL result;
		InvokeHelper(0x17b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_ReferenceStyle()
	{
		long result;
		InvokeHelper(0x17c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ReferenceStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x17c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_RegisteredFunctions(VARIANT Index1, VARIANT Index2)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x307, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &Index1, &Index2);
		return result;
	}
	BOOL RegisterXLL(LPCTSTR Filename)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Filename);
		return result;
	}
	void Repeat()
	{
		InvokeHelper(0x12d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ResetTipWizard()
	{
		InvokeHelper(0x3a0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_RollZoom()
	{
		BOOL result;
		InvokeHelper(0x4b6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_RollZoom(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4b6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Save(VARIANT Filename)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x11b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Filename);
	}
	void SaveWorkspace(VARIANT Filename)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xd4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Filename);
	}
	BOOL get_ScreenUpdating()
	{
		BOOL result;
		InvokeHelper(0x17e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ScreenUpdating(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x17e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void SetDefaultChart(VARIANT FormatName, VARIANT Gallery)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xdb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &FormatName, &Gallery);
	}
	long get_SheetsInNewWorkbook()
	{
		long result;
		InvokeHelper(0x3e1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SheetsInNewWorkbook(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3e1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowChartTipNames()
	{
		BOOL result;
		InvokeHelper(0x4b7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowChartTipNames(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4b7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowChartTipValues()
	{
		BOOL result;
		InvokeHelper(0x4b8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowChartTipValues(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4b8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_StandardFont()
	{
		CString result;
		InvokeHelper(0x39c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_StandardFont(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x39c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_StandardFontSize()
	{
		double result;
		InvokeHelper(0x39d, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_StandardFontSize(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x39d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_StartupPath()
	{
		CString result;
		InvokeHelper(0x181, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	VARIANT get_StatusBar()
	{
		VARIANT result;
		InvokeHelper(0x182, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_StatusBar(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x182, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	CString get_TemplatesPath()
	{
		CString result;
		InvokeHelper(0x17d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_ShowToolTips()
	{
		BOOL result;
		InvokeHelper(0x183, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowToolTips(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x183, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Top()
	{
		double result;
		InvokeHelper(0x7e, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Top(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DefaultSaveFormat()
	{
		long result;
		InvokeHelper(0x4b9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DefaultSaveFormat(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4b9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_TransitionMenuKey()
	{
		CString result;
		InvokeHelper(0x136, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_TransitionMenuKey(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x136, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_TransitionMenuKeyAction()
	{
		long result;
		InvokeHelper(0x137, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TransitionMenuKeyAction(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x137, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_TransitionNavigKeys()
	{
		BOOL result;
		InvokeHelper(0x138, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_TransitionNavigKeys(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x138, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Undo()
	{
		InvokeHelper(0x12f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	double get_UsableHeight()
	{
		double result;
		InvokeHelper(0x185, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	double get_UsableWidth()
	{
		double result;
		InvokeHelper(0x186, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	BOOL get_UserControl()
	{
		BOOL result;
		InvokeHelper(0x4ba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_UserControl(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4ba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_UserName()
	{
		CString result;
		InvokeHelper(0x187, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_UserName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x187, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Value()
	{
		CString result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_VBE()
	{
		LPDISPATCH result;
		InvokeHelper(0x4bb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Version()
	{
		CString result;
		InvokeHelper(0x188, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x22e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x22e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Volatile(VARIANT Volatile)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x314, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Volatile);
	}
	void _Wait(VARIANT Time)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x189, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Time);
	}
	double get_Width()
	{
		double result;
		InvokeHelper(0x7a, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Width(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_WindowsForPens()
	{
		BOOL result;
		InvokeHelper(0x18b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_WindowState()
	{
		long result;
		InvokeHelper(0x18c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_WindowState(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x18c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_UILanguage()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_UILanguage(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DefaultSheetDirection()
	{
		long result;
		InvokeHelper(0xe5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DefaultSheetDirection(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xe5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CursorMovement()
	{
		long result;
		InvokeHelper(0xe8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CursorMovement(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xe8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ControlCharacters()
	{
		BOOL result;
		InvokeHelper(0xe9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ControlCharacters(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xe9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT _WSFunction(VARIANT Arg1, VARIANT Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8, VARIANT Arg9, VARIANT Arg10, VARIANT Arg11, VARIANT Arg12, VARIANT Arg13, VARIANT Arg14, VARIANT Arg15, VARIANT Arg16, VARIANT Arg17, VARIANT Arg18, VARIANT Arg19, VARIANT Arg20, VARIANT Arg21, VARIANT Arg22, VARIANT Arg23, VARIANT Arg24, VARIANT Arg25, VARIANT Arg26, VARIANT Arg27, VARIANT Arg28, VARIANT Arg29, VARIANT Arg30)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xa9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
		return result;
	}
	BOOL get_EnableEvents()
	{
		BOOL result;
		InvokeHelper(0x4bc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EnableEvents(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4bc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayInfoWindow()
	{
		BOOL result;
		InvokeHelper(0x4bd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayInfoWindow(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4bd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL Wait(VARIANT Time)
	{
		BOOL result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6ea, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, &Time);
		return result;
	}
	BOOL get_ExtendList()
	{
		BOOL result;
		InvokeHelper(0x701, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ExtendList(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x701, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_OLEDBErrors()
	{
		LPDISPATCH result;
		InvokeHelper(0x702, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString GetPhonetic(VARIANT Text)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x703, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, &Text);
		return result;
	}
	LPDISPATCH get_COMAddIns()
	{
		LPDISPATCH result;
		InvokeHelper(0x704, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DefaultWebOptions()
	{
		LPDISPATCH result;
		InvokeHelper(0x705, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_ProductCode()
	{
		CString result;
		InvokeHelper(0x706, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_UserLibraryPath()
	{
		CString result;
		InvokeHelper(0x707, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_AutoPercentEntry()
	{
		BOOL result;
		InvokeHelper(0x708, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoPercentEntry(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x708, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_LanguageSettings()
	{
		LPDISPATCH result;
		InvokeHelper(0x709, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Dummy101()
	{
		LPDISPATCH result;
		InvokeHelper(0x70a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Dummy12(LPDISPATCH p1, LPDISPATCH p2)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH ;
		InvokeHelper(0x70b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p1, p2);
	}
	LPDISPATCH get_AnswerWizard()
	{
		LPDISPATCH result;
		InvokeHelper(0x70c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void CalculateFull()
	{
		InvokeHelper(0x70d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL FindFile()
	{
		BOOL result;
		InvokeHelper(0x6eb, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_CalculationVersion()
	{
		long result;
		InvokeHelper(0x70e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_ShowWindowsInTaskbar()
	{
		BOOL result;
		InvokeHelper(0x70f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowWindowsInTaskbar(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x70f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FeatureInstall()
	{
		long result;
		InvokeHelper(0x710, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FeatureInstall(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x710, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Ready()
	{
		BOOL result;
		InvokeHelper(0x78c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	VARIANT Dummy13(VARIANT Arg1, VARIANT Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8, VARIANT Arg9, VARIANT Arg10, VARIANT Arg11, VARIANT Arg12, VARIANT Arg13, VARIANT Arg14, VARIANT Arg15, VARIANT Arg16, VARIANT Arg17, VARIANT Arg18, VARIANT Arg19, VARIANT Arg20, VARIANT Arg21, VARIANT Arg22, VARIANT Arg23, VARIANT Arg24, VARIANT Arg25, VARIANT Arg26, VARIANT Arg27, VARIANT Arg28, VARIANT Arg29, VARIANT Arg30)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x78d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
		return result;
	}
	LPDISPATCH get_FindFormat()
	{
		LPDISPATCH result;
		InvokeHelper(0x78e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_FindFormat(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x78e, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_ReplaceFormat()
	{
		LPDISPATCH result;
		InvokeHelper(0x78f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_ReplaceFormat(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x78f, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_UsedObjects()
	{
		LPDISPATCH result;
		InvokeHelper(0x790, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_CalculationState()
	{
		long result;
		InvokeHelper(0x791, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_CalculationInterruptKey()
	{
		long result;
		InvokeHelper(0x792, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CalculationInterruptKey(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x792, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Watches()
	{
		LPDISPATCH result;
		InvokeHelper(0x793, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_DisplayFunctionToolTips()
	{
		BOOL result;
		InvokeHelper(0x794, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayFunctionToolTips(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x794, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AutomationSecurity()
	{
		long result;
		InvokeHelper(0x795, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AutomationSecurity(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x795, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_FileDialog(long fileDialogType)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x796, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, fileDialogType);
		return result;
	}
	void Dummy14()
	{
		InvokeHelper(0x798, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void CalculateFullRebuild()
	{
		InvokeHelper(0x799, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_DisplayPasteOptions()
	{
		BOOL result;
		InvokeHelper(0x79a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayPasteOptions(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x79a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayInsertOptions()
	{
		BOOL result;
		InvokeHelper(0x79b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayInsertOptions(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x79b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_GenerateGetPivotData()
	{
		BOOL result;
		InvokeHelper(0x79c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_GenerateGetPivotData(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x79c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_AutoRecover()
	{
		LPDISPATCH result;
		InvokeHelper(0x79d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Hwnd()
	{
		long result;
		InvokeHelper(0x79e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Hinstance()
	{
		long result;
		InvokeHelper(0x79f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void CheckAbort(VARIANT KeepAbort)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7a0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &KeepAbort);
	}
	LPDISPATCH get_ErrorCheckingOptions()
	{
		LPDISPATCH result;
		InvokeHelper(0x7a2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_AutoFormatAsYouTypeReplaceHyperlinks()
	{
		BOOL result;
		InvokeHelper(0x7a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoFormatAsYouTypeReplaceHyperlinks(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7a3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_SmartTagRecognizers()
	{
		LPDISPATCH result;
		InvokeHelper(0x7a4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_NewWorkbook()
	{
		LPDISPATCH result;
		InvokeHelper(0x61d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_SpellingOptions()
	{
		LPDISPATCH result;
		InvokeHelper(0x7a5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Speech()
	{
		LPDISPATCH result;
		InvokeHelper(0x7a6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_MapPaperSize()
	{
		BOOL result;
		InvokeHelper(0x7a7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MapPaperSize(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7a7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowStartupDialog()
	{
		BOOL result;
		InvokeHelper(0x7a8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowStartupDialog(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7a8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_DecimalSeparator()
	{
		CString result;
		InvokeHelper(0x711, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_DecimalSeparator(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x711, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ThousandsSeparator()
	{
		CString result;
		InvokeHelper(0x712, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ThousandsSeparator(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x712, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_UseSystemSeparators()
	{
		BOOL result;
		InvokeHelper(0x7a9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_UseSystemSeparators(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7a9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_ThisCell()
	{
		LPDISPATCH result;
		InvokeHelper(0x7aa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_RTD()
	{
		LPDISPATCH result;
		InvokeHelper(0x7ab, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_DisplayDocumentActionTaskPane()
	{
		BOOL result;
		InvokeHelper(0x8cb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayDocumentActionTaskPane(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x8cb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void DisplayXMLSourcePane(VARIANT XmlMap)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x8cc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &XmlMap);
	}
	BOOL get_ArbitraryXMLSupportAvailable()
	{
		BOOL result;
		InvokeHelper(0x8ce, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	VARIANT Support(LPDISPATCH Object, long ID, VARIANT arg)
	{
		VARIANT result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x8cf, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Object, ID, &arg);
		return result;
	}

	// _Application properties
public:

};
