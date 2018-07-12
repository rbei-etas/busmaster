/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      DatabaseDissociateDlg.h
 * \brief     This header file contains the defintion of class
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */
#pragma once
#include "Datatypes/MsgSignal_Datatypes.h"

// CDatabaseDissociateDlg dialog

class CDatabaseDissociateDlg : public CDialog
{
    DECLARE_DYNAMIC(CDatabaseDissociateDlg)
    std::list<std::string> mDbPaths;
    std::list<std::string> mDbPathsDissociated;
    enum { IDD = IDD_DLG_DISSOCIATE_DB };
public:
    CDatabaseDissociateDlg(std::list<std::string> dbPath, CWnd* pParent = nullptr);
    std::list<std::string> GetDissociatedFiles();
    virtual ~CDatabaseDissociateDlg();
    // Dialog Data

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()


public:
    CListBox m_omDissociateDbLst;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedCbtnDissociate();
    afx_msg void OnOK();
    afx_msg void OnClose();
    void OnlbnSelChangeDbList();
};
