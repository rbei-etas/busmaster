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
 * \file      ListHWInterface.cpp
 * \brief     Source file for CListHWInterface class.
 * \authors   Ratnadip Choudhury, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CListHWInterface class.
 */

#include "CongDlg_stdafx.h"
#include "ConfigDlg_Resource.h"
#include "ListHWInterface.h"
#include "DataTypes/DIL_Datatypes.h"

IMPLEMENT_DYNAMIC(CListHWInterface, CDialog)
CListHWInterface::CListHWInterface(CWnd* pParent /*=NULL*/,int nCount,INTERFACE_HW* pInterfaceList)
	: CDialog(CListHWInterface::IDD, pParent)
    , m_nCount(nCount)
    , m_nSelIndex(-1)
    , m_pInterfaceList(pInterfaceList)
    , m_omCardName(_T(""))
    , m_omVendor(_T(""))
    , m_omDescription(_T(""))
{
}

CListHWInterface::~CListHWInterface()
{
}

void CListHWInterface::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_omListBox);
    DDX_Text(pDX, IDC_NAME, m_omCardName);
    DDX_Text(pDX, IDC_VENDOR, m_omVendor);
    DDX_Text(pDX, IDC_DESC, m_omDescription);
}

BOOL CListHWInterface::OnInitDialog()
{
    CDialog::OnInitDialog();

    if (m_pInterfaceList != NULL)
    {
        for (int i = 0; i < m_nCount; i++)
        {
            CString omTemp;
            if ((m_pInterfaceList + i)->m_acDeviceName[0] != '\0')
            {
                omTemp.Format(_T("0x%X (%s)"), (m_pInterfaceList + i)->m_dwIdInterface,
                                        (m_pInterfaceList + i)->m_acDeviceName);
            }
            else
            {
                omTemp.Format(_T("0x%X"), (m_pInterfaceList + i)->m_dwIdInterface);
            }
            m_omListBox.InsertString(i, omTemp);
        }
        m_omListBox.SetCurSel(0); // Select first entry as a default

        vUpdateControlsWithCurrSel();
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CListHWInterface::OnBnClickedOk()
{
    m_nSelIndex = m_omListBox.GetCurSel();
    OnOK();
}

BEGIN_MESSAGE_MAP(CListHWInterface, CDialog)
ON_BN_CLICKED(IDOK, OnBnClickedOk)
ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList3)
END_MESSAGE_MAP()

void CListHWInterface::OnLbnSelchangeList3()
{
    vUpdateControlsWithCurrSel();
}

void CListHWInterface::vUpdateControlsWithCurrSel(void)
{
    // Get the currently selected entry in the list control
    int i = m_omListBox.GetCurSel(); 
    m_omCardName = (m_pInterfaceList + i)->m_acNameInterface;
    m_omDescription = (m_pInterfaceList + i)->m_acDescription;

    switch ((m_pInterfaceList + i)->m_dwVendor)
    {
    case DRIVER_CAN_ETAS_BOA:
    case DRIVER_CAN_ETAS_ES581:
        m_omVendor = _T("ETAS GmbH");
        break;
    case DRIVER_CAN_ICS_NEOVI:
        m_omVendor = _T("Intrepid Control Systems, Inc.");
        break;
    case DRIVER_CAN_KVASER_CAN:
        m_omVendor = _T("Kvaser AB");
        break;
    case DRIVER_CAN_PEAK_USB:
        m_omVendor = _T("Peak GmbH");
        break;
    case DRIVER_CAN_VECTOR_XL:
        m_omVendor = _T("Vector Informatik GmbH");
        break;
    }

    UpdateData(FALSE);
}
