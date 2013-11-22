#include "DataTypes_stdafx.h"
#include "Cluster.h"

bool Compare_Frame_Structs(FRAME_STRUCT& ob1, FRAME_STRUCT& ob2 )
{
    return (ob1.m_strFrameId== ob2.m_strFrameId);
}

Cluster::Cluster(void)
{
    Clear();
}


Cluster::~Cluster(void)
{
}

void Cluster::Clear()
{
    m_strName = "";
    m_omClusterID = "";
    m_mapChnls.clear();
    m_ouClusterInfo.DoCleanup();
    m_ouEcuList.clear();
    m_mapSlotEcu.clear();
}
//Cluster
HRESULT Cluster::GetClusterInfo( ABS_FLEXRAY_CLUSTER& ouClusterInfo )
{
    ouClusterInfo = m_ouClusterInfo;
    return S_OK;
}
HRESULT Cluster::GetECU(string omECUStrId, ECU_Struct& ouEcu)
{
    ECUMAP::iterator ecuIterator = m_ouEcuList.find(omECUStrId);
    if ( ecuIterator  != m_ouEcuList.end() )
    {
        ouEcu = ecuIterator->second;
    }

    return S_OK;
}

HRESULT Cluster::GetECUList( list<ECU_Struct>& ouEcuList )
{
    map<ECU_ID, ECU_Struct>::iterator itrEcu;
    if ( m_ouEcuList.size() > 0 )
    {
        ouEcuList.clear();
        for ( itrEcu = m_ouEcuList.begin() ; itrEcu != m_ouEcuList.end(); itrEcu++ )
        {
            ouEcuList.push_back(itrEcu->second);
        }
    }
    return S_OK;
}
HRESULT Cluster::GetFrameList( string omStrEcuName, list<FRAME_STRUCT>& ouFrameList )
{
    return S_OK;
}

HRESULT Cluster::GetTxFrameList( string omStrEcuName, list<FRAME_STRUCT>& ouFrameList )
{
    map<ECU_ID, ECU_Struct>::iterator itrEcu = m_ouEcuList.find(omStrEcuName);
    if ( m_ouEcuList.end() != itrEcu )
    {
        return itrEcu->second.GetFrameList(ouFrameList, DIR_TX);
    }
    return S_FALSE;
}

HRESULT Cluster::GetPDUList ( SLOT_BASECYCLE ouSlotBaseKey, list<PDU_STRUCT>& ouPduList )
{
    return S_OK;
}
HRESULT Cluster::GetSignalList ( SLOT_BASECYCLE ouSlotBaseKey, list<SIGNAL_STRUCT>& ouSignalList )
{
    return S_OK;
}
HRESULT Cluster::GetEcuChannel( ECHANNEL& ouChannelType )
{
    return S_OK;
}

//ECU
HRESULT ECU_Struct::GetControllerParams(ABS_FLEXRAY_SPEC_CNTLR& ouControllerParams)
{
    ouControllerParams = m_ouControllerParams;
    return S_OK;
}
HRESULT ECU_Struct::GetFrame(UINT unSlotId, UINT nCycleNumber, ECHANNEL& oeChannel, FRAME_STRUCT& ouFrame)
{
    HRESULT hResult = S_FALSE;
    map<UINT, list<FRAME_STRUCT>>::iterator itrFrame = m_ouTxFrames.find(unSlotId);
    if( itrFrame != m_ouTxFrames.end() )
    {
        list<FRAME_STRUCT>::iterator itrFrameStruct = itrFrame->second.begin();
        while ( itrFrameStruct != itrFrame->second.end() )
        {
            if (
                itrFrameStruct->m_nSlotId == unSlotId   &&
                ( nCycleNumber % itrFrameStruct->m_nReptition == itrFrameStruct->m_nBaseCycle )
            )
            {
                ouFrame = *itrFrameStruct;
                return S_OK;
            }
            itrFrameStruct++;
        }
        itrFrame++;
    }
    //For Rx
    {
        map<UINT, list<FRAME_STRUCT>>::iterator itrFrame = m_ouRxFrames.find(unSlotId);
        while( itrFrame != m_ouRxFrames.end() )
        {
            list<FRAME_STRUCT>::iterator itrFrameStruct = itrFrame->second.begin();
            while ( itrFrameStruct != itrFrame->second.end() )
            {
                if ( itrFrameStruct->m_ouChannel == oeChannel &&
                        itrFrameStruct->m_nSlotId == unSlotId   &&
                        ( nCycleNumber % itrFrameStruct->m_nReptition == itrFrameStruct->m_nBaseCycle )
                   )
                {
                    ouFrame = *itrFrameStruct;
                    return S_OK;
                }
                itrFrameStruct++;
            }
            itrFrame++;
        }
    }


    return hResult;
}


HRESULT ECU_Struct::GetFrameList( list<FRAME_STRUCT>& ouFrameList, EDIRECTION ouEDIRECTION )
{
    HRESULT hResult = S_FALSE;
    list<FRAME_STRUCT> ouFrameTempList;
    map<SLOT, list<FRAME_STRUCT> >::iterator itrMapList;
    if ( DIR_TX == ouEDIRECTION )
    {
        itrMapList = m_ouTxFrames.begin();
        for ( ; itrMapList != m_ouTxFrames.end(); itrMapList++)
        {
            for (  list<FRAME_STRUCT>::iterator itrFrameList = itrMapList->second.begin(); itrFrameList != itrMapList->second.end(); itrFrameList++ )
            {
                ouFrameTempList.push_back(*itrFrameList);
            }
        }
        hResult = S_OK;
    }
    else if ( DIR_RX == ouEDIRECTION )
    {
        itrMapList = m_ouRxFrames.begin();
        for ( ; itrMapList != m_ouRxFrames.end(); itrMapList++)
        {
            for (  list<FRAME_STRUCT>::iterator itrFrameList = itrMapList->second.begin(); itrFrameList != itrMapList->second.end(); itrFrameList++ )
            {
                ouFrameTempList.push_back(*itrFrameList);
            }
        }
        hResult = S_OK;
    }

    int nCount = 0;
    list<FRAME_STRUCT>::iterator itrFrame = ouFrameTempList.begin();
    while( itrFrame != ouFrameTempList.end() && nCount < 128 )
    {
        if ( itrFrame->m_eSlotType == STATIC )
        {
            ouFrameList.push_back(*itrFrame);
            nCount++;
        }
        itrFrame++;
    }

    itrFrame = ouFrameTempList.begin();
    while( itrFrame != ouFrameTempList.end() && nCount < 128 )
    {
        if ( itrFrame->m_eSlotType == DYNAMIC )
        {
            ouFrameList.push_back(*itrFrame);
            nCount++;
        }
        itrFrame++;
    }
    return hResult;
}
HRESULT ECU_Struct::GetPDUList ( SLOT_BASECYCLE ouSlotBaseKey, list<PDU_STRUCT>& ouPduList )
{
    return S_OK;
}
HRESULT ECU_Struct::GetSignalList ( SLOT_BASECYCLE ouSlotBaseKey, list<SIGNAL_STRUCT>& ouSignalList )
{
    return S_OK;
};

//Frame
HRESULT FRAME_STRUCT::GetSignalList ( string omStrPduName, list<SIGNAL_STRUCT>& ouSignalList )
{
    return S_OK;
}
HRESULT FRAME_STRUCT::GetPDUList ( list<SIGNAL_STRUCT>& ouSignalList )
{
    return S_OK;
}
HRESULT FRAME_STRUCT::GetSignalList ( list<SIGNAL_STRUCT>& ouSignalList )
{
    list<PDU_STRUCT>::iterator itrPdu = m_ouPduList.begin();

    while(itrPdu != m_ouPduList.end())
    {
        std::copy(itrPdu->m_ouSignalList.begin(), itrPdu->m_ouSignalList.end(),
                  std::back_insert_iterator<std::list<SIGNAL_STRUCT> >(ouSignalList));

        itrPdu++;
    }

    return S_OK;
};
HRESULT FRAME_STRUCT::GetSignalCount ( int& nCount)
{
    list<PDU_STRUCT>::iterator itrPdu = m_ouPduList.begin();
    nCount = 0;
    while(itrPdu != m_ouPduList.end())
    {
        nCount += itrPdu->m_ouSignalList.size();
        itrPdu++;
    }
    return S_OK;
}


//PDU
HRESULT PDU_STRUCT::GetSignalList ( list<SIGNAL_STRUCT>& ouSignalList )
{
    return S_OK;
}

SIGNAL_STRUCT::SIGNAL_STRUCT()
{
    //  m_ouSigConstrnt.RemoveAll();
    //  m_ouDescriptorMap.clear();
}

SIGNAL_STRUCT::SIGNAL_STRUCT(const SIGNAL_STRUCT& objRef)
{
    //  m_omSigId = objRef.m_omSigId;
    m_unStartbit = objRef.m_unStartbit;
    m_strSignalName = objRef.m_strSignalName;
    m_nLength = objRef.m_nLength;
    m_omUnit = objRef.m_omUnit;
    //m_omPhylVal = objRef.m_omPhylVal;
    m_bDataType = objRef.m_bDataType;
    //m_ouSigConstrnt.RemoveAll();

    //m_ouSigConstrnt.Append(objRef.m_ouSigConstrnt);
    m_ouCompuMethod = objRef.m_ouCompuMethod;
    m_ouEndianness = objRef.m_ouEndianness;
    //m_ouDescriptorMap = objRef.m_ouDescriptorMap;
}

SIGNAL_STRUCT::~SIGNAL_STRUCT()
{

}

SIGNAL_STRUCT& SIGNAL_STRUCT::operator=(const SIGNAL_STRUCT& objRef)
{
    //m_omSigId = objRef.m_omSigId;
    m_unStartbit = objRef.m_unStartbit;
    m_strSignalName = objRef.m_strSignalName;
    m_nLength = objRef.m_nLength;
    m_omUnit = objRef.m_omUnit;
    //m_omPhylVal = objRef.m_omPhylVal;
    m_bDataType = objRef.m_bDataType;
    //m_ouSigConstrnt.RemoveAll();
    //m_ouSigConstrnt.Append(objRef.m_ouSigConstrnt);
    m_ouCompuMethod = objRef.m_ouCompuMethod;
    m_ouEndianness = objRef.m_ouEndianness;
    //  m_ouDescriptorMap = objRef.m_ouDescriptorMap;
    return *this;
}
