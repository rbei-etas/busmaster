/**
 * \file      UDS_NegRespMng.h
 * \brief     Interface file for CUDS_NegRespMng class
 * \author    Sanchez Marin Maria Alejandra
 * Designer:  Bentea Radu Mihai
 * Interface file for the Negative Response Description
 */

#pragma once
#include "afxwin.h"
#include "UDSWnd_Defines.h"
#include "UDS_Protocol.h"
#include "UDS_Resource.h"

extern int BytesShown_Line;
extern int initialByte;
extern CString CurrentService;

/** This variable is used to know if a message has been sent from the UDSMainWnd  */
extern bool FSending;

/** Variable used to control if a message received should be shown */
extern bool FDontShow;       // if it's TRUE the message should not be sent
// This variable is used to check if the response corresponds to my request
extern long Font_Color;


class CUDS_NegRespMng
{
public:

    CUDS_NegRespMng(CUDS_Protocol* pEngine);
    ~CUDS_NegRespMng(void);
    CString VerifyNegResponse(unsigned char value);
    CString evaluateResp(unsigned char arrayMsg[],int Byte2Init);

};
