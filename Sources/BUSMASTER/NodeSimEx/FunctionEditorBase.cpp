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


CGlobalObj* CFunctionEditorBase::m_pGlobalObj = nullptr;

CFunctionEditorBase::CFunctionEditorBase(CGlobalObj* pGlobalObj)
{
    m_pGlobalObj = pGlobalObj;
}
CFunctionEditorBase::~CFunctionEditorBase()
{
}

/* Initializes Function editor with bus specific information */
BOOL CFunctionEditorBase::bInitBusInfo(SBUS_SPECIFIC_INFO& sInfo, ETYPE_BUS eBus, CGlobalObj* pGlobalObj)
{
    BOOL bReturn = TRUE;
    switch (eBus)
    {
        case CAN:
        {
            sInfo.m_eBus = CAN;
            sInfo.m_omBusName = "BUSMASTER";
            sInfo.m_omHeaderFileName = pGlobalObj->m_omMsgStructFile;
        }
        break;
        case J1939:
        {
            sInfo.m_eBus = J1939;
            sInfo.m_omBusName = "J1939";
            sInfo.m_omHeaderFileName = pGlobalObj->m_omMsgStructFile;
        }
        break;
        case LIN:
        {
            sInfo.m_eBus = LIN;
            sInfo.m_omBusName = "LIN";
            sInfo.m_omHeaderFileName = pGlobalObj->m_omMsgStructFile;
        }
        break;
    }
    return bReturn;

}

CFunctionEditorDoc* CFunctionEditorBase::pCreateNewDocument(ETYPE_BUS eBus)
{
    CMultiDocTemplate* pTemplate = CGlobalObj::ouGetObj(eBus).m_pEditorDocTemplate;
    if (nullptr == pTemplate)
    {
        AfxMessageBox("No Editor found. Please open the editor using simulated systems window", MB_OK);
        return nullptr;
    }

    // Now open the selected file
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)pTemplate->CreateNewDocument();
    if (pDoc != nullptr)
    {
        SBUS_SPECIFIC_INFO sInfo;
        if (bInitBusInfo(sInfo, eBus, &CGlobalObj::ouGetObj(eBus)))
        {
            pDoc->bInitBusSpecificInfo(sInfo);
        }
    }
    return pDoc;
}
