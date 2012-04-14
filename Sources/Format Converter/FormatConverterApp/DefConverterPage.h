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
 * \file      DefConverterPage.h
 * \brief     Definition of CDefConverterPage dialog
 * \author    Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CDefConverterPage dialog
 */

#pragma once

#include "resource.h"
#include "PluginManagerBase.h"
#include "afxwin.h"

class CDefConverterPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDefConverterPage)
    CPluginManagerBase* m_pouPluginManager;
public:
    CDefConverterPage();
	HRESULT SetPluginManager(CPluginManagerBase *m_pouPluginManager);
    

    virtual ~CDefConverterPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_DEF_CONVERTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CComboBox m_omComboConverterNames;
    HRESULT LoadConverters();
    INT GetConverterPos();
public:
    virtual BOOL OnInitDialog();
    CEdit m_omEditInputPath;
    CEdit m_omEditOutputPath;
    afx_msg void OnBnClickedBtnInput();
    afx_msg void OnBnClickedBtnOutput();
    afx_msg void OnBnClickedBtnConvert();
    string m_omStrInputFilePath;
    string m_omStrOutputFilePath;
    afx_msg void OnCbnSelchangeComboConversions();
    string m_omstrConversionComment;
    string m_omstrEditHelp;
};
