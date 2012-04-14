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
 * \file      PluginManagerBase.h
 * \brief     Definition of Plugin Manager Base
 * \author    Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of Plugin Manager Base
 */

#pragma once

#include "BaseConverter.h"
#include "afxtempl.h"
typedef struct tagConverterInfo
{
    CBaseConverter *m_pouConverter;
    HMODULE m_hModule;
    CString m_strFileName;
    CPropertyPage* m_pomPage;
    tagConverterInfo()
    {
        m_pomPage = NULL;
        m_pouConverter = NULL;
        m_hModule = NULL;
    }
}ConverterInfo;

typedef CList<ConverterInfo, ConverterInfo& > ConvertersInfoList;

class CPluginManagerBase
{
//Member variables
public:
    ConvertersInfoList m_ConverterList;
//Member functions
public:
    CPluginManagerBase(void);
    virtual ~CPluginManagerBase(void);
    virtual HRESULT LoadConvertersFromFolder(CONST TCHAR* pchPluginFolder = NULL)=0;
    virtual HRESULT LoadConverter(CString& strFileName)=0;
    virtual HRESULT UnLoadAllPlugins()=0;
protected:
};
