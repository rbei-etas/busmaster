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
 * \file      FunctionEditorBase.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "NodeSimEx_stdafx.h"
#include "FunctionEditorDoc.h"
#include "FunctionEditorBase.h"

#include "GlobalObj.h"


CFunctionEditorBase::CFunctionEditorBase()
{
}
CFunctionEditorBase::~CFunctionEditorBase()
{
}

/* Initializes Function editor with bus specific information */
BOOL CFunctionEditorBase::bInitBusInfo(SBUS_SPECIFIC_INFO& sInfo, eTYPE_BUS eBus)
{
    BOOL bReturn = TRUE;
    switch (eBus)
    {
        case CAN:
        {
            sInfo.m_eBus = CAN;
            sInfo.m_omBusName = _T("BUSMASTER");
            sInfo.m_omHeaderFileName = CGlobalObj::ouGetObj(sInfo.m_eBus).m_omMsgStructFile;
        }
        break;
        case J1939:
        {
            sInfo.m_eBus = J1939;
            sInfo.m_omBusName = _T("J1939");
            sInfo.m_omHeaderFileName = CGlobalObj::ouGetObj(sInfo.m_eBus).m_omMsgStructFile;
        }
        break;
    }
    return bReturn;

}

CFunctionEditorDoc* CFunctionEditorBase::pCreateNewDocument(eTYPE_BUS eBus)
{
    CMultiDocTemplate* pTemplate = CGlobalObj::ouGetObj(eBus).m_pEditorDocTemplate;
    // Now open the selected file
    CFunctionEditorDoc *pDoc = (CFunctionEditorDoc*)pTemplate->CreateNewDocument();
    if (pDoc != NULL)
    {
        SBUS_SPECIFIC_INFO sInfo;
        if (bInitBusInfo(sInfo, eBus))
        {
            pDoc->bInitBusSpecificInfo(sInfo);
        }
    }
    return pDoc;
}
