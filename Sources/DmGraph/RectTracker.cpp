// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Ivan Krivyakov, 2011
//
// FILE NAME
//		RectTracker.cpp: implementation for the CRectTracker class
//
// CLASS NAME
//		CRectTracker
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila created
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "RectTracker.h"

// Code based on:
// http://www.codeguru.com/cpp/misc/misc/graphics/article.php/c331

static COLORREF const CLR_WHITE = RGB(255,255,255);

static void WINAPI NormalizeRect(LPRECT prc);
static void WINAPI DrawRect(HDC hDC, const LPPOINT pTopLeft, const LPPOINT pBottomRight);


//------------------------------------------------------
CRectTracker::CRectTracker()
{
    SetDefaults();
    ZeroMemory(&CurPos.Begin, sizeof(POINT));
    ZeroMemory(&CurPos.End, sizeof(POINT));
};

//------------------------------------------------------
CRectTracker::CRectTracker( POINT const& Begin, POINT const& End )
{
    SetDefaults();
    CurPos.Begin = Begin;
    CurPos.End = End;
};

//------------------------------------------------------
void CRectTracker::SetDefaults()
{
    LineWidth = 1;
    LineColor = CLR_WHITE;
    HandleSize.cx = HandleSize.cy = 8;
    HandleColor = CLR_WHITE;
    VicinitySize = 10;
    ZeroMemory(&ValidRect, sizeof(RECT));
};

//------------------------------------------------------
void CRectTracker::GetPos( POINT* Begin, POINT* End ) const
{
    *Begin = CurPos.Begin;
    *End = CurPos.End;
};

//------------------------------------------------------
void CRectTracker::SetPos( POINT const& Begin, POINT const& End )
{
    CurPos.Begin = Begin;
    CurPos.End = End;
};

//------------------------------------------------------    
void CRectTracker::SetValidRect( RECT const& Rect )
{
    ValidRect = Rect; 
    NormalizeRect(&ValidRect); 
}; 

//------------------------------------------------------    
BOOL CRectTracker::IsValidPos( KLinePos const& Pos )
{
    if (IsRectEmpty(&ValidRect))
        return TRUE;

    return PtInRect(&ValidRect, Pos.Begin) && PtInRect(&ValidRect, Pos.End);
};

//------------------------------------------------------    
RECT CRectTracker::GetHandleRect( KHandleType H ) const
{
    RECT R;
    
    if (H == handleBegin)
    {
        R.left = CurPos.Begin.x - HandleSize.cx/2;
        R.top  = CurPos.Begin.y - HandleSize.cy/2;
    }
    else
    {
        R.left = CurPos.End.x - HandleSize.cx/2;
        R.top  = CurPos.End.y - HandleSize.cy/2;
    };

    R.right = R.left + HandleSize.cx;
    R.bottom = R.top + HandleSize.cy;
    return R;
};

//------------------------------------------------------    
void CRectTracker::Draw( HDC hDC ) 
{
    SaveDC(hDC);
    HPEN LinePen = CreatePen( PS_SOLID, LineWidth, LineColor );
    HBRUSH HandleBrush = CreateSolidBrush( HandleColor );

    HPEN hOldPen = (HPEN)SelectObject(hDC, LinePen);

	DrawRect(hDC, &CurPos.Begin, &CurPos.End);

	SelectObject(hDC, hOldPen);

    FillRect(hDC, &GetHandleRect(handleBegin), HandleBrush );
    FillRect(hDC, &GetHandleRect(handleEnd), HandleBrush );

	DeleteObject(hOldPen);
	DeleteObject(HandleBrush);
};

//------------------------------------------------------    
int CRectTracker::HitTest( const LPPOINT point ) const
{
	ATLASSERT(point);

    // check handles
    RECT R = GetHandleRect(handleBegin);

    if (PtInRect(&R, *point))
        return hitBegin;

    R = GetHandleRect(handleEnd);
    if (PtInRect(&R, *point))
        return hitEnd;

    SetRect(&R, CurPos.Begin.x, CurPos.Begin.y, CurPos.End.x, CurPos.End.y );
    NormalizeRect(&R);
    RECT VicX = R;
    RECT VicY = R;

    InflateRect(&VicX, VicinitySize, 0 );
    InflateRect(&VicY, 0, VicinitySize );

    if (!PtInRect(&VicX, *point) && !PtInRect(&VicY, *point))
        return hitNothing;

    // check distance from the point to the line by x and by y
    int dx = CurPos.End.x - CurPos.Begin.x;
    int dy = CurPos.End.y - CurPos.Begin.y;
	double r = sqrt((double)(dx*dx + dy*dy));

    // compute coordinates relative to the origin of the line
    int x1 = point->x - CurPos.Begin.x;
    int y1 = point->y - CurPos.Begin.y;

    // compute coordinates relative to the line
    double x2 = (x1*dx + y1*dy)/r;
    double y2 = (-x1*dy + y1*dx)/r;

    if ((x2>=0) && (x2<=r) && (y2<=VicinitySize) && (y2 >= -VicinitySize))
        return hitMiddle;

    return hitNothing;
};

//------------------------------------------------------    
BOOL CRectTracker::SetCursor( HWND hWnd, UINT nHitTest ) const
{
    // trackers should only be in client area
    if (nHitTest != HTCLIENT)
	    return FALSE;

    // convert cursor position to client co-ordinates
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(hWnd, &point);

    // do hittest and normalize hit
    switch (HitTest(&point))
    {
        case hitBegin:
            ::SetCursor( LoadCursor(NULL, IDC_SIZEWE ) );
            break;

        case hitEnd:
            ::SetCursor( LoadCursor(NULL, IDC_SIZEWE ) );
            break;

        case hitMiddle:
            ::SetCursor( LoadCursor(NULL, IDC_SIZEALL ) );
            break;

        default:
            return FALSE;
    };

    return TRUE;
};        

//------------------------------------------------------    
BOOL CRectTracker::Track( HWND hWnd, const LPPOINT point )
{
    return TrackHandle( hWnd, point, HitTest(point) );
};

//------------------------------------------------------    
BOOL CRectTracker::TrackRubberBand( HWND hWnd, const LPPOINT point )
{
	ATLASSERT(point);

    memcpy(&CurPos.Begin, point, sizeof(POINT));
    memcpy(&CurPos.End, point, sizeof(POINT));
    return TrackHandle( hWnd, point, hitEnd );
};

//------------------------------------------------------    
BOOL CRectTracker::TrackHandle( HWND hWnd, const LPPOINT point, int HitTest )
{
	ATLASSERT(point);

    if ((HitTest != hitBegin) && (HitTest != hitEnd) && (HitTest != hitMiddle))
        return FALSE;

    if (!IsValidPos())
        return FALSE;

    // set mouse cursor parameters
    RECT CursorRect;

    if (IsRectEmpty(&ValidRect))
    {
        GetClientRect(hWnd, &CursorRect);
    }
    else
    {
        // valid rectangle is not empty
        if (HitTest == hitMiddle)
        {
            RECT BeginRect = ValidRect;
			OffsetRect(&BeginRect, point->x, point->y);
			OffsetRect(&BeginRect,  -CurPos.Begin.x, -CurPos.Begin.y);
            NormalizeRect(&BeginRect);

            RECT EndRect = ValidRect;
			OffsetRect(&EndRect, point->x, point->y);
			OffsetRect(&EndRect,  -CurPos.End.x, -CurPos.End.y);
            NormalizeRect(&EndRect);

            CursorRect = ValidRect;
            IntersectRect(&CursorRect, &CursorRect, &BeginRect);
            IntersectRect(&CursorRect, &CursorRect, &EndRect);
        }
        else
            CursorRect = ValidRect;
    };

    if (IsRectEmpty(&CursorRect))
        return FALSE;

    ClientToScreen(hWnd, (LPPOINT)&CursorRect );
    ClientToScreen(hWnd, ((LPPOINT)&CursorRect)+1 );
    ClipCursor(&CursorRect);
    SetCapture(hWnd);

	// get DC for drawing
	HDC dc = GetDC(hWnd);

    // set dc parameters
    HPEN LinePen = CreatePen( PS_SOLID, LineWidth, CLR_WHITE );
    HPEN hOldPen = (HPEN)SelectObject(dc, LinePen);
    SetROP2(dc, R2_XORPEN );

    KLinePos OriginalPos = CurPos;
    BOOL bCanceled = FALSE;

    // draw the rectangle for the first time
	DrawRect(dc, &CurPos.Begin, &CurPos.End);

	// get messages until capture lost or cancelled/accepted
    BOOL bExit = FALSE;
    KLinePos NewPos;

	while (!bExit)
	{
		MSG msg;
		BOOL b = ::GetMessage(&msg, NULL, 0, 0);
		ATLASSERT(b);

		if (GetCapture() != hWnd)
			break; // exit loop

		switch (msg.message)
		{
		    // handle movement/accept messages
		    case WM_LBUTTONUP:
		    case WM_MOUSEMOVE:
                {
			        NewPos = CurPos;
                    POINT MousePoint = { (int)(short)LOWORD(msg.lParam), (int)(short)HIWORD(msg.lParam) };

                    switch (HitTest)
                    {
                        case hitBegin:
                            NewPos.Begin = MousePoint;
                            break;

                        case hitEnd:
                            NewPos.End = MousePoint;
                            break;

                        case hitMiddle:
                             NewPos.Begin.x = OriginalPos.Begin.x + (MousePoint.x-point->x);
                             NewPos.Begin.y = OriginalPos.Begin.y + (MousePoint.y-point->y);

                             NewPos.End.x = OriginalPos.End.x + (MousePoint.x-point->x);
                             NewPos.End.y = OriginalPos.End.y + (MousePoint.y-point->y);
                             break;
                    };

                    // redraw the line
                    if ((NewPos.Begin.x != CurPos.Begin.x) || (NewPos.Begin.y != CurPos.Begin.y) ||
						(NewPos.End.x != CurPos.End.x) || (NewPos.End.y != CurPos.End.y))
                    {
                        // draw new rectangle
						DrawRect(dc, &NewPos.Begin, &NewPos.End);

                        // erase old rectangle
						DrawRect(dc, &CurPos.Begin, &CurPos.End);
                    };

                    if (IsValidPos(NewPos))
                        CurPos = NewPos;

                    if (msg.message == WM_LBUTTONUP)
                        bExit = TRUE;
                };
    	        break;

		    // handle cancel messages
		    case WM_KEYDOWN:
			    if (msg.wParam == VK_ESCAPE)
                {
                    bCanceled = TRUE;
                    bExit = TRUE;
                };
			    break;

		    case WM_RBUTTONDOWN:
                bCanceled = TRUE;
                bExit = TRUE;
                break;

		    // just dispatch rest of the messages
		    default:
			    DispatchMessage(&msg);
			    break;
	    }
    };

    ClipCursor(NULL);
    ReleaseCapture();

    if (bCanceled)
        CurPos = OriginalPos;

    SelectObject(dc, hOldPen);
	DeleteObject(LinePen);
	ReleaseDC(hWnd, dc);
    return !bCanceled;
};


////////////////////////////

void WINAPI NormalizeRect(LPRECT prc)
{
	int nTemp;
	if (prc->left > prc->right)
	{
		nTemp = prc->left;
		prc->left = prc->right;
		prc->right = nTemp;
	}
	if (prc->top > prc->bottom)
	{
		nTemp = prc->top;
		prc->top = prc->bottom;
		prc->bottom = nTemp;
	}
}

static void WINAPI DrawRect(HDC hDC, const LPPOINT pTopLeft, const LPPOINT pBottomRight)
{
	POINT pts[5];
	pts[0].x = pTopLeft->x;
	pts[0].y = pTopLeft->y;
	pts[1].x = pTopLeft->x;
	pts[1].y = pBottomRight->y;
	pts[2].x = pBottomRight->x;
	pts[2].y = pBottomRight->y;
	pts[3].x = pBottomRight->x;
	pts[3].y = pTopLeft->y;
	pts[4].x = pTopLeft->x;
	pts[4].y = pTopLeft->y;
	Polyline(hDC, pts, 5);
}
