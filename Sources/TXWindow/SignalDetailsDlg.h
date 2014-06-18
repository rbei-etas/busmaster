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
 * @brief This header file contains the defintion of class
 * @author Amarnath Shastry
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include "TxWindow_resource.h"
#include "Utility/RadixEdit.h"
#include "Utility/AlphanumiricEdit.h"
#include "CMsgSignalTemp.h"

enum eMODES;

class CSignalDetailsDlg : public CDialog
{
public:
    CSignalDetailsDlg( eMODES eMode,
                       int nDataFormat,
                       CString omStrMsgName,
                       UINT nMsgLen ,
                       CString omStrSignalType = "",
                       CString omStrMaxVal = "",
                       CString omStrMinVal = "",
                       CString omStrOffset = "0",
                       CString omStrScale = "1",
                       CWnd* pParent = nullptr);   // standard constructor

    CSignalDetailsDlg( eMODES eMode, sSIGNALS* psSigInfo,
                       CWnd* pParent /*=nullptr*/);

    enum { IDD = IDD_DLG_SIGNAL };
    CRadixEdit m_odScale;
    CRadixEdit m_odOffset;
    CRadixEdit m_odMinValue;
    CRadixEdit m_odMaxValue;
    CSpinButtonCtrl m_omSpinLen;
    CSpinButtonCtrl m_omSpinByIndex;
    CSpinButtonCtrl m_omSpinStartBit;
    CComboBox m_omComboSgType;
    SHORT m_shByteIndex;
    UINT m_unSgLen;
    CString m_omStrSignalName;
    BYTE m_byStartBit;
    CString m_omStrUnit;

    CAlphanumiricEdit m_odNumericEdit;

    /** Doesn't process a space */
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    /** DDX/DDV support */
    virtual void DoDataExchange(CDataExchange* pDX);

    /**
     * Fills the combo box control,
     * sets the range for spin controls
     */
    virtual BOOL OnInitDialog();

    /** Sets a flag and calls base class function */
    virtual void OnCancel();

    /** Updates the signal details to the data structure */
    virtual void OnOK();

private:
    BOOL m_bLenChanged;
    CString m_omStrFirstSignalName;
    BOOL m_bDupliacateFound;
    CString m_omStrSgType;
    int m_nDataFormat;
    UINT m_unMode;
    CString m_omStrPrevSignalName;
    CString m_omStrPrevSignalType;
    BOOL m_bIsDataSaved;
    BOOL m_bIsCanceled;
    CString m_omStrMsgName;
    UINT m_nMsgLength;
    CString m_omStrMaxVal;
    CString m_omStrMinVal;
    CString m_omStrOffset;
    CString m_omStrScale;
    BOOL m_bNameChanged;
};
