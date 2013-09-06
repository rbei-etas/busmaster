/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      PropPageBaseWnd.h: Declaration of the CPropPageBaseWnd class
//
// CLASS NAME
//      CPropPageBaseWnd
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila created
//

#pragma once

class CPropPageBaseWnd : public CWindow
{
public:
    HRESULT InitCollectionList(IDMGraphCollection* pGraphCollection);
    HRESULT AppendNewItem(IDMGraphCollection* pGraphCollection);
    HRESULT DeleteItem(IDMGraphCollection* pGraphCollection);
    HRESULT RemoveSelection(IDMGraphCollection* pGraphCollection);
};
