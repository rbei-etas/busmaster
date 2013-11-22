/**********************************************************************************
    Project         :   Frame
    FileName        :   ListComboBox.h
    Description     :   Definition of CListComboBox class
    Directory       :
    Version         :   V1.0
    Authors         :   Ravi D
    Date Created    :   11/10/2006
    Date Modified   :
    Modifications   :
**********************************************************************************/

#pragma once
#include "afxcoll.h"


// CListComboBox

class CListComboBox : public CComboBox
{
    DECLARE_DYNAMIC(CListComboBox)

public:
    //CListComboBox();
    CListComboBox( int nItem,              // Row
                   int nSubItem,           // Sub Item
                   const CStringArray& psList,   // List of strings to populate list box
                   const CString& omStrText,      // Selected text
                   BOOL bIsEditable);      // Editable list or not

    virtual ~CListComboBox();

protected:
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnNcDestroy();
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnCbnCloseup();
    int m_nSubItem;
    int m_nItem;
    BOOL m_bVK_ESCAPE;
    BOOL m_bIsEditable;
    CStringArray m_sList;
    CString omStrText;
};


