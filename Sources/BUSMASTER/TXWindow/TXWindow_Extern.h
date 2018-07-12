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
 * \file      TxWnd_Extern.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#if defined USAGEMODE
#undef USAGEMODE
#endif

#include "Utility/XMLUtils.h"
//#include "DataTypes/Cluster.h"
#include "IBMNetWorkGetService.h"
#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#ifdef __cplusplus

extern "C" {  // only need to export C interface if used by C++ source code
#endif

    USAGEMODE HRESULT TXComman_nSetNetworkConfig(ETYPE_BUS eBus, IBMNetWorkGetService* ouFlexConfig);

    USAGEMODE HRESULT TXComman_vShowConfigureMsgWindow(void* pParentWnd, ETYPE_BUS eBUS);

    USAGEMODE HRESULT TXLIN_vCreateScheduleConfigDlg(void* pParentWnd, IBMNetWorkGetService* pClusterConfig);
    USAGEMODE HRESULT TXLIN_vShowScheduleConfigDlg(bool bShow);

    USAGEMODE HRESULT TXComman_vSetScheduleConfig(xmlDocPtr);
    USAGEMODE HRESULT TXComman_vGetScheduleConfig(xmlNodePtr);
    USAGEMODE HRESULT TXComman_vSetClientID(ETYPE_BUS eBusType, DWORD dwClientID);
    USAGEMODE HRESULT TXComman_vSetDILInterfacePtr(ETYPE_BUS eBusType, void* pDilPointer);
    USAGEMODE HRESULT TXComman_vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam);
    USAGEMODE HRESULT TXComman_hConfigWindowShown(ETYPE_BUS eBusType);




    USAGEMODE HRESULT TX_vBusStatusChanged(ETYPE_BUS eBusType,  ESTATUS_BUS eBusStatus);

    USAGEMODE HRESULT TXComman_vGetTxWndConfigData( ETYPE_BUS eBusType, xmlNodePtr pxmlNodePtr);
    USAGEMODE HRESULT TXComman_vSetTxWndConfigDataXML(ETYPE_BUS eBusType, xmlDocPtr pDoc);







#ifdef __cplusplus
}
#endif
