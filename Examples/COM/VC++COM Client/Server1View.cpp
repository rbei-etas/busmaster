// Server1View.cpp : implementation of the CServer1View class
//

#include "stdafx.h"
#include "Server1.h"

#include "Server1Doc.h"
#include "Server1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServer1View

IMPLEMENT_DYNCREATE(CServer1View, CView)

BEGIN_MESSAGE_MAP(CServer1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CServer1View construction/destruction

CServer1View::CServer1View()
{
	// TODO: add construction code here

}

CServer1View::~CServer1View()
{
}

BOOL CServer1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CServer1View drawing

void CServer1View::OnDraw(CDC* /*pDC*/)
{
	CServer1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CServer1View printing

BOOL CServer1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CServer1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CServer1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CServer1View diagnostics

#ifdef _DEBUG
void CServer1View::AssertValid() const
{
	CView::AssertValid();
}

void CServer1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CServer1Doc* CServer1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServer1Doc)));
	return (CServer1Doc*)m_pDocument;
}
#endif //_DEBUG


// CServer1View message handlers
