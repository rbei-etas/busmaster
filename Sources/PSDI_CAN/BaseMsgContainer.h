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
 * \file      BaseMsgContainer.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
class CBaseMsgContainer
{
public:
    virtual HRESULT SW_DoInitialization(void) = 0;
    virtual HRESULT SW_ShowAddDelSignalsDlg(CWnd* pParent, CMainEntryList* podMainSubList) = 0;
    virtual HRESULT SW_ShowSigWatchWnd(CWnd* pParent, INT nCmd) = 0;
    virtual HRESULT SW_GetConfigSize(void) = 0;
    virtual HRESULT SW_GetConfigData(void* pbyConfigData) = 0;
    virtual HRESULT SW_SetConfigData(const void* pbyConfigData) = 0;
    virtual HRESULT SW_ClearSigWatchWnd(void) = 0;
    virtual HRESULT SW_UpdateMsgInterpretObj(CMsgInterpretation* RefObj) = 0;
    virtual BOOL    SW_IsWindowVisible(void) = 0;
    virtual HRESULT SW_SetDisplayMode(BOOL bHex) = 0;

    virtual HRESULT vInit(void* pParam);
    virtual HRESULT vEditClearAll();
    virtual int nGetAppendBufferCount();
    virtual int nGetOWBufferCount();
    virtual BOOL bStartReadThread();
    virtual BOOL bStopReadThread();

    virtual HRESULT vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
            TCHAR* pomDataPtrArr[MAX_MSG_WND_COL_CNT],
            BYTE bExprnFlag_Disp);
    virtual HRESULT hReadFromOWBuffer(void* psMsg, __int64 nMapIndex);
    virtual HRESULT hReadFromAppendBuffer(void* psMsg, int nMsgIndex);
    virtual HRESULT hUpdateFormattedMsgStruct(int nListIndex, int& nMsgCode,
            BYTE bExprnFlag_Disp, __int64 nTimeOffset = 0);
    void vSetCurrMsgName(CString strMsgNameOrCode);
    void vClearFormattedMsgStruct();
    void vSaveOWandGetDetails(void* pMsg, __int64& dwMapIndex,
                              __int64& dwTimeStamp, UINT& nMsgCode, int& nBufferIndex );


    //void vSetRxMsgCallBkPtr(MSG_RX_CALL_BK pFuncPtr);
    void SetClientID(DWORD dwClientID);
    void DoSortBuffer(int nField,bool bAscending);
    void GetMapIndexAtID(int nIndex,__int64& nMapIndex);
    HRESULT ApplyFilterScheme(void* pvFilterApplied);
    HRESULT GetFilterScheme(void* pvFilterApplied);
    HRESULT EnableFilterApplied(BOOL bEnable);
};