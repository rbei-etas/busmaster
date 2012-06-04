//#include <boost/regex.hpp>
//#include <boost/thread.hpp>
//#include <iostream>
//#include <string>
//
//int main()
//{
//    std::string line;
//    boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );
//
//    while (std::cin)
//    {
//        std::getline(std::cin, line);
//
//		boost::xtime timeWait;
//		timeWait.sec = 6000;
//		boost::this_thread::sleep(timeWait);
//
//        boost::smatch matches;
//        if (boost::regex_match(line, matches, pat))
//            std::cout << matches[2] << std::endl;
//    }
//}
//---------------------------------------------------------------------------

#include <J1939_Tester_StdAfx.h>
#include <tchar.h>
//---------------------------------------------------------------------------

#pragma argsused

//---------------------------------------------------------------------------

#define BOOST_TEST_MODULE MyTest
#define USAGE_IMPORT 1
#include <boost/test/unit_test.hpp>

#include "Include/BaseDefs.h"
#include "DIL_J1939/DIL_J1939_stdafx.h"
#include "Include/DIL_CommonDefs.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_J1939/DIL_J1939_Extern.h"
#include "DIL_J1939/ConnectionDet.h"
#include "WrapperErrorLogger.h" //For Logging Error in DIL Interface 
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufVSE.h"
#include "DIL_J1939/NodeConManager.h"
#include "DIL_J1939/MonitorNode.h"
#include "DIL_J1939/TransferLayer.h"

/* Global variables to hold CAN DIL parameters */
CBaseDIL_CAN* g_pouDIL_CAN_Interface = NULL; // CAN DIL interface
DWORD g_dwDriverId = DRIVER_CAN_VECTOR_XL;
WrapperErrorLogger g_ouWrapperLogger;	
INTERFACE_HW_LIST g_asINTERFACE_HW;
DWORD g_dwClientID = 0;
SCONTROLER_DETAILS g_asControllerDetails[defNO_OF_CHANNELS];
INT g_nChannelCount = 0;

/* J1939 Params */
DWORD g_dwJ1939ClientId[2] = {0};
const UINT64 J1939_NODE_NAME     = 0x8000000000000015;
BYTE g_arrNodeAddr[] = {0x05, 0x10};
UINT32 g_arrRqstPGNs[] = {0x5500, 0x3300};
UINT32 g_arrBroadcastPGNs[] = {0xF008, 0xFD58};
CMsgBufVSE g_ouVSEBufJ1939;

static void CallBackSample(UINT32 /*unPGN*/, BYTE /*bySrc*/, 
                                BYTE /*byDest*/, BOOL /*bSuccess*/)
{    
}

HRESULT InitializeDIL(void)
{   		
    HRESULT hResult = S_OK;
    if (g_pouDIL_CAN_Interface == NULL)
    {		
        hResult = DIL_GetInterface(CAN, (void**)&g_pouDIL_CAN_Interface);
    }
    else
    {
        g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();        
    }
    if (hResult == S_OK)
    {        				
        if ((hResult = g_pouDIL_CAN_Interface->DILC_SelectDriver(g_dwDriverId, NULL, &g_ouWrapperLogger)) == S_OK)
        {			
            g_pouDIL_CAN_Interface->DILC_PerformInitOperations();
            INT nCount = defNO_OF_CHANNELS;
            if (g_pouDIL_CAN_Interface->DILC_ListHwInterfaces(g_asINTERFACE_HW, nCount) == S_OK)
            {
                HRESULT hResult = g_pouDIL_CAN_Interface->DILC_SelectHwInterfaces(g_asINTERFACE_HW, nCount);

				g_nChannelCount = nCount;
                if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
                {
                    hResult = g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _T("CAN_MONITOR"));
                    if ((hResult == S_OK)|| (hResult == ERR_CLIENT_EXISTS))
                    {				
		                g_pouDIL_CAN_Interface->DILC_SetConfigData((PCHAR)g_asControllerDetails, 
															sizeof(SCONTROLER_DETAILS) * nCount);

                    }
                    else
                    {
						//AfxMessageBox("registering client failed");
                    }
                }
                else
                {
                    //AfxMessageBox("Selecting hardware interface failed");
                }
            }
            else
            {
				hResult = S_FALSE;
                //AfxMessageBox("Listing hardware interfaces failed");
            }
        }
        else
        {
		if ( hResult!=DAL_ALREADY_SELECTED )
		{
			//AfxMessageBox("Driver selection failed");	
		}            	
        }
    }
    return hResult;    
}

BOOST_AUTO_TEST_SUITE( J1939_Exports_Tester )

BOOST_AUTO_TEST_CASE( CAN_J1939_Initializations )
{
	HRESULT hr = S_FALSE;	

	/* CAN message */
	STCAN_MSG sMsg;

	/* Initialise message structure */
	sMsg.m_unMsgID = 0x100;              // Message ID
	sMsg.m_ucEXTENDED = FALSE;           // Standard Message type
	sMsg.m_ucRTR = FALSE;                // Not RTR type
	sMsg.m_ucDataLen = 8;                // Length is 8 Bytes
	for( int i = 0; i<8;i++)
		sMsg.m_ucData[i] = i+1;
	sMsg.m_ucData[7] = 0;

	sMsg.m_ucChannel = 1;                // First CAN channel

	
	/********************************** CAN Related activities  - Start ************************************/
	/* Initialize CAN DIL section */
	hr = InitializeDIL();

	if (hr == S_OK)
	{		
		//AfxMessageBox( "CAN initializations done!" );
		/* Connect to CAN network */
		hr = g_pouDIL_CAN_Interface->DILC_StartHardware();
		Sleep(1000);
		if (hr == S_OK)
		{
			/* Send a test CAN message */
			hr = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, sMsg);					
		}
	}
	else
	{
		BOOST_FAIL( "Failed to Initialize CAN Network..." );  
		exit(0);
	}
	/********************************** CAN Related activities  - End    ***********************************/	
}

BOOST_AUTO_TEST_CASE( J1939_Register_Go_Online )
{
	/********************************** J1939 Related activities - Start ***********************************/
	/* J1939 initialization */

	VERSIONINFO objVerJ1939;

	DILJ_GetVersionInfo(objVerJ1939);

	BOOST_REQUIRE ( S_OK == DILJ_Initialise(&g_ouWrapperLogger, g_pouDIL_CAN_Interface ) );	

	/* Register client - Montior node */
	Sleep(100);
	BOOST_REQUIRE ( S_OK == DILJ_RegisterClient(TRUE, 
                    J1939_MONITOR_NODE, J1939_ECU_NAME, g_arrNodeAddr[0], g_dwJ1939ClientId[0]) );
	
	/* Register client - Normal J1939 node */
	Sleep(100);
	BOOST_REQUIRE ( S_OK == DILJ_RegisterClient(TRUE, 
                    _T("Node 1"), J1939_NODE_NAME, g_arrNodeAddr[1], g_dwJ1939ClientId[1]) );

	/*                        Go online                  */
	BOOST_CHECK ( S_OK == DILJ_GoOnline() );		
}

BOOST_AUTO_TEST_CASE( J1939_Configure_Timeouts )
{
	/* Configure Timeouts */
	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_BROADCAST, 100) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_RESPONSE, 200) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_HOLDING, 500) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_T1, 750) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_T2, 1250) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_T3, 1250) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_T4, 1050) );

	/* Get Timeouts */
	UINT unBroadcast, unResponse, unHolding, unT1, unT2, unT3, unT4;		

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_BROADCAST, unBroadcast) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_RESPONSE, unResponse) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_HOLDING, unHolding) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_T1, unT1) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_T2, unT2) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_T3, unT3) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_T4, unT4) );

	BOOST_CHECK ( 100  == unBroadcast );
	BOOST_CHECK ( 200  == unResponse );
	BOOST_CHECK ( 500  == unHolding );
	BOOST_CHECK ( 750  == unT1 );
	BOOST_CHECK ( 1250 == unT2 );
	BOOST_CHECK ( 1250 == unT3 );
	BOOST_CHECK ( 1050 == unT4 );
}

BOOST_AUTO_TEST_CASE( J1939_Addr_Claim_Transmissions )
{	
	/* Claim Address - Success case */
	for ( int nChnlId = 1; nChnlId <= g_nChannelCount ; nChnlId++ )
	{
		for ( int nClientId = 0; nClientId < 2 ; nClientId++ )
		{
			BOOST_CHECK ( S_OK == DILJ_NM_ClaimAddress(g_dwJ1939ClientId[nClientId], nChnlId, g_arrNodeAddr[nClientId]) );
		}
	}

	/* Check if address is claimed or not - both success and failure cases*/	
	BOOST_CHECK ( TRUE  == DILJ_NM_bIsAddressClaimed(g_arrNodeAddr[0]) );
	BOOST_CHECK ( FALSE == DILJ_NM_bIsAddressClaimed(0x15) );

	/* Check if Online - should be true */
	BOOST_CHECK ( TRUE == DILJ_bIsOnline() );

	BYTE bytJ1939Data[] = "12345678123456781234567812345678";
	/* Send J1939 Message through monitor node*/
	Sleep(500);

	/* Fetch Node Name */
	TCHAR strNodeName[256];

	/* Pass a valid registered node address */
	BOOST_CHECK ( S_OK == DILJ_NM_GetNodeName(g_arrNodeAddr[0],strNodeName) );
	BOOST_CHECK ( S_OK == DILJ_NM_GetNodeName(g_arrNodeAddr[1],strNodeName) );

	/* Pass an in-valid registered node address */
	BOOST_CHECK ( S_OK == DILJ_NM_GetNodeName(0x20,strNodeName) );
	
	/* Add message buffer for monitor node*/
	BOOST_CHECK ( S_OK == DILJ_ManageMsgBuf(MSGBUF_ADD, g_dwJ1939ClientId[0], &g_ouVSEBufJ1939) );
	/* Try to add it again */
	BOOST_CHECK ( S_OK == DILJ_ManageMsgBuf(MSGBUF_ADD, g_dwJ1939ClientId[1], &g_ouVSEBufJ1939) );
	/* Try to add for an invalid Client ID */
	BOOST_CHECK ( ERROR_NOCLIENT == DILJ_ManageMsgBuf(MSGBUF_ADD, 0x255, &g_ouVSEBufJ1939) );


	/* Get Byte address */	
	for ( int nClientId = 0; nClientId < 2 ; nClientId++ )
	{
			BOOST_CHECK ( S_OK == DILJ_NM_GetByteAddres(g_arrNodeAddr[nClientId], 
														g_dwJ1939ClientId[nClientId]) );
	}

	/* Request PGN */		
	for ( int nChnlId = 1; nChnlId <= g_nChannelCount ; nChnlId++ )
	{
		for ( int nClientId = 0; nClientId < 2 ; nClientId++ )
		{
			BOOST_CHECK ( S_OK == DILJ_RequestPGN( g_dwJ1939ClientId[nClientId], 
												   nChnlId, g_arrRqstPGNs[nClientId]) );
		}
	}

	/* Set the callback - all possible cases */
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_LDATA_CONF, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_LDATA_IND, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_BC_LDATA_CONF, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_BC_LDATA_IND, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_NM_ACL, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], (ETYPE_CLBCK_FN)5, (void*)CallBackSample) );

	/* Send J1939 Broadcast Message through monitor node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[0], 1, MSG_TYPE_BROADCAST, g_arrBroadcastPGNs[0], 
											bytJ1939Data, 14,
											DEFAULT_PRIORITY, g_arrNodeAddr[0], 0x08) );

	Sleep(500);
	/* Send J1939 Broadcast Message through normal node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[1], 1, MSG_TYPE_BROADCAST, g_arrBroadcastPGNs[1], 
											bytJ1939Data, 14,
											DEFAULT_PRIORITY, g_arrNodeAddr[1], 0x58) );
	Sleep(500);

	/* Send J1939 8 byte Data Message through monitor node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[0], 1, MSG_TYPE_DATA, g_arrRqstPGNs[0], bytJ1939Data, 8,
							DEFAULT_PRIORITY, g_arrNodeAddr[0], g_arrNodeAddr[1]) );
	
	/* Send J1939 8 byte Message through normal node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[1], 1, MSG_TYPE_DATA, g_arrRqstPGNs[1], bytJ1939Data, 8,
							DEFAULT_PRIORITY, g_arrNodeAddr[1], g_arrNodeAddr[0]) );

	///* Send J1939 10 byte Data Message through monitor node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[0], 1, MSG_TYPE_DATA, g_arrRqstPGNs[0], bytJ1939Data, 10,
							DEFAULT_PRIORITY, g_arrNodeAddr[0], g_arrNodeAddr[1]) );

	//Sleep(10);
	/* Send J1939  10 byte Data Message through normal node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[1], 1, MSG_TYPE_DATA, g_arrRqstPGNs[1], bytJ1939Data, 10,
							DEFAULT_PRIORITY, g_arrNodeAddr[1], g_arrNodeAddr[0]) );

	SYSTEMTIME objSysTime;
	UINT64 unAbsTime;

	BOOST_CHECK ( S_OK == DILJ_GetTimeModeMapping(objSysTime, unAbsTime) );

	/* Send Ack Messages */
	for ( int nChnlId = 1; nChnlId <= g_nChannelCount ; nChnlId++ )
	{
		/* Monitor node to Node1 */
		BOOST_CHECK ( S_OK == DILJ_SendAckMsg( g_dwJ1939ClientId[0], nChnlId, 
											   ACK_POS, g_arrRqstPGNs[1], g_arrNodeAddr[0], g_arrNodeAddr[1]) );

		/* Node1 to Monitor node */
		BOOST_CHECK ( S_OK == DILJ_SendAckMsg( g_dwJ1939ClientId[1], nChnlId, 
											   ACK_NEG, g_arrRqstPGNs[0], g_arrNodeAddr[1], g_arrNodeAddr[0]) );
	}

	/* Command Address - Success case */
	/* Monitor Node commanding ECU node to assume address - 0x15 */
	
	g_arrNodeAddr[1] = 0x15;
	for ( int nChnlId = 1; nChnlId <= g_nChannelCount ; nChnlId++ )
	{
		BOOST_CHECK ( S_OK == DILJ_NM_CommandAddress(g_dwJ1939ClientId[0], nChnlId, 
			                                         J1939_NODE_NAME, g_arrNodeAddr[1]));
	}

	/* Request Address from another node */
	DILJ_NM_RequestAddress(g_dwJ1939ClientId[0], 1, DEFAULT_PRIORITY,g_arrNodeAddr[0], g_arrNodeAddr[1]);
	DILJ_NM_RequestAddress(g_dwJ1939ClientId[1], 1, DEFAULT_PRIORITY,g_arrNodeAddr[1], g_arrNodeAddr[0]);
}


BOOST_AUTO_TEST_CASE( J1939_CAN_Closure_Activities )
{
	HRESULT hr = S_FALSE;
	Sleep(500);
	/*                      Go offline                       */
	BOOST_REQUIRE ( S_OK == DILJ_GoOffline() );

	/* Check if Offline - should be false */
	BOOST_REQUIRE ( FALSE == DILJ_bIsOnline() );

	/* Clear message buffer for monitor node*/
	BOOST_CHECK ( S_OK == DILJ_ManageMsgBuf(MSGBUF_CLEAR, g_dwJ1939ClientId[0], &g_ouVSEBufJ1939) );
	/* Try to clear it again */
	BOOST_CHECK ( S_OK == DILJ_ManageMsgBuf(MSGBUF_CLEAR, g_dwJ1939ClientId[1], &g_ouVSEBufJ1939) );
	/* Try to clear for an invalid Client ID */
	BOOST_CHECK ( ERROR_NOCLIENT == DILJ_ManageMsgBuf(MSGBUF_CLEAR, 0x255, &g_ouVSEBufJ1939) );

	/* Un-Register client - Montior node */	
	BOOST_REQUIRE ( S_OK == DILJ_RegisterClient(FALSE, 
                    J1939_MONITOR_NODE, J1939_ECU_NAME, g_arrNodeAddr[0], g_dwJ1939ClientId[0]) );

	/* Un-Register client - Normal J1939 node */	
	BOOST_REQUIRE ( S_OK == DILJ_RegisterClient(FALSE, 
                    _T("Node 1"), J1939_NODE_NAME, g_arrNodeAddr[1], g_dwJ1939ClientId[1]) );

	/* Uninitialize J1939 */
	BOOST_CHECK ( S_OK == DILJ_Uninitialise() );
	/********************************** J1939 Related activities - End  ***********************************/

	/********************************** CAN Closure activities  - Start ***********************************/
	/* Disconnect from CAN network */
	hr = g_pouDIL_CAN_Interface->DILC_StopHardware();
	/********************************** CAN Closure activities  - End  ***********************************/
}

