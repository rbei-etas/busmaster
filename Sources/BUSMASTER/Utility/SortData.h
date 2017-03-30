
#include "IBMNetWorkService.h"
#include "CANDriverDefines.h"
#include "..\Application\Hashdefines.h"
#include <algorithm>
using namespace std;

enum EColumnType
{
    eNoColType = 0,
    eTimeStampCol,
    eTxRxCol,
    eChannelCol,
    eMsgTypeCol,
    eCANIDCol,
    eMsgNameCol,
    eMsgDLCCol
};

template <typename SBUFFERTYPE>
class CCANMsgWndDataHandler
{
private:
    static EColumnType  m_nSortField;
    static int  m_nMFactor;
    static IBMNetWorkGetService * m_pNetworkService;

    static void SetColumnField ( int nField )
    {
        switch ( nField )
        {
            case 1:
                m_nSortField = EColumnType::eTimeStampCol;
                break;
            case 2:
                m_nSortField = EColumnType::eTxRxCol;
                break;
            case 3:
                m_nSortField = EColumnType::eChannelCol;
                break;
            case 4:
                m_nSortField = EColumnType::eMsgTypeCol;
                break;
            case 5:
                m_nSortField = EColumnType::eCANIDCol;
                break;
            case 6:
                m_nSortField = EColumnType::eMsgNameCol;
                break;
            case 7:
                m_nSortField = EColumnType::eMsgDLCCol;
                break;
            default:
                m_nSortField = EColumnType::eNoColType;
                break;
        }
    }
    

public:
    CCANMsgWndDataHandler ()
    {
        this->m_nSortField = EColumnType::eNoColType;
        m_nMFactor = -1;
        m_pNetworkService = nullptr;
    }
    static void SortBufferData ( int nField, bool bAscending, IBMNetWorkGetService * pNetworkService, SBUFFERTYPE * bufferData, int nCntOfDataEntries );
    static int CompareBufferData ( const void* pEntry1, const void* pEntry2 );
};

template<typename SBUFFERTYPE>
EColumnType  CCANMsgWndDataHandler<SBUFFERTYPE>::m_nSortField = EColumnType::eNoColType;
template<typename SBUFFERTYPE>
int  CCANMsgWndDataHandler<SBUFFERTYPE>::m_nMFactor = -1;
template<typename SBUFFERTYPE>
IBMNetWorkGetService * CCANMsgWndDataHandler<SBUFFERTYPE>::m_pNetworkService = nullptr;


template<typename SBUFFERTYPE>
void CCANMsgWndDataHandler<SBUFFERTYPE>::SortBufferData ( int nField, bool bAscending, IBMNetWorkGetService * pNetworkService, SBUFFERTYPE * bufferData, int nCntOfDataEntries )
{
    m_pNetworkService = pNetworkService;
    SetColumnField ( nField );
    m_nMFactor = bAscending ? 1 : -1;
    size_t tLenOfBuff = (size_t)nCntOfDataEntries;
    
    qsort ( (void*) bufferData, tLenOfBuff,
            sizeof ( SBUFFERTYPE ), CCANMsgWndDataHandler<SBUFFERTYPE>::CompareBufferData );
}

template<typename SBUFFERTYPE>
int CCANMsgWndDataHandler<SBUFFERTYPE>::CompareBufferData ( const void* pEntry1, const void* pEntry2 )
{
    int nRetValue = 0;

    STCANDATA* pCANDataEntry1 = (STCANDATA*) pEntry1;
    STCANDATA* pCANDataEntry2 = (STCANDATA*) pEntry2;
    
    switch ( m_nSortField )
    {
        //case EColumnType::eMsgDLCCol: // Sort by DLC
        //{
        //    nRetValue = (int) (pCANDataEntry1->m_uDataInfo.m_sCANMsg.m_ucDataLen - pCANDataEntry2->m_uDataInfo.m_sCANMsg.m_ucDataLen);
        //    break;
        //}
        case EColumnType::eMsgNameCol: // Sort by message name
        {
            CString msgName1, msgName2;
            IFrame* pouFrame = nullptr;
            ERRORCODE eResult = m_pNetworkService->GetFrame ( CAN, 0, pCANDataEntry1->m_uDataInfo.m_sCANMsg.m_unMsgID, nullptr, &pouFrame );
            if ( EC_SUCCESS == eResult && nullptr != pouFrame )
            {
                std::string strName;
                pouFrame->GetName ( strName );
                msgName1 = strName.c_str();
            }

            pouFrame = nullptr;
            eResult = m_pNetworkService->GetFrame ( CAN, 0, pCANDataEntry2->m_uDataInfo.m_sCANMsg.m_unMsgID, nullptr, &pouFrame );
            if ( EC_SUCCESS == eResult && nullptr != pouFrame )
            {
                std::string strName;
                pouFrame->GetName ( strName );
                msgName2 = strName.c_str();
            }

            nRetValue = msgName1.CompareNoCase ( msgName2 );
            break;
        }
        case EColumnType::eCANIDCol: // Sort by CAN id
        {
            nRetValue = (int) (pCANDataEntry1->m_uDataInfo.m_sCANMsg.m_unMsgID - pCANDataEntry2->m_uDataInfo.m_sCANMsg.m_unMsgID);
            break;
        }
        //case EColumnType::eMsgTypeCol: // Sort by Message Type
        //{
        //    nRetValue = (int) (pCANDataEntry1->m_uDataInfo.m_sCANMsg.m_ucEXTENDED - pCANDataEntry2->m_uDataInfo.m_sCANMsg.m_ucEXTENDED);


        //    break;
        //}
        case EColumnType::eChannelCol: // Sort by channel
        {
            nRetValue = (int) (pCANDataEntry1->m_uDataInfo.m_sCANMsg.m_ucChannel - pCANDataEntry2->m_uDataInfo.m_sCANMsg.m_ucChannel);
            break;
        }
        //case EColumnType::eTxRxCol: // Sort by Tx/Rx column
        //{
        //    nRetValue = (int) (IS_TX_MESSAGE ( pCANDataEntry1->m_ucDataType ) - IS_TX_MESSAGE ( pCANDataEntry2->m_ucDataType ));
        //    break;
        //}
        case EColumnType::eTimeStampCol: // Sort by time stamp
        {
            nRetValue = (int) (pCANDataEntry1->m_lTickCount.QuadPart - pCANDataEntry2->m_lTickCount.QuadPart);
        }
        break;
        default:
        {
            ASSERT ( false );
        }
        break;
    }

    nRetValue *= m_nMFactor;
    return nRetValue;
}


//class CLINMsgWndDataHandler
//{
//
//};
//
//class CJ1939MsgWndDataHandler
//{
//
//};