/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  MsgContainer_J1939.h
  Description   :  
  $Log:   X:/Archive/Sources/PSDI_CAN/MsgContainer_J1939.h_v  $
 * 
 *    Rev 1.4   06 Jun 2011 11:57:32   CANMNTTM
 *  
 * 
 *    Rev 1.3   15 Apr 2011 19:28:10   CANMNTTM
 * Added RBEI Copyright information.

  Author(s)     :  Pradeep Kadoor
  Date Created  :  15/04/2011
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#if !defined  MSGCONTAINERJ1939_H_INCLUDED_
#define MSGCONTAINERJ1939_H_INCLUDED_

#include "afxtempl.h"
#include "Utility/Utility_Thread.h"
#include "Application/MsgWndHdrInfo.h"
#include "CommonClass/GUI_FormatMsgJ1939.h"
#include "include/BaseDefs.h"
#include "Include/CanUsbDefs.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/Filter_Datatypes.h"
#include "CommonClass/RefTimeKeeper.h"
#include "CommonClass/MsgContainerBase.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/BaseDIL_J1939.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DataTypes/MsgBufVVSE.h"

typedef void (*MSG_RX_CALL_BK)(void* pParam, ETYPE_BUS eBusType);

// To save delta time in case of append mode
typedef struct stJ1939MsgSpl : public STJ1939_MSG
{
    __int64      m_nDeltime;

    stJ1939MsgSpl()
    {
        m_nDeltime = 0;
    }
	void vGetDataStream(BYTE* pbyData) const;
	int nGetSize() const;
	void vCopyFromJ1939Msg(const STJ1939_MSG& ouSource, UINT64 u64DelTS);
}STJ1939MSGSPL;

class CMsgContainerJ1939: public CMsgContainerBase
{
private:
	CMsgBufVSE			    m_ouVSEBufJ1939;
    CJ1939BufVFSE           m_ouOWJ1939Buf; 
    CMsgBufVVSE             m_ouAppendJ1939Buf;
	STJ1939MSGSPL			m_sJ1939ReadMsgSpl;
	SFORMATTEDATA_J1939     m_sOutFormattedData;
    CFormatMsgJ1939         m_ouFormatJ1939;
	STJ1939_MSG				m_sJ1939Data;
	BYTE*					m_pbyJ1939Data;
    DWORD                   m_dwClientId;
	CBaseDILI_J1939*		m_pouDIL_J1939;
//
//private:
//	
//    //ReadMcNetData virtual function
	void vProcessNewData(STJ1939_MSG& sJ1939Msg);
	USHORT ushCalculateStrLen(bool bForHex, USHORT ushLength);
//    void vProcessCurrErrorEntry(const SERROR_INFO& sErrInfo);
//    BOOL bIsTransitionInState( UINT unChannel,
//                                       BYTE byRxError,
//                                       BYTE byTxError );
    BOOL bTobeBlocked(STJ1939_MSG& sJ1939Data);
    CRITICAL_SECTION m_sCritSecDataSync;
    CRITICAL_SECTION m_omCritSecFilter;
    SFILTERAPPLIED_J1939 m_sFilterJ1939;
//    eERROR_STATE m_eCurrErrorState[ defNO_OF_CHANNELS ];

public:
    CMsgContainerJ1939(void);
    ~CMsgContainerJ1939(void);
    void InitTimeParams(void);
    void vRetrieveDataFromBuffer();
    BOOL bGetDilInterFace();

public:
    //Exported functions
    void vInit(void* pParam);
    void vEditClearAll();
    int nGetAppendBufferCount();
    int nGetOWBufferCount();
    BOOL bStartReadThread();
    BOOL bStopReadThread();

    void vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL &sHdrColStruct,
                                     TCHAR *pomDataPtrArr[MAX_MSG_WND_COL_CNT], 
                                     BYTE bExprnFlag_Disp);
    HRESULT hReadFromOWBuffer(void *psMsg, __int64 nMapIndex);
	HRESULT hReadFromAppendBuffer(void *psMsg, int nMsgIndex);
    HRESULT hUpdateFormattedMsgStruct(int nListIndex, int &nMsgCode,
                                      BYTE bExprnFlag_Disp, __int64 nTimeOffset = 0);
    void vSetCurrMsgName(CString strMsgNameOrCode);
    void vClearFormattedMsgStruct();
    void vSaveOWandGetDetails(void *pMsg, __int64 &dwMapIndex, 
                              __int64 &dwTimeStamp, UINT &nMsgCode, int &nBufferIndex );
	void vResetFormattedData();
    
	void SetClientID(DWORD dwClientID);
	void DoSortBuffer(int nField,bool bAscending);	
	void GetMapIndexAtID(int nIndex,__int64& nMapIndex);
	HRESULT hToggleDILBufferRead(BOOL bRead);
    HRESULT ApplyFilterScheme(void* pvFilterApplied);
    HRESULT GetFilterScheme(void* pvFilterApplied);
    HRESULT EnableFilterApplied(BOOL bEnable);
	__int64 nCreateMapIndexKey(LPVOID pMsgData);
};


#endif //MSGCONTAINERJ1939_H_INCLUDED_