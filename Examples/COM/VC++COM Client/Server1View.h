// Server1View.h : interface of the CServer1View class
//


#pragma once


class CServer1View : public CView
{
protected: // create from serialization only
	CServer1View();
	DECLARE_DYNCREATE(CServer1View)

// Attributes
public:
	CServer1Doc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CServer1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Server1View.cpp
inline CServer1Doc* CServer1View::GetDocument() const
   { return reinterpret_cast<CServer1Doc*>(m_pDocument); }
#endif

