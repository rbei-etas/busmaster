// Server1Doc.cpp : implementation of the CServer1Doc class
//

#include "stdafx.h"
#include "Server1.h"

#include "Server1Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServer1Doc

IMPLEMENT_DYNCREATE(CServer1Doc, CDocument)

BEGIN_MESSAGE_MAP(CServer1Doc, CDocument)
END_MESSAGE_MAP()


// CServer1Doc construction/destruction

CServer1Doc::CServer1Doc()
{
	// TODO: add one-time construction code here

}

CServer1Doc::~CServer1Doc()
{
}

BOOL CServer1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CServer1Doc serialization

void CServer1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CServer1Doc diagnostics

#ifdef _DEBUG
void CServer1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CServer1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CServer1Doc commands
