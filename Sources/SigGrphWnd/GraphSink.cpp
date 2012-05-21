// GraphSink.cpp : implementation file
//

#include "SigGrphWnd_stdafx.h"
#include "GraphSink.h"
#include "GraphBottomView.h"


// CGraphSink

IMPLEMENT_DYNAMIC(CGraphSink, CCmdTarget)


CGraphSink::CGraphSink()
{
    EnableAutomation();
    m_pParentWnd = NULL;
}

CGraphSink::~CGraphSink()
{
}


void CGraphSink::OnFinalRelease()
{
    // When the last reference for an automation object is released
    // OnFinalRelease is called.  The base class will automatically
    // deletes the object.  Add additional cleanup required for your
    // object before calling the base class.
    CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CGraphSink, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CGraphSink, CCmdTarget)
    DISP_FUNCTION_ID(CGraphSink,"CursorPosition",1,OnCursorPosition,VT_EMPTY,VTS_R8 VTS_R8 VTS_I2)
END_DISPATCH_MAP()

// Note: we add support for IID_IGraphSink to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {AC152B1A-8D77-472F-B442-BB9117F381AB}
static const IID IID_IGraphSink =
{ 0xAC152B1A, 0x8D77, 0x472F, { 0xB4, 0x42, 0xBB, 0x91, 0x17, 0xF3, 0x81, 0xAB } };

BEGIN_INTERFACE_MAP(CGraphSink, CCmdTarget)
INTERFACE_PART(CGraphSink, DIID__IDMGraphCtrlEvents, Dispatch)
END_INTERFACE_MAP()


// CGraphSink message handlers

void CGraphSink::OnCursorPosition(double x, double y, short shCursorID)
{
    if(m_pParentWnd)
    {
        ((CGraphBottomView*)m_pParentWnd->m_pomBottomView)->
        vUpdateCursordetails(x, y, shCursorID);
    }
}
