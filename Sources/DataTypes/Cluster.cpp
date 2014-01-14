#include "DataTypes_stdafx.h"
#include "Cluster.h"
#include "../Application/HashDefines.h"

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
    HRESULT hResult = S_FALSE;
    map<ECU_ID, ECU_Struct>::iterator itrEcu = m_ouEcuList.find(omStrEcuName);
    if ( m_ouEcuList.end() != itrEcu )
    {
        itrEcu->second.GetFrameList(ouFrameList);
        hResult = S_OK;
    }
    return S_OK;
}

HRESULT Cluster::GetFrames(list<FRAME_STRUCT>& ouFrameList)
{
    if(m_ouEcuList.size() <= 0)
    {
        return S_FALSE;
    }

    ECUMAP::iterator itrEcuList;
    list<FRAME_STRUCT> ouFrameTempList;
    for ( itrEcuList = m_ouEcuList.begin(); itrEcuList != m_ouEcuList.end(); itrEcuList++ )
    {
        GetFrameList(itrEcuList->first.c_str(), ouFrameTempList);
    }
    ouFrameTempList.sort();
    ouFrameTempList.unique();
    ouFrameTempList.erase (unique (ouFrameTempList.begin(), ouFrameTempList.end(), Compare_Frame_Structs ), ouFrameTempList.end());  //  2.72,  3.14, 12.15


    //TODO::
    int nCount = 0;
    list<FRAME_STRUCT>::iterator itrFrame = ouFrameTempList.begin();
    while( itrFrame != ouFrameTempList.end())
    {
        ouFrameList.push_back(*itrFrame);
        nCount++;

        itrFrame++;
    }

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

HRESULT Cluster::GetFrameNames(string omStrEcuName, list<string>& lstFrames)
{
    map<ECU_ID, ECU_Struct>::iterator itrEcu = m_ouEcuList.find(omStrEcuName);
    if ( m_ouEcuList.end() != itrEcu )
    {
        return itrEcu->second.GetFrameNames(lstFrames);
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

HRESULT ECU_Struct::GetFrame(UINT unSlotId, FRAME_STRUCT& ouFrame)
{
    HRESULT hResult = S_FALSE;
    map<UINT, list<FRAME_STRUCT>>::iterator itrFrame = m_ouTxFrames.find(unSlotId);
    if( itrFrame != m_ouTxFrames.end() )
    {
        list<FRAME_STRUCT>::iterator itrFrameStruct = itrFrame->second.begin();
        while ( itrFrameStruct != itrFrame->second.end() )
        {
            if ( itrFrameStruct->m_nSlotId == unSlotId )
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
                if ( itrFrameStruct->m_nSlotId == unSlotId )
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

HRESULT ECU_Struct::GetFrameNames( list<string>& ouFrameTempList)
{
    HRESULT hResult = S_OK;
    map<SLOT, list<FRAME_STRUCT> >::iterator itrMapList;

    itrMapList = m_ouTxFrames.begin();
    for ( ; itrMapList != m_ouTxFrames.end(); itrMapList++)
    {
        for (  list<FRAME_STRUCT>::iterator itrFrameList = itrMapList->second.begin(); itrFrameList != itrMapList->second.end(); itrFrameList++ )
        {
            ouFrameTempList.push_back(itrFrameList->m_strFrameName);
        }
    }

    itrMapList = m_ouRxFrames.begin();
    for ( ; itrMapList != m_ouRxFrames.end(); itrMapList++)
    {
        for (  list<FRAME_STRUCT>::iterator itrFrameList = itrMapList->second.begin(); itrFrameList != itrMapList->second.end(); itrFrameList++ )
        {
            ouFrameTempList.push_back(itrFrameList->m_strFrameName);
        }
    }

    return hResult;
}

HRESULT ECU_Struct::GetFrameList( list<FRAME_STRUCT>& ouFrameTempList)
{
    HRESULT hResult = S_OK;
    map<SLOT, list<FRAME_STRUCT> >::iterator itrMapList;

    itrMapList = m_ouTxFrames.begin();
    for ( ; itrMapList != m_ouTxFrames.end(); itrMapList++)
    {
        for (  list<FRAME_STRUCT>::iterator itrFrameList = itrMapList->second.begin(); itrFrameList != itrMapList->second.end(); itrFrameList++ )
        {
            ouFrameTempList.push_back(*itrFrameList);
        }
    }

    itrMapList = m_ouRxFrames.begin();
    for ( ; itrMapList != m_ouRxFrames.end(); itrMapList++)
    {
        for (  list<FRAME_STRUCT>::iterator itrFrameList = itrMapList->second.begin(); itrFrameList != itrMapList->second.end(); itrFrameList++ )
        {
            ouFrameTempList.push_back(*itrFrameList);
        }
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

HRESULT FRAME_STRUCT::GetSignalNames (CStringList& ouSignalList )
{
    list<PDU_STRUCT>::iterator itrPdu = m_ouPduList.begin();

    while(itrPdu != m_ouPduList.end())
    {
        list<SIGNAL_STRUCT>::iterator itrSiglst = itrPdu->m_ouSignalList.begin();

        while(itrSiglst != itrPdu->m_ouSignalList.end())
        {
            ouSignalList.AddTail(itrSiglst->m_strSignalName.c_str());
            itrSiglst++;
        }

        itrPdu++;
    }

    return S_OK;
}

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
    m_unDefaultVal  = objRef.m_unDefaultVal;
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
    m_unDefaultVal  = objRef.m_unDefaultVal;
    //  m_ouDescriptorMap = objRef.m_ouDescriptorMap;
    return *this;
}


/*******************************************************************************
  Function Name    :  bWriteDBHeader
  Input(s)         :    -
  Output           :  TRUE/FALSE
  Functionality    :  Creates a "Unions.h" file and adds union defintions
                      to it.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  Amitesh Bharti, 12.03.2003
                      changes made to create unions.h structures as per
                      CRH0002
  Modifications    :  Raja N 12.02.2004
                      Modified to include Sign check for signals and moved
                      some hardcoded strings to Hashdefines.h
  Modifications    :  Raja N 18.05.2004
                      Added two more members in the union to give word and long
                      access of the data array
  Modifications    :  Raja N 18.05.2004
                      Added two more members in the union to give word and long
                      access of the data array
  Modifications    :  Anish 21.12.2006
                      Added code to have header name as header file name in
                      ifndef condition at the begining of *_UNIONS.h file
*******************************************************************************/
CString Cluster::bWriteDBHeader(CString omStrActiveDataBase, ETYPE_BUS eBus)
{
    BOOL bRetVal                = TRUE;
    CString omStrPath           = EMPTY_STRING;
    //Add header for ifndef condition
    CString omStrHeaderString   = EMPTY_STRING;
    CString omStrHeaderFileName = defHEADER_FILE_NAME;
    char  acErrorMsg[defSIZE_OF_ERROR_BUFFER];
    CFileException   omException  ;
    CStdioFile omHeaderFile;
    omStrHeaderFileName = omStrActiveDataBase.Left(
                              omStrActiveDataBase.ReverseFind('.') );
    omStrHeaderString = omStrHeaderFileName;
    omStrHeaderFileName += defHEADER_FILE_NAME;
    TRY
    {
        // Open HeaderFile
        bRetVal = omHeaderFile.Open( omStrHeaderFileName,
        CFile::modeCreate|CFile::modeWrite|CFile::typeText,&omException);
        if (bRetVal == FALSE)
        {
            // Get the exception error message
            omException.GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
            // union.h file  open error notification
            // AfxMessageBox and MessageBox does not work here. So the API
            // call for MessageBox is used.
            if (m_bAutoServerMode == FALSE)
            {
                ::MessageBox(NULL,acErrorMsg,("BUSMASTER") ,MB_ICONERROR|MB_OK);
            }
        }
        else
        {
            int nIndex = omStrHeaderString.ReverseFind(defCHAR_PATH_SEPRATER);
            int nLength = omStrHeaderString.GetLength();
            omStrHeaderString = omStrHeaderString.Right(nLength - nIndex -1);
            omStrHeaderString.MakeUpper();
            CString omStrTemp;
            omStrTemp.Format(H_FILE_HEADER_START,omStrHeaderString,omStrHeaderString);

            // Add Header "Help information"
            omHeaderFile.WriteString(defHELP_INFO_IN_UNIONS);

            // Add Header "ifndef..."
            omHeaderFile.WriteString(omStrTemp);

            // Add Pragma Pack
            omStrTemp.Format(H_FILE_HEADER_PRAGMA_PACK,omStrHeaderString,omStrHeaderString);
            omHeaderFile.WriteString(omStrTemp);

            CString omStrDLC = STR_EMPTY;
            UINT aunSigStartBit[defMAX_SIGNALS] ;
            UINT aunLength[defMAX_SIGNALS] ;
            CStringArray omStrArraySigName;
            omStrArraySigName.RemoveAll();
            BOOL bReturn = FALSE;
            UINT unSigCount = 0;
            CString omStrcommandLine = STR_EMPTY;
            CString omStrSigName = STR_EMPTY;
            CString omStrdelimiter = STR_EMPTY;

            list<FRAME_STRUCT> ouFrameList;
            GetFrames(ouFrameList );
            for ( list<FRAME_STRUCT>::iterator unMsgIndex = ouFrameList.begin();
            unMsgIndex != ouFrameList.end();
            unMsgIndex++)
            {
                // Get all signal names.
                // signal name will be the variable name
                // of the union of length specified in DB
                unSigCount = 0;
                list<SIGNAL_STRUCT> sigList;
                unMsgIndex->GetSignalList(sigList);
                //sSIGNALS* pSg = m_psMessages[unMsgIndex].m_psSignals;
                list<SIGNAL_STRUCT>::iterator itrSig = sigList.begin();
                while(itrSig != sigList.end())
                {
                    //UINT nSize = omStrArraySigName.GetSize();
                    /*aunSigStartBit[unSigCount] =
                    (itrSig->m_unStartByte - 1 ) * defBITS_IN_BYTE;*/
                    aunSigStartBit[unSigCount] = itrSig->m_unStartbit;
                    unSigCount++;
                    //pSg = pSg->m_psNextSignalList;
                    itrSig++;
                }
                // Check if there is no signal add one signal declaration
                // with no name occuppying whole message length.
                if(sigList.size() > 0 )
                {
                    bReturn =
                        bSortSignalStartBitAscend(aunSigStartBit,
                                                  sigList.size());
                    if(bReturn == TRUE )
                    {
                        switch (eBus)
                        {
                            case LIN: // TODO: LIN
                            {
                                bReturn = bFormSigNameAndLength(aunLength,
                                                                aunSigStartBit,
                                                                omStrArraySigName,
                                                                unMsgIndex);
                            }
                            break;
                            default: //This is a general routine which contructs
                                //structure based on message length and its signal length
                            {
                                /* bReturn = bFormSigNameAndLengthJ1939(aunSigStartBit,
                                                                      omStrArraySigName,
                                                                      unMsgIndex);*/
                            }
                            break;
                        }
                    }
                }
                else
                {
                    CString omFormatString;
                    // If message length is more then four byte
                    // define two signal with no name.
                    UINT unTempLen = unMsgIndex->m_nLength;
                    while (unTempLen > defUINT_SIZE)
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              STR_EMPTY,
                                              defUINT_LENGTH);
                        omStrArraySigName.Add(omFormatString);
                        unSigCount++;
                        unTempLen -= defUINT_SIZE;
                    }
                    UINT unLength = unMsgIndex->m_nLength
                                    * defBITS_IN_BYTE
                                    - ( unSigCount * defUINT_LENGTH );
                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defUNSIGNED_INTEGER,
                                          STR_EMPTY,
                                          unLength);
                    omStrArraySigName.Add(omFormatString);

                    unSigCount = 0;
                    bReturn = TRUE;
                }
                //                INT nIndex = 0;
                if(bReturn == TRUE )
                {
                    bInsertBusSpecStructures(omHeaderFile,
                                             omStrcommandLine,
                                             omStrArraySigName,
                                             unMsgIndex, LIN);
                }
            }

            // add "#endif.."
            //omHeaderFile.WriteString(H_FILE_HEADER_END);

            omStrPath = omStrHeaderFileName;
            // Close opened file
            omHeaderFile.Close();
        }
    }
    CATCH_ALL(pomE)
    {
        if(pomE != NULL )
        {
            pomE->GetErrorMessage(acErrorMsg ,sizeof(acErrorMsg) );
            bRetVal = FALSE;
            pomE->Delete();
            // union.h file  open error notification
            if (m_bAutoServerMode == FALSE)
            {
                ::MessageBox(NULL,acErrorMsg,("BUSMASTER") ,MB_ICONERROR|MB_OK);
            }
        }
        // Close opened file
        omHeaderFile.Close();
    }
    END_CATCH_ALL

    return omStrPath;
}

/******************************************************************************
  Function Name    :  bInsertBusSpecStructures

  Input(s)         :  CStdioFile& omHeaderFile : Header file name
                      CString& omStrcommandLine : command line arguments
                      CStringArray& omStrArraySigName : Signal names array
                      list<FRAME_STRUCT>::iterator itrFrame : Frame structure
                      ETYPE_BUS eBUS : Bus type

  Output           :  BOOL : Return value
******************************************************************************/
BOOL Cluster::bInsertBusSpecStructures(CStdioFile& omHeaderFile,
                                       CString& omStrcommandLine,
                                       CStringArray& omStrArraySigName,
                                       list<FRAME_STRUCT>::iterator itrFrame, ETYPE_BUS eBUS)
{
    CString omStrSigName     = STR_EMPTY;
    //    INT nIndex           = 0;
    CString omStrdelimiter   = STR_EMPTY;

    /* Add New lines */
    omHeaderFile.WriteString(NEW_LINE);
    omHeaderFile.WriteString(NEW_LINE);

    CString strFrameName = itrFrame->m_strFrameName.c_str();
    /* Include guards for each message added */
    omStrcommandLine.Format(defMSG_GUARD_CHECK,  strFrameName);
    omHeaderFile.WriteString(omStrcommandLine);

    omStrcommandLine.Format(defMSG_GUARD_DEFINE, strFrameName);
    omHeaderFile.WriteString(omStrcommandLine);

    switch (eBUS)
    {
        case LIN: // TODO: LIN
        {
            omStrcommandLine = defUNION;
            omHeaderFile.WriteString(omStrcommandLine);

            omHeaderFile.WriteString(defDATA_BYTE_ARRAY_LIN);
            omHeaderFile.WriteString(defDATA_WORD_ARRAY);
            omHeaderFile.WriteString(defDATA_LONG_ARRAY);

            omStrcommandLine = defSTART_OF_STRUCT_DEF;

            // Write the Structure Header
            omHeaderFile.WriteString(omStrcommandLine);

            UINT unCountSig = omStrArraySigName.GetSize();

            for(UINT i = 0 ; i < unCountSig; i++ )
            {
                // Copy Structure Members
                omHeaderFile.WriteString(omStrArraySigName.GetAt(i));
            }
            // Remove all signal name
            omStrArraySigName.RemoveAll();

            omStrcommandLine.Format(defEND_OF_UNION_DEF,
                                    strFrameName );

            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defTYPEDEF_STRUCT;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defMSG_TYPE;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defCHECKSUM;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defDATATYPE_DLC;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defDATATYPE_MSGID_LIN;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine.Format(defDATATYPE_MSG_DATA,
                                    strFrameName +
                                    defUNDERSCORE, defSIGNALMEMBER);

            omHeaderFile.WriteString(omStrcommandLine);

            omHeaderFile.WriteString(defDATATYPE_TIMESTAMP);

            //Adding the channel number to the message structures.
            omStrcommandLine = defDATATYPE_CHANNEL_NUM ;
            omHeaderFile.WriteString( omStrcommandLine );

            omStrcommandLine = defCHECKSUMVAL ;
            omHeaderFile.WriteString( omStrcommandLine );


            /*omStrcommandLine = defDATATYPE_FD_FLAG;
            omHeaderFile.WriteString(omStrcommandLine);*/

            omStrcommandLine.Format(defEND_OF_STRUCT_DEF,
                                    strFrameName );
            omHeaderFile.WriteString(omStrcommandLine);
        }
        break;
        case J1939:
        {
            omStrcommandLine = defUNION;
            omHeaderFile.WriteString(omStrcommandLine);

            CString omDataBytes;
            omDataBytes.Format(defDATA_BYTE_ARRAY_J1939, itrFrame->m_nLength);
            omHeaderFile.WriteString(omDataBytes);

            omStrcommandLine = defSTART_OF_STRUCT_DEF;

            // Write the Structure Header
            omHeaderFile.WriteString(omStrcommandLine);

            UINT unCountSig = omStrArraySigName.GetSize();

            for(UINT i = 0 ; i < unCountSig; i++ )
            {
                // Copy Structure Members
                omHeaderFile.WriteString(omStrArraySigName.GetAt(i));
            }
            // Remove all signal name
            omStrArraySigName.RemoveAll();

            omStrcommandLine.Format(defEND_OF_UNION_DEF,
                                    strFrameName );

            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defTYPEDEF_STRUCT;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defMSGPROPS_J1939;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defDLCJ1939;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine.Format(defDATATYPE_MSG_DATA,
                                    strFrameName +
                                    defUNDERSCORE+"*", defSIGNALMEMBER);
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine.Format(defEND_OF_STRUCT_DEF,
                                    strFrameName );
            omHeaderFile.WriteString(omStrcommandLine);
        }
        break;
    }

    /* Endif for guards of each message */
    omStrcommandLine.Format(defMSG_GUARD_END,  strFrameName );
    omHeaderFile.WriteString(omStrcommandLine);

    return TRUE;
}

/******************************************************************************
  Function Name    :  bFormSigNameAndLength

  Input(s)         :  UINT* punLength : Signal length
                      UINT* punStartBit : Signal start bit
                      CStringArray& omStrArraySigName : Signal array
                      list<FRAME_STRUCT>::iterator itrFrame : Frame structure

  Output           :  BOOL : Return value
******************************************************************************/
BOOL Cluster::bFormSigNameAndLength(UINT* punLength,
                                    UINT* punStartBit,
                                    CStringArray& omStrArraySigName,
                                    list<FRAME_STRUCT>::iterator itrFrame)
{
    BOOL bReturn    = FALSE;
    int unSigCount = 0;
    itrFrame->GetSignalCount(unSigCount);//m_psMessages[nIndex].m_unNumberOfSignals;;
    //sSIGNALS* pSg = m_psMessages[nIndex].m_psSignals;

    list<SIGNAL_STRUCT> sigList;
    itrFrame->GetSignalList(sigList);
    //sSIGNALS* pSg = m_psMessages[unMsgIndex].m_psSignals;
    list<SIGNAL_STRUCT>::iterator itrSig = sigList.begin();

    UINT unStartBit = 0;
    UINT unUnused   = 0;
    UINT unCount    = 0;

    //rmdup(punStartBit, unSigCount);
    UINT* p = (UINT*)std::unique(punStartBit, punStartBit+unSigCount);
    unSigCount = (p-punStartBit);
    if( itrSig != sigList.end() && punLength != NULL && punStartBit != NULL )
    {
        //unSigCount = m_psMessages[nIndex].m_unNumberOfSignals;
        CString omFormatString;
        for(UINT i = 0 ; i < unSigCount ; i++ )
        {
            SIGNAL_STRUCT itrSig = psGetSigPtr(punStartBit[i],
                                               sigList);
            if(itrSig.m_unStartbit >= 0)
            {
                UINT unSigLen = itrSig.m_nLength /** 8*/;

                // if first signal is not defined from first bit in first
                // byte
                if(punStartBit[i] > 0  && i == 0)
                {
                    // If more then first 32 bits are not having
                    // any signal defined, add two signal without
                    // any name one with size 32 bit and other
                    // with size of rest of bits.
                    INT nTempBitLen = static_cast<INT> (punStartBit[i]);
                    while (nTempBitLen > defBITS_IN_FOUR_BYTE )
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              STR_EMPTY,
                                              defUINT_LENGTH);
                        omStrArraySigName.Add(omFormatString);
                        nTempBitLen -= defBITS_IN_FOUR_BYTE;
                    }


                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defUNSIGNED_INT,
                                          STR_EMPTY,
                                          nTempBitLen);
                    omStrArraySigName.Add(omFormatString);
                }

                // For signals > 32 bits
                if( unSigLen > defBITS_IN_FOUR_BYTE)
                {
                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defINTEGER64,
                                          itrSig.m_strSignalName.c_str(),
                                          unSigLen);
                }
                // For < 32 bits use signed int
                else
                {
                    // Check for the Sign of the signal
                    if( itrSig.m_bDataType == CHAR_INT )
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defSIGNED_INTEGER,
                                              itrSig.m_strSignalName.c_str(),
                                              unSigLen);
                    }
                    else
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INTEGER,
                                              itrSig.m_strSignalName.c_str(),
                                              unSigLen);
                    }
                }
                omStrArraySigName.Add(omFormatString);
                // check if there is some unsed in between two signal
                // defined

                if( ( (punStartBit[i] + unSigLen) <
                        ( punStartBit[i + 1] ) )
                        && (unSigCount != i + 1 )
                  )
                {
                    INT nLengthTemp = static_cast<INT>(punStartBit[i + 1])
                                      - static_cast<INT>(punStartBit[i] + unSigLen);
                    while (nLengthTemp > defBITS_IN_FOUR_BYTE)
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              STR_EMPTY,
                                              defUINT_LENGTH);
                        omStrArraySigName.Add(omFormatString);
                        nLengthTemp -= defBITS_IN_FOUR_BYTE;
                    }
                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defUNSIGNED_INT,
                                          STR_EMPTY,
                                          nLengthTemp);
                    omStrArraySigName.Add(omFormatString);
                }
                else if (i == (unSigCount -1))// if last signal, check if unsed bits at the end

                {

                    INT nLastBitLength = 0;
                    nLastBitLength = static_cast<INT>
                                     ((itrFrame->m_nLength) *
                                      defBITS_IN_BYTE ) ;
                    nLastBitLength -= static_cast<INT> (punStartBit[i]);
                    nLastBitLength -= static_cast<INT> (unSigLen);
                    // If there is unsed bits at the end add bit wise definition
                    // without giving any name.
                    if(nLastBitLength >0 )
                    {
                        // If last unsed signal is more then 32 bit
                        // add two signal with no name and first
                        // having length of
                        INT nTempLastBitLen = nLastBitLength;
                        nTempLastBitLen = 32; // To be commented
                        while (nTempLastBitLen > defBITS_IN_FOUR_BYTE)
                        {
                            omFormatString.Format(defUNION_FORMAT_STRING,
                                                  defUNSIGNED_INT,
                                                  STR_EMPTY,
                                                  defUINT_LENGTH);
                            omStrArraySigName.Add(omFormatString);
                            nTempLastBitLen -= defBITS_IN_FOUR_BYTE;
                        }
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              STR_EMPTY,
                                              nTempLastBitLen);
                        omStrArraySigName.Add(omFormatString);
                    }
                }
            }
        }
        bReturn = TRUE;
    }
    return bReturn;
}

/******************************************************************************
  Function Name    :  psGetSigPtr

  Input(s)         :  UINT unStartBitSrc : Signal Start bit
                      list<SIGNAL_STRUCT> sigList:list of signals

  Output           :  SIGNAL_STRUCT : Return value
******************************************************************************/
SIGNAL_STRUCT Cluster::psGetSigPtr(UINT unStartBitSrc, list<SIGNAL_STRUCT> sigList)
{
    //sSIGNALS* psTemSig = psSigRoot;

    list<SIGNAL_STRUCT>::iterator itrSig = sigList.begin();
    UINT unStartBit = 0;
    while (itrSig != sigList.end())
    {
        if (unStartBitSrc == itrSig->m_unStartbit)
        {
            break;
        }
        itrSig++;
    }
    return *itrSig;
}

/******************************************************************************
  Function Name    :  bSortSignalStartBitAscend

  Input(s)         :  UINT *punStartBit: pointer to array of start bits of
                                         signal
                      UINT unCount  : Total count of signal.
  Output           :  TRUE/FALSE : Return value
                      UINT *punStartBit : Array of sorted value of start bit
                                          of signals
  Functionality    :  This function will sort the start bits in assending order.
                      The sorted start bit is returned in punStartBit based on
                      the number of signal count passed as unCount.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amitesh Bharti
  Date Created     :  12.03.2003
  Modifications    :  Amitesh Bharti, 22.04.2003, review comments incorporated.
******************************************************************************/
BOOL Cluster::bSortSignalStartBitAscend(UINT* punSigStartBit, UINT unCount)
{
    BOOL bReturn = FALSE;

    if(unCount >0 && punSigStartBit != NULL )
    {
        for(UINT i = 0 ; i< unCount ; i++ )
        {
            for(UINT j = i+1 ; j< unCount ; j++ )
            {
                if(punSigStartBit[i] > punSigStartBit[j] )
                {
                    UINT unTemp = punSigStartBit[i];
                    punSigStartBit[i] = punSigStartBit[j];
                    punSigStartBit[j] = unTemp;
                }
            }
        }
        bReturn = TRUE;
    }

    return 1;
}