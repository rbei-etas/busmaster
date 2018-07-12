/**
 * \file      UDS_NegRespMng.cpp
 * \brief     Definition file for CUDS_NegRespMng class
 * \author    Sanchez Marin Maria Alejandra
 * Designer:  Bentea Radu Mihai
 * Manager of the Negative Response Description
 */


#include "UDS_NegRespMng.h"

#define SIZE_NEG_RESP_STRUCT    39

CUDS_Protocol* UdsProtocolPtr2 = NULL;


typedef struct
{
    INT             MsgValue;
    CString         m_omTypeResp;
    long            FontColor;
} NEGATIVE_RESP_STRUCT;


NEGATIVE_RESP_STRUCT NegativeResponseStructUDS[SIZE_NEG_RESP_STRUCT] =
{
    {0x10,        "Neg Response - General Reject",                       RGB(255, 00,0 ) },
    {0x11,        "Neg Response - Service Not Supported",                RGB(255, 00,0 ) },
    {0x12,        "Neg Response - SubFunction Not Supported",            RGB(255, 00,0 ) },
    {0x13,        "Neg Response - Incorrect Message Length Or Invalid Format",      RGB(255, 00,0 )},       // Not in KWP2000
    {0x21,        "Neg Response - Busy Repeat Request",               RGB(255, 00,0 )},
    {0x22,        "Neg Response - Conditions Not Correct",            RGB(255, 00,0 )},  //0x 23 routine not complete
    {0x24,        "Neg Response - Request Sequence Error",            RGB(255, 00,0 )},     // Not KWP2000, added to 22
    {0x31,        "Neg Response - Request Out Of Range",              RGB(255, 00,0 )},
    {0x33,        "Neg Response - Security Access Denied",            RGB(255, 00,0 )},
    {0x35,        "Neg Response - Invalid Key",                              RGB(255, 00,0 )},
    {0x36,        "Neg Response - Exceed Number Of Attempts",                RGB(255, 00,0 )},
    {0x37,        "Neg Response - Required Time Delay Not Expired",          RGB(255, 00,0 )},
    {0x70,        "Neg Response - Upload Download NotAccepted",               RGB(255, 00,0 )},
    {0x71,        "Neg Response - Transfer Data Suspended",                   RGB(255, 00,0 )},
    {0x72,        "Neg Response - General Programming Failure",               RGB(255, 00,0 )},
    {0x73,        "Neg Response - Wrong Block Sequence Counter",              RGB(255, 00,0 )},
    {0x78,        "Neg Response - Response Pending",                          RGB(184,134,11 ) },
    {0x7E,        "Neg Response - SubFunction Not Supported In Active Session",           RGB(255, 00,0 )},
    {0x7F,        "Neg Response - Service Not Supported In Active Session",               RGB(255, 00,0 )},
    {0x81,        "Neg Response - Rpm Too High",                                          RGB(255, 00,0 )},
    {0x82,        "Neg Response - Rpm Too Low",                                           RGB(255, 00,0 )},
    {0x83,        "Neg Response - Engine Is Running",                                     RGB(255, 00,0 )},
    {0x84,        "Neg Response - Engine Is Not Running",                                 RGB(255, 00,0 )},
    {0x85,        "Neg Response - Engine RunTime TooLow",                                 RGB(255, 00,0 )},
    {0x86,        "Neg Response - Temperature Too High",                 RGB(255, 00,0 )},
    {0x87,        "Neg Response - Temperature Too Low",                  RGB(255, 00,0 )},
    {0x88,        "Neg Response - Vehicle Speed Too High",               RGB(255, 00,0 )},
    {0x89,        "Neg Response - Vehicle Speed Too Low",                RGB(255, 00,0 )},
    {0x8A,        "Neg Response - Throttle/Pedal Too High",              RGB(255, 00,0 )},
    {0x8B,        "Neg Response - Throttle/Pedal Too Low",               RGB(255, 00,0 )},
    {0x8C,        "Neg Response - Transmission Range In Neutral",        RGB(255, 00,0 )},
    {0x8D,        "Neg Response - Transmission Range In Gear",           RGB(255, 00,0 )},
    {0x8F,        "Neg Response - Brake Switch(es)NotClosed (Brake Pedal not pressed or not applied)",       RGB(255, 00,0 )},
    {0x90,        "Neg Response - Shifter Lever Not In Park ",                                           RGB(255, 00,0 )},
    {0x91,        "Neg Response - Torque Converter Clutch Locked",                  RGB(255, 00,0 )},
    {0x92,        "Neg Response - Voltage Too High",                                RGB(255, 00,0 )},
    {0x93,        "Neg Response - voltageTooLow",                                   RGB(255, 00,0 )},
    {10,          "     Positive Response",          RGB(0, 100,0 )},      //It has to be always declared here
    {4,           "Negative Response",            RGB(255, 00,0 )},
};

NEGATIVE_RESP_STRUCT NegativeResponseStructKWP[32] =
{
    {0x10,        "Neg Response - General Reject",                RGB(255, 00,0 ) },
    {0x11,        "Neg Response - Service Not Supported",         RGB(255, 00,0 ) },
    {0x12,        "Neg Response - SubFunction Not Supported",     RGB(255, 00,0 ) },
    {0x13,        "Neg Response - Incorrect Message Length Or Invalid Format",          RGB(255, 00,0 )},
    {0x21,        "Neg Response - Busy Repeat Request",                                 RGB(255, 00,0 )},
    {0x22,        "Neg Response - Conditions Not Correct Or Request Sequence Error",       RGB(255, 00,0 )},
    {0x23,        "Neg Response - Routine Not Complete",             RGB(255, 00,0 )},
    {0x31,        "Neg Response - Request Out Of Range",             RGB(255, 00,0 )},
    {0x33,        "Neg Response - Security Access Denied- Security Access Requested  ",       RGB(255, 00,0 )},
    {0x35,        "Neg Response - Invalid Key",                                               RGB(255, 00,0 )},
    {0x36,        "Neg Response - Exceed Number Of Attempts",             RGB(255, 00,0 )},
    {0x37,        "Neg Response - Required Time Delay Not Expired",       RGB(255, 00,0 )},
    {0x40,        "Neg Response - Download not accepted",                 RGB(255, 00,0 )},
    {0x41,        "Neg Response - Improper download type" ,               RGB(255, 00,0 )},
    {0x42,        "Neg Response - Can not download to specified address",           RGB(255, 00,0 )},
    {0x43,        "Neg Response - Can not download number of bytes requested",      RGB(255, 00,0 )},
    {0x50,        "Neg Response - Upload not accepted",                 RGB(255, 00,0 )},
    {0x51,        "Neg Response - Improper upload type",                RGB(255, 00,0 )},
    {0x52,        "Neg Response - Can not upload from specified address",           RGB(255, 00,0 )},
    {0x53,        "Neg Response - Can not upload number of bytes requested",        RGB(255, 00,0 )},
    {0x71,        "Neg Response - Transfer suspended",            RGB(255, 00,0 )},
    {0x72,        "Neg Response - Transfer Aborted",              RGB(255, 00,0 )},
    {0x74,        "Neg Response - Illegal Address In Block Transfer",            RGB(255, 00,0 )},
    {0x75,        "Neg Response - Illegal Byte Count In Block Transfer",         RGB(255, 00,0 ) },
    {0x76,        "Neg Response - Illegal Block Transfer Type",              RGB(255, 00,0 ) },
    {0x77,        "Neg Response - Block Transfer Data Checksum Error",       RGB(255, 00,0 ) },
    {0x78,        "Neg Response - Request Correctly Received-Response Pending",         RGB(255, 00,0 ) },
    {0x79,        "Neg Response - Incorrect ByteCount During Block Transfer",           RGB(255, 00,0 ) },
    {0x80,        "Neg Response - Service Not Supported In Active Diagnostic Mode",     RGB(255, 00,0 ) },
    {10,          "     Positive Response",       RGB(0, 100,0 )},         //It has to be always declared here
    {4,           "Negative Response",         RGB(255, 00,0 )},
};


CUDS_NegRespMng::CUDS_NegRespMng(CUDS_Protocol* pEngine)
{
    UdsProtocolPtr2 = pEngine;
}

CUDS_NegRespMng::~CUDS_NegRespMng(void)
{
}

CString CUDS_NegRespMng::VerifyNegResponse(unsigned char value)
{
    switch(UdsProtocolPtr2->fDiagnostics)
    {
        case STANDARD_UDS:
        {
            for (int i_counter = 0; i_counter < SIZE_NEG_RESP_STRUCT ; i_counter++)
            {
                if (NegativeResponseStructUDS[i_counter].MsgValue == value)
                {

                    Font_Color = NegativeResponseStructUDS[i_counter].FontColor;
                    return NegativeResponseStructUDS[i_counter].m_omTypeResp;
                }
            }
        }
        case STANDARD_KWP2000:
        {
            for (int i_counter = 0; i_counter < 32 ; i_counter++)
            {
                if (NegativeResponseStructKWP[i_counter].MsgValue == value)
                {

                    Font_Color = NegativeResponseStructKWP[i_counter].FontColor;
                    return NegativeResponseStructKWP[i_counter].m_omTypeResp;
                }
            }
        }
    }

    Font_Color = NegativeResponseStructUDS[SIZE_NEG_RESP_STRUCT-1].FontColor;
    return NegativeResponseStructUDS[SIZE_NEG_RESP_STRUCT-1].m_omTypeResp;

}


CString CUDS_NegRespMng::evaluateResp(unsigned char arrayMsg[],int Byte2Init )
{

    // Byte2Init indicates which is the first byte to read from the message  to evaluate if it's a possitive or a negative Response
    // Sometimes this value is Initialbyte but in te case of a long response is Initialbyte+1

    unsigned char prueba = arrayMsg[Byte2Init+2];
    int TempValue = strtol(CurrentService, NULL, 16);
    if (arrayMsg[Byte2Init+1] == 0x7F)
    {
        if (arrayMsg[Byte2Init+2] ==TempValue       /*CurrentService*/)
        {
            // Kill the timer   and able SendButton
            UdsProtocolPtr2->KillTimer_Able_Send();

            if( arrayMsg[Byte2Init+3] == 0x78)
            {
                FSending = TRUE;                             // Continue showing the bytes
                UdsProtocolPtr2->StartTimer_Disable_Send();  // Start the timer again and disable SEND button
                BytesShown_Line = NUM_BYTES_SHOWN_RESP-2;
                UdsProtocolPtr2->numberOfBytes = -2;
            }
            return VerifyNegResponse(arrayMsg[Byte2Init+3]);             // arrayMsg[Byte2Init+3] will indicate wich message'll be shown in the RespDescription section

            // If its equal to 0x78 restart everything
        }
        else
        {
            FDontShow = TRUE;       // It's a negResponse but not for my Msg
        }
        // If it isn't the current Service I shouldn't do anything
    }
    else if(arrayMsg[Byte2Init+1] ==  TempValue+0x40)
    {
        UdsProtocolPtr2->KillTimer_Able_Send();
        Font_Color = NegativeResponseStructUDS[SIZE_NEG_RESP_STRUCT-2].FontColor;
        return NegativeResponseStructUDS[SIZE_NEG_RESP_STRUCT-2].m_omTypeResp;    // The Positive resp message will always be at the end of the list
        //Write Possitive Response in the RespDescription  section
    }
    else
    {
        FDontShow = TRUE;    // If it isn't a NegResp/PosResptomyMsg
    }
    // Then it shouldn't be shown

    return VerifyNegResponse(arrayMsg[Byte2Init+3]);
}

