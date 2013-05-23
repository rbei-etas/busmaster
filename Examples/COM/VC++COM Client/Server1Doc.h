// Server1Doc.h : interface of the CServer1Doc class
//


#pragma once

class CServer1Doc : public CDocument
{
protected: // create from serialization only
	CServer1Doc();
	DECLARE_DYNCREATE(CServer1Doc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CServer1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