BOOST_AUTO_TEST_CASE( J1939_Classes_And_Utility_Functions )
{
	BYTE bytDataAck[8]= {0x00};
	PrepareEOM_ACK(bytDataAck, 14, 2, 0x5500);

	BYTE bytDataCTS[8]= {0x00};
	PrepareClear_2_Send(bytDataCTS, 2, 1, 0xFDC5);

	BYTE bytDataCM[8]= {0x00};
	vPrepareData(bytDataCM, 0x16, 16, 0xFE05);

	bIsCommandAddress(PDU_FORMAT_ACL, DATA_LEN_CMD_ADDRESS);

	/* RQST message type */
	BYTE bytDataRQSTMsg[8] = {0x00, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	BOOST_CHECK ( MSG_TYPE_REQUEST == eGetMsgType(0x1CEA5800, bytDataRQSTMsg) );  	
	bytDataRQSTMsg[1] = PDU_FORMAT_ACL;	
	BOOST_CHECK ( MSG_TYPE_NM_RQST_ACL == eGetMsgType(0x1CEA5800, bytDataRQSTMsg) );  

	/* RTS message */
	BYTE bytDataMsg[8] = {0x10, 0x10, 0x00, 0x03, 0xFF, 0x00, 0xA8, 0x00};
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_RTS == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* BAM message */
	bytDataMsg[0] = CB_BAM;
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_BAM == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* CTS message */
	bytDataMsg[0] = CB_CLEAR_TO_SEND;
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_CTS == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* CON_ABORT message */
	bytDataMsg[0] = CB_CON_ABORT;
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_CON_ABORT == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* EOM ACK message */
	bytDataMsg[0] = CB_EOM_ACK;
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_EOM_ACK == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* CConnectionDet class testing */	
	CConnectionDet* pConDet = new CConnectionDet(0x05, 0x10);	
	BOOL bRet = pConDet->bIsMsgRxForThisConnection(0xCF004EE);
	pConDet->bIsMsgRxForThisConnection(0xCF004EE);

	/* CNodeConManager class testing */
	CNodeConManager* pNodeConMgr = NULL;
	pNodeConMgr = new CNodeConManager(0, J1939_MONITOR_NODE, 0x8000000000000025, 0x25);			
	pNodeConMgr->bAddConDetObj(pConDet);

	pNodeConMgr->vStartTimerFunction(1000);
	//pNodeConMgr->vSendRequestForPGN(0xFECA, 0xFF, 1);	//private function
	pNodeConMgr->vSendConAckMsg(0x05, 4, 0xD400, 1);
	//pNodeConMgr->vSendConAbortMsg(0xCD40005, 0x5, 1); //private function
	//pNodeConMgr->vSendACLMsg();	//private function	
	BYTE bytCount = pNodeConMgr->byGetConCount();

	pConDet = pNodeConMgr->pouGetConDetObj(bytCount-1);		
	if ( pConDet )
	{		
		STJ1939_MSG msgJ1939;
		pNodeConMgr->SendLongMsg(&msgJ1939, pConDet, DATA_CLEAR_2_SEND);		
	}	

	BOOL bStopped = pNodeConMgr->bStopTimerFunction();

	pNodeConMgr->WaitForMorePackets();
	pNodeConMgr->WaitAfterClear2Send();
	
	delete pConDet;
	pConDet = NULL;	

	/* CTransferLayer class testing */
	CTransferLayer::ouGetTransLayerObj().lTConnectReq(0,0,CM_BROADCAST);
	CTransferLayer::ouGetTransLayerObj().vTConnectCon(0, T_CONNECTED, 0, CM_STANDARD);
	CTransferLayer::ouGetTransLayerObj().vTConnectInd( 0, 0, 0);
	CTransferLayer::ouGetTransLayerObj().vTDisconnectInd(0, T_SHUTDOWN);
	CTransferLayer::ouGetTransLayerObj().vTLongDataCon(0, 'F');
	CTransferLayer::ouGetTransLayerObj().vTConTestCon( 0, 0, 0, 0);
	CTransferLayer::ouGetTransLayerObj().vTBroadDataInd( 0, 0, NULL);

}
BOOST_AUTO_TEST_SUITE_END()