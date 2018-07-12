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
 * \file      MsgWndThread.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#pragma once

//#include "MsgFrmtWnd.h"

//#define WM_MODIFY_VISIBILITY      (WM_USER + 14)
#define WM_SHUTDOWN_MSGWNDTHREAD    (WM_USER + 15)

// CMsgWndThread
class CMsgFrmtWnd;

class CMsgWndThread /*: public CWinThread*/
{
public:
    //DECLARE_DYNCREATE(CMsgWndThread)
    CMsgWndThread();           // protected constructor used by dynamic creation
protected:

    CMsgFrmtWnd* m_pMsgWnd[MAX_PROTOCOL];
    class CMsgContainerBase* mMsgContainer[BUS_TOTAL];
public:
    virtual ~CMsgWndThread();
    //virtual BOOL InitInstance();
    //virtual int ExitInstance();

    BOOL CreateMsgWnd(HWND hFrameWnd, ETYPE_BUS eBusType, DWORD dwClientID, void* pParam);
    void vUpdateClientID(ETYPE_BUS eBusType, DWORD dwClientID);
    afx_msg void vModifyVisibilityStatus(UINT, ETYPE_BUS);
    afx_msg void vShutdownThread(UINT, LONG);
    HWND hGetHandleMsgWnd(ETYPE_BUS eBusType);
    void vSetDILInterfacePointer(ETYPE_BUS eBusType, void** ppvJ1939DIL);
    void toggleMessageWindowDisplay(ETYPE_BUS);
    bool isWindowVisible(ETYPE_BUS);

protected:
    //DECLARE_MESSAGE_MAP()
};


