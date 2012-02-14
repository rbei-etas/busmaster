/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		DrawFct.h: Declaration of some GDI utility functions
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila major re-design of old CNTGraphCtrl
//
#ifndef __DRAW_FCT__H__
#define __DRAW_FCT__H__

void WINAPI DrawDiamond(HDC hDC, LPPOINT point, int symsz);
void WINAPI DrawAsterisk(HDC hDC, LPPOINT point, int symsz);
void WINAPI DrawDownTriangle(HDC hDC, LPPOINT point, int symsz);
void WINAPI DrawRightTriangle(HDC hDC, LPPOINT point, int symsz);
void WINAPI DrawUpTriangle(HDC hDC, LPPOINT point, int symsz);
void WINAPI DrawLeftTriangle(HDC hDC, LPPOINT point, int symsz);

#endif // __DRAW_FCT__H__