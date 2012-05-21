// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		DrawFct.cpp: implementation of some GDI utility functions
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila major re-design from old CNTGraphCtrl
//
#include "stdafx.h"
#include "DrawFct.h"


////////////////////////////////////////////////////////////
// Draw Diamonds
void WINAPI DrawDiamond(HDC hDC, LPPOINT point, int symsz)
{
	ATLASSERT(hDC);
	ATLASSERT(point);

	POINT points[5];

	points[0].x = point->x;
	points[0].y = point->y - symsz;
	points[1].x = point->x + symsz;
	points[1].y = point->y;
	points[2].x = point->x;
	points[2].y = point->y + symsz;
	points[3].x = point->x - symsz;
	points[3].y = point->y;
	Polygon (hDC, points, 4);
}

// Draw Asterisk
void WINAPI DrawAsterisk(HDC hDC, LPPOINT point, int symsz)
{
	ATLASSERT(hDC);
	ATLASSERT(point);

	POINT points[4];

	points[0].x = point->x;
	points[0].y = point->y - symsz;
	points[1].x = point->x + symsz/2;
	points[1].y = point->y + symsz/2;
	points[2].x = point->x - symsz/2;
	points[2].y = point->y + symsz;
	Polygon (hDC, points, 3);

	points[0].x = point->x - symsz;
	points[0].y = point->y - symsz/2;
	points[1].x = point->x + symsz/2 ;
	points[1].y = point->y;
    points[2].x = point->x + symsz;
	points[2].y = point->y + symsz;
	Polygon (hDC, points, 3);

	points[0].x = point->x - symsz/2;
	points[0].y = point->y - symsz/2;
	points[1].x = point->x + symsz ;
	points[1].y = point->y - symsz/2;
    points[2].x = point->x - symsz/2;
	points[2].y = point->y + symsz;
	Polygon (hDC, points, 3);
}

void WINAPI DrawDownTriangle(HDC hDC, LPPOINT point, int symsz)
{
	ATLASSERT(hDC);
	ATLASSERT(point);

	POINT points[4];
	points[0].x = point->x - symsz;
	points[0].y = point->y - symsz;
	points[1].x = point->x + symsz;
	points[1].y = point->y - symsz;
	points[2].x = point->x;
	points[2].y = point->y + symsz;
	Polygon (hDC, points, 3);
}

void WINAPI DrawRightTriangle(HDC hDC, LPPOINT point, int symsz)
{
	ATLASSERT(hDC);
	ATLASSERT(point);

	POINT points[4];

	points[0].x = point->x - symsz;
	points[0].y = point->y - symsz;
	points[1].x = point->x + symsz;
	points[1].y = point->y;
	points[2].x = point->x - symsz;
	points[2].y = point->y + symsz;
	Polygon (hDC, points, 3);
}

void WINAPI DrawUpTriangle(HDC hDC, LPPOINT point, int symsz)
{
	ATLASSERT(hDC);
	ATLASSERT(point);

	POINT points[4];

	points[0].x = point->x ;
	points[0].y = point->y - symsz ;
	points[1].x = point->x + symsz;
	points[1].y = point->y + symsz;
	points[2].x = point->x - symsz;
	points[2].y = point->y + symsz;
	Polygon (hDC, points, 3);
}

void WINAPI DrawLeftTriangle(HDC hDC, LPPOINT point, int symsz)
{
	ATLASSERT(hDC);
	ATLASSERT(point);

	POINT points[4];

	points[0].x = point->x + symsz - 1;
	points[0].y = point->y - symsz;
	points[1].x = point->x - symsz;
	points[1].y = point->y;
	points[2].x = point->x + symsz - 1;
	points[2].y = point->y + symsz;
	Polygon (hDC, points, 3);
}
