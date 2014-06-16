
/**
 * \file      UDS_Protocol.h
 * \brief     Interface file for CUDS_Protocol class
 * \author    Sanchez Marin Maria Alejandra
 * Designer:    Bentea Radu Mihai
 * Interface file for CUDS_Protocol class
 */

#pragma once


#include "DataTypes/UDS_DataTypes.h"
#include "include/struct_can.h"
#include "UDS_Resource.h"



//#include  "MainPanel.h"
//#include  "Node.h"
#include    "Panel.h"
#include    "stdafx.h"
//#include  <afxwin.h>
#include    <windows.h>
#include    <iostream>
#include    <process.h>
#include    <stdlib.h>
//#include  "DataTypes/UDS_DataTypes.h"
//#include  "include/struct_can.h"
//#include  "UDS_Resource.h"



#define _SPEED_MOTB_2010_CycleTime      100
#define _MILE_REL_MOTB_2010_CycleTime   10


class CNode : public CWinApp
{
public:
    /**
    * CNode variables
    */
    CMainPanel* MainP;
    CPanel* Pan;
    mPSTXSELMSGDATA MessageNode;

    BOOL flagEnvio;
    unsigned int currentTime;
    unsigned int initialTimeSpeed;
    unsigned int initialTimeRel;

    /**
    * CNode Construction
    */
    CNode(CMainPanel* MaPa, CPanel* Pa);

    /**
    * CNode initialization
    */
    BOOL CNode::InitInstance();

    /**
    * CNode methods
    */
    // UINT CNode::StartTimer();
    //void CNode::StopTimer();
    void CNode::EvaluateMessageReceived(STCAN_MSG  Mensaje);


    void CNode::OnTimerMile();
    void CNode::OnTimerSpeed();

private:
    /**
    * CNode variables
    */
    //HANDLE timer;

    byte CrcTable8[256];
    byte crc;
    byte SPEED_MSG_DATA[8];
    byte i_abs;

    DECLARE_MESSAGE_MAP()

};