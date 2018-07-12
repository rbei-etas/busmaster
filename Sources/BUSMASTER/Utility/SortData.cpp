//#include "Utils_stdafx.h"
//#include "SortData.h"
//
//template<typename SBUFFERTYPE>
//void CCANMsgWndDataHandler<SBUFFERTYPE>::SortBufferData ( int nField, bool bAscending, IBMNetWorkGetService * pNetworkService, SBUFFERTYPE * bufferData )
//{
//    m_pNetworkService = pNetworkService;
//    SetColumnField(nField);
//    m_nMFactor = bAscending ? 1 : -1;
//
//    qsort ( (void*) bufferData, (size_t) SBUFFERTYPE::GetBufferLength (),
//            sizeof ( SBUFFERTYPE ), CCANMsgWndDataHandler::CompareBufferData );
//
//}
//
//
//template<typename SBUFFERTYPE>
//int CCANMsgWndDataHandler<SBUFFERTYPE>::CompareBufferData ( const void* pEntry1, const void* pEntry2 )
//{
//    int nRetValue = 0;
//
//    STCANDATA* pDatCAN1 = (STCANDATA*) pEntry1;
//    STCANDATA* pDatCAN2 = (STCANDATA*) pEntry2;
//
//    switch ( m_nSortField )
//    {
//        case EColumnType::eMsgDLCCol: // Sort by DLC
//        {
//            nRetValue = (int) (pDatCAN1->m_uDataInfo.m_sCANMsg.m_ucDataLen - pDatCAN2->m_uDataInfo.m_sCANMsg.m_ucDataLen);
//            break;
//        }
//        case EColumnType::eMsgNameCol: // Sort by message name
//        {
//            /*CString str1, str2;
//            AfxGetMainWnd ()->SendMessage ( WM_GET_MSG_NAME_FROM_CODE, (WPARAM) pDatCAN1->m_uDataInfo.m_sCANMsg.m_unMsgID, (LPARAM) &str1 );
//            AfxGetMainWnd ()->SendMessage ( WM_GET_MSG_NAME_FROM_CODE, (WPARAM) pDatCAN2->m_uDataInfo.m_sCANMsg.m_unMsgID, (LPARAM) &str2 );
//
//            nRetValue = (int) (str1.CompareNoCase ( str2 ));*/
//            
//            break;
//        }
//        case EColumnType::eCANIDCol: // Sort by CAN id
//        {
//            nRetValue = (int) (pDatCAN1->m_uDataInfo.m_sCANMsg.m_unMsgID - pDatCAN2->m_uDataInfo.m_sCANMsg.m_unMsgID);
//            break;
//        }
//        case EColumnType::eMsgTypeCol: // Sort by Message Type
//        {
//            nRetValue = (int) (pDatCAN1->m_uDataInfo.m_sCANMsg.m_ucEXTENDED - pDatCAN2->m_uDataInfo.m_sCANMsg.m_ucEXTENDED);
//            
//            
//            break;
//        }
//        case EColumnType::eChannelCol: // Sort by channel
//        {
//            nRetValue = (int) (pDatCAN1->m_uDataInfo.m_sCANMsg.m_ucChannel - pDatCAN2->m_uDataInfo.m_sCANMsg.m_ucChannel);
//            break;
//        }
//        case EColumnType::eTxRxCol: // Sort by Tx/Rx column
//        {
//            nRetValue = (int) (IS_TX_MESSAGE ( pDatCAN1->m_ucDataType ) - IS_TX_MESSAGE ( pDatCAN2->m_ucDataType ));
//            break;
//        }
//        case EColumnType::eTimeStampCol: // Sort by time stamp
//        {
//            nRetValue = (int) (pDatCAN1->m_lTickCount.QuadPart - pDatCAN2->m_lTickCount.QuadPart);
//        }
//        break;
//        default:
//        {
//            ASSERT ( false );
//        }
//        break;
//    }
//
//    nRetValue *= m_nMFactor;
//    return nRetValue;
//}
//
//
//template<typename SBUFFERTYPE>
//__int64 CCANMsgWndDataHandler<SBUFFERTYPE>::GetSlotID ( STCANDATA& pDatCAN )
//{
//    STCAN_MSG& sMsg = pDatCAN.m_uDataInfo.m_sCANMsg;
//    // Form message to get message index in the CMap
//    int nMsgID = MAKE_RX_TX_MESSAGE ( sMsg.m_unMsgID,
//                                      IS_RX_MESSAGE ( pDatCAN.m_ucDataType ) );
//
//    nMsgID = MAKE_DEFAULT_MESSAGE_TYPE ( nMsgID );
//    // For extended message
//    if ( sMsg.m_ucEXTENDED )
//    {
//        nMsgID = MAKE_EXTENDED_MESSAGE_TYPE ( nMsgID );
//    }
//    // Apply Channel Information
//    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE ( nMsgID,
//                                                          sMsg.m_ucChannel );
//    return n64MapIndex;
//}
