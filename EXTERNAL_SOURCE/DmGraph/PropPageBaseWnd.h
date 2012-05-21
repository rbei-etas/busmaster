/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		PropPageBaseWnd.h: Declaration of the CPropPageBaseWnd class
//
// CLASS NAME
//		CPropPageBaseWnd
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila created
//


#if !defined(AFX_PROPPAGEBASEWND_H__72038DCC_2019_40FA_B5CD_5A9D4F15156F__INCLUDED_)
#define AFX_PROPPAGEBASEWND_H__72038DCC_2019_40FA_B5CD_5A9D4F15156F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPropPageBaseWnd : public CWindow  
{
public:
	HRESULT InitCollectionList(IDMGraphCollection* pGraphCollection);
	HRESULT AppendNewItem(IDMGraphCollection* pGraphCollection);
	HRESULT DeleteItem(IDMGraphCollection* pGraphCollection);
	HRESULT RemoveSelection(IDMGraphCollection* pGraphCollection);
};

#endif // !defined(AFX_PROPPAGEBASEWND_H__72038DCC_2019_40FA_B5CD_5A9D4F15156F__INCLUDED_)
