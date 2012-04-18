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
 * \file      PluginManagerDLL.cpp
 * \brief     Implementation of CPluginManagerDLL command target
 * \authors   Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CPluginManagerDLL command target
 */

#include "stdafx.h"
#include "PluginManagerDLL.h"
#include <shlwapi.h>

// CPluginManagerDLL

typedef HRESULT (*GETCONVERTERINTERFACE)(CBaseConverter*&);

CPluginManagerDLL::CPluginManagerDLL()
{
}

CPluginManagerDLL::~CPluginManagerDLL()
{
}
HRESULT CPluginManagerDLL::LoadConvertersFromFolder(CONST TCHAR* pchPluginFolder)
{
    TCHAR m_acEvalFilePath[MAX_PATH];
    CString strPluginPath;
    if(pchPluginFolder == NULL)
    {
        GetModuleFileName( NULL, m_acEvalFilePath, MAX_PATH );
        PathRemoveFileSpec(m_acEvalFilePath);
        strPluginPath = m_acEvalFilePath;
        strPluginPath += defDEFAULTPLUGINFOLDER;
    }
    else
    {
        strPluginPath = pchPluginFolder;
    }
    
    if(PathFileExists(strPluginPath) == TRUE)
    {
        SetCurrentDirectory(strPluginPath);

        CFileFind omFileFinder;
	    CString strWildCard = defDLLFILEEXTENSION; //look for the plugin files

	    BOOL bWorking = omFileFinder.FindFile(strWildCard); 
	    while (bWorking)
	    {
	        bWorking = omFileFinder.FindNextFile(); 
		    if (omFileFinder.IsDots() || omFileFinder.IsDirectory())
			    continue;
            LoadConverter(omFileFinder.GetFilePath());
    		
	    }
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT CPluginManagerDLL::LoadConverter(CString& strFileName)
{
    ConverterInfo ConverterInfo;
    ConverterInfo.m_hModule = LoadLibrary(strFileName);
	if ( !ConverterInfo.m_hModule )
	{
		MessageBox(NULL, strFileName, _T("Plugin Loading Error"), MB_OK);
	} 
	else
	{ 
        BOOL bSuccess = TRUE;
        
        GETCONVERTERINTERFACE pFnBaseConverter;
        pFnBaseConverter = (GETCONVERTERINTERFACE)GetProcAddress(ConverterInfo.m_hModule, defCONVERTERINTERFACE);
        if(pFnBaseConverter != NULL)
        {
            pFnBaseConverter(ConverterInfo.m_pouConverter);
        }
        else
        {
            return S_FALSE;
        }
        
        //Vlaidation
        if(bSuccess == TRUE)
        {
            m_ConverterList.AddTail(ConverterInfo);
        }
	}
    return S_OK;
}


HRESULT CPluginManagerDLL::UnLoadAllPlugins()
{
    INT_PTR nCount = m_ConverterList.GetCount();
    for(INT_PTR i = 0; i < nCount; i++)
    {
        POSITION pos = m_ConverterList.FindIndex(i);
        ConverterInfo& ouConverterInfo = m_ConverterList.GetAt(pos);
        
        if(ouConverterInfo.m_pouConverter != NULL)
        { 
            delete ouConverterInfo.m_pouConverter;
        }
        if(ouConverterInfo.m_pomPage != NULL)
        {
            delete ouConverterInfo.m_pomPage;
        }
        if( ouConverterInfo.m_hModule != NULL)
        {
            FreeLibrary(ouConverterInfo.m_hModule);
        }
    }   
    return S_OK;
}