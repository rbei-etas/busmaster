/**
 * \file      Node.cpp
 * \brief     Definition file for Node class
 * \author    Marc Serra
 *  Manager of the UDS_Protocol Porject
 */

//#include  "MainPanel.h"
#include "MainPanel.h"
#include    "Node.h"
//#include  "stdafx.h"
//#include  <afxwin.h>
//#include  <windows.h>
//#include  <iostream>
//#include  <process.h>
//#include  <stdlib.h>
//#include  "DataTypes/UDS_DataTypes.h"
//#include  "include/struct_can.h"
//#include  "UDS_Resource.h"



/**
* CNode variables
*/
CMainPanel* MainP = NULL;//Added for Panels
CPanel* Pan = NULL;

/*unsigned int currentTime;
unsigned int initialTimeSpeed;
unsigned int initialTimeRel;
HANDLE timer;
BOOL flagEnvio;
*/
mPSTXSELMSGDATA MessageNode;



byte i_abs;


BEGIN_MESSAGE_MAP(CNode, CWinApp)
END_MESSAGE_MAP()


/**
* CNode Construction
*/
CNode::CNode(CMainPanel* MaPa, CPanel* Pa)
{
    MainP=MaPa;
    Pan = Pa;
    InitInstance();
}


/**
* CNode initialization
*/
/**********************************************************************************************************
 Function Name  :
  Input(s)      :   -
  Output        :   -
 Description    :
 Member of      :   CNode

 Author(s)      :   Marc Serra
 Date Created   :   18.09.2013
**********************************************************************************************************/
BOOL CNode::InitInstance()
{
    CWinApp::InitInstance();

    MessageNode = new mSTXSELMSGDATA();
    MessageNode->m_unCount=1;
    MessageNode->m_psTxMsg = new STCAN_MSG[1];
    MessageNode->m_psTxMsg->m_ucChannel = 1;
    MessageNode->m_psTxMsg->m_ucDataLen = 4;
    MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    //  MessageNode->m_psTxMsg->m_bCANFD = 0x656;
    MessageNode->m_psTxMsg->m_unMsgID = 0x656;



    MessageNode->m_psTxMsg->m_ucData[0] = 1;
    MessageNode->m_psTxMsg->m_ucData[1] = 1;
    MessageNode->m_psTxMsg->m_ucData[2] = 1;
    MessageNode->m_psTxMsg->m_ucData[3] = 1;

    flagEnvio = TRUE;
    //StartTimer();

    return TRUE;
}

/**
* CNode methods
*/
/**********************************************************************************************************
 Function Name  :
  Input(s)      :   -
  Output        :   -
 Description    :
 Member of      :   CNode

 Author(s)      :   Marc Serra
 Date Created   :   18.09.2013
**********************************************************************************************************/
/*unsigned __stdcall TimerFunction(void* a) {
    CNode* b = (CNode*) a;
    b->currentTime = b->MainP->nCalculateCurrTime(FALSE) ;
    b->initialTimeRel = b->currentTime;
    b->initialTimeSpeed = b->currentTime;

    //flagEnvio = TRUE;
    //flagEnvio for now... igual es canvia
    while (b->flagEnvio){
        //Actualize time
        b->currentTime = b->MainP->nCalculateCurrTime(FALSE);
        b->currentTime = b->MainP->nCalculateCurrTime(TRUE);

        //Falta Cridar a les funcions OnTimerMileRel i OnTimerSpeed
        if((b->currentTime - b->initialTimeRel) > _MILE_REL_MOTB_2010_CycleTime - 1){
            b->OnTimerMile();
            //b->initialTimeRel = b->currentTime;
            //b->MainP->SendPanelsMessage(b->MessageNode);
        }
        if((b->currentTime - b->initialTimeSpeed) > _SPEED_MOTB_2010_CycleTime - 1 ){
            b->OnTimerSpeed();
            //b->initialTimeSpeed = b->currentTime;
            //b->MainP->SendPanelsMessage(b->MessageNode);
        }

    }

    return 0;
}
*/
/**********************************************************************************************************
 Function Name  :   StartTimer
  Input(s)      :   -
  Output        :   -
 Description    :   This function is called to start the timer
 Member of      :   CNode

 Author(s)      :   Marc Serra
 Date Created   :   18.09.2013
**********************************************************************************************************/
/*UINT CNode::StartTimer(){
    //int id1=11;
    flagEnvio= TRUE;
    timer = (HANDLE) _beginthreadex(NULL, 0,  TimerFunction,  this, 0, 0);
    //StopTimer();
    return 0;
}*/

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
/*void CNode::StopTimer()
{
    flagEnvio= FALSE;
}*/

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CNode::EvaluateMessageReceived(STCAN_MSG  Mensaje )
{


}


/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CNode::OnTimerMile()
{
    initialTimeRel = currentTime;

    Pan->PrepareData();
    MessageNode->m_psTxMsg->m_ucChannel = 1;
    MessageNode->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    MessageNode->m_psTxMsg->m_unMsgID = 0x688;
    MessageNode->m_psTxMsg->m_ucData[0] = 0xFF;
    MessageNode->m_psTxMsg->m_ucData[1] = 0x0F + (0x00F & Pan->valueMileCouRrMotbk2010) * 16;
    MessageNode->m_psTxMsg->m_ucData[2] = (0xFF0 & Pan->valueMileCouRrMotbk2010) / 16;
    MessageNode->m_psTxMsg->m_ucData[3] = 0x0FF & Pan->valueMileCouFtMotbk2010;
    MessageNode->m_psTxMsg->m_ucData[4] = 0xF0 + Pan->valueMileCouFtMotbk2010 / 256;
    MessageNode->m_psTxMsg->m_ucData[5] = 0xFF;
    MessageNode->m_psTxMsg->m_ucData[6] = 0xFF;
    MessageNode->m_psTxMsg->m_ucData[7] = 0xFF;

    MainP->SendPanelsMessage(MessageNode);
}



/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CNode::OnTimerSpeed()
{
    initialTimeSpeed = currentTime;

    Pan->UpdateAlive();
    Pan->PrepareData();
    MessageNode->m_psTxMsg->m_ucChannel = 1;
    MessageNode->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    MessageNode->m_psTxMsg->m_unMsgID = 0x656;
    MessageNode->m_psTxMsg->m_ucData[0] = Pan->valueCrcVMotbk2010;
    MessageNode->m_psTxMsg->m_ucData[1] = (0x0F & Pan->valueAlivVMotbk2010) + (0x00F & Pan->valueVWhlRrMotbk2010) * 16;
    MessageNode->m_psTxMsg->m_ucData[2] = (0xFF0 & Pan->valueVWhlRrMotbk2010) / 16;
    MessageNode->m_psTxMsg->m_ucData[3] = 0x0FF & Pan->valueVWhlFtMotbk2010;
    MessageNode->m_psTxMsg->m_ucData[4] = Pan->valueStBrkFtMotbk2010 * 64 + Pan->valueStBrkRrMotbk2010 * 16 + (0xF00 &Pan->valueVWhlFtMotbk2010) / 256;
    MessageNode->m_psTxMsg->m_ucData[5] = Pan->valueStAbsMotbk2010 * 16 + Pan->valueStSenVFtMotbk2010 * 4 + Pan->valueStSenVRrMotbk2010;
    MessageNode->m_psTxMsg->m_ucData[6] = Pan->valueStStassMotbk2010 * 32 + Pan->valueModVehAbsChovMotbk2010 * 8 + Pan->valueModVehAbsAcvMotbk2010;
    MessageNode->m_psTxMsg->m_ucData[7] = Pan->valueStModTltMotbk2010 * 4 + Pan->valueStIlkTltMotbk2010;

    MainP->SendPanelsMessage(MessageNode);
}




//byte SpeedCRC8(byte array[], byte length){

//    int ApplicationId = 0x14;       /* crc8 id secure calculation     */
/*    byte count;

    crc = 0;
    crc = CrcTable8[crc ^ (byte)(ApplicationId & 0x00ff)];
    crc = CrcTable8[crc ^ (byte)(ApplicationId >> 8)];
    for (count = 0; count < length; count++){
        crc = CrcTable8[crc ^ array[count]];
    }
    return (crc);
}


void kl15StateChange (){
    if(getValue(EnvST_KL_15_MOTBK_2010_) == 1){
      setTimer(_tSPEED_MOTBK_2010, _SPEED_MOTBK_2010CycleTime);
      setTimer(_tMILE_REL_MOTBK_2010, _MILE_REL_MOTBK_2010CycleTime);
    }
    if (getValue(EnvST_KL_15_MOTBK_2010_) == 0){
      //cancelTimer(_tSPEED_MOTBK_2010);    //SPEED message should be kept in KL15 off
      //cancelTimer(_tMILE_REL_MOTBK_2010); //MILE_REL message should be kept in KL15 off
    }
}*/


