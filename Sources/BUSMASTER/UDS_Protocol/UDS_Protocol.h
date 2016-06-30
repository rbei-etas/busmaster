
/**
 * \file      UDS_Protocol.h
 * \brief     Interface file for CUDS_Protocol class
 * \author    Sanchez Marin Maria Alejandra
 * Designer:    Bentea Radu Mihai
 * Interface file for CUDS_Protocol class
 */

#pragma once
#include "UDSMainWnd.h"
#include "DataTypes/UDS_DataTypes.h"
//#include "include/struct_can.h"
#include "CANDriverDefines.h"
#include "UDSWnd_Defines.h"
#include "UDS_Resource.h"


extern bool FWaitFlow;
extern int c_unPreviousTime;
extern double c_dDiffTime;
extern int Counter_BSize;

typedef struct mstCanDataSpl : public STCANDATA
{
    __int64      m_nDeltime;

    mstCanDataSpl()
    {
        m_nDeltime = 0;
    }
} mSTCANDATASPL;

class CUDS_Protocol : public CWinApp
{
public:

    /**
     * Constructor
     */
    CUDS_Protocol();

    //void  FormPadre();

    UINT SourceAddress;
    UINT TargetAddress;
    /** This variable contains the type of flow control received */
    INT TypeofControl;
    UINT MsgID;
    /** This variable indicates which is the interface currently selected   */
    UDS_INTERFACE fInterface ;

    /** This variable indicates the Diagnostic standard selected   */
    DIAGNOSTIC_STANDARD fDiagnostics;

    /** This variable contains all the bytes received */
    CString Data_Recibida;

    /**    This variable will be used to set the number of bytes at the beginning of the row of bytes in the responseData section     */
    int numberOfBytes;


    virtual BOOL InitInstance();

    /**
    * This function is called from the SettingsWnd to save all the values tha were settled
    * in the settings Window. This function also sets some values in the MainWnd such as
    * SA, TA,CANId
    */
    void CUDS_Protocol::UpdateFields();

    /**
     * \brief     It returns the type of frame that was received
     * This function is called to analize the  received response
     */

    TYPE_OF_FRAME CUDS_Protocol::getFrameType(unsigned char FirstByte);

    /**
    * \brief     Start the timer that enable the SEND button
    * This function is called  to disable the SEND button and to
    * start the default timer. It enters here when a new message
    * cannot be sent because the previous one hasn't been transmited
    * completely or because a long response is being received.
    */
    void CUDS_Protocol::StartTimer_Disable_Send();

    /**
    * \brief     Kill the timer that enable the SEND button
    * This function is called to able the SEND button and to
    * kill the timer. It only enters here when the time to wait for a response from the ECU
    * has passed and no response has been received
    */
    void CUDS_Protocol::KillTimer_Able_Send();

    /**
     * \brief     Show the response data bytes.
     * This function is called by EvaluateMessage to show in the Response Data section
     * the bytes received in the bus as response of a request sent from the UDS tool.
     */
    void CUDS_Protocol::Show_ResponseData(unsigned char psMsg[], unsigned char Datalen , int posFirstByte);

    DECLARE_MESSAGE_MAP()
};