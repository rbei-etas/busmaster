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
 * \file      TSExecutorBase.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "TestSetupEditorLib/TestSetupEntity.h"
#include "TSExecutorResult.h"
#include "ExpressionExecutor.h"
class CTSExecutorBase
{
protected:
    CExpressionExecutor m_ouExpressionEWxecutor;
    CListCtrl* m_pomResultDispaly;
public:
    CTSExecutorBase(void);
    virtual HRESULT TSX_DoInitialization(void) = 0;
    virtual HRESULT TSX_Reset(void) = 0;
    virtual HRESULT TSX_bStartStopReadThread(BOOL bStart) = 0;
    virtual HRESULT TSX_SendMessage(CBaseEntityTA* pEntity) = 0;
    virtual HRESULT TSX_VerifyMessage(CBaseEntityTA* pEntity, CResultVerify& ouVerifyResult) = 0;
    virtual HRESULT TSX_VerifyResponse(CBaseEntityTA* pEntity, CResultVerify& ouVerifyResult) = 0;
    virtual ~CTSExecutorBase(void);
    HRESULT TSX_SetDisplayWnd(CListCtrl* pomResultDispaly);
protected:
    HRESULT TSX_DisplayMessage(CString& omStrMsg);
    HRESULT TSX_DisplayResult(CString& omStrResult);
};
