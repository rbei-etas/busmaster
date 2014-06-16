/**
 * \file      UDSSettingsWnd.h
 * \brief     Interface file for CUDS_Protocol class
 * \author    Sanchez Marin Maria Alejandra
 * Designer:    Bentea Radu Mihai
 * Interface file for CUDSSettingsWnd class
 */

#pragma once


#include "UDS_Resource.h"
#include "UDS_Protocol.h"
#include "Utility/RadixEdit.h"
#include "DataTypes/UDS_DataTypes.h"

/**  It contains the number of Data bytes that can be sent in the current message */
extern int numberOfTaken;

extern int initialByte;
/** It's used to control the index i according to if it's working on extended or normal addressing  */
extern int aux_Finterface;
/** Corresponds to the STMin received from the flowControlMessage */
extern int STMin;

extern int BSize;

extern int SizeFC;
extern int P2_Time;
extern int P2_Time_Extended;
extern int S3_Client;
extern int S3_Server;
/** it corresponds to the exact ID of the message received    */
extern int respID;

/** It Corresponds to the value put in the settingWnd if it's extended   */
extern int RespMsgID;
extern bool fMsgSize;   // If TRUE the msg should have 8 bytes always
extern bool FCRespReq;    // If TRUE TP will be ...0x3E 0x80 (No response required


// CUDSSettingsWnd dialog

class CUDSSettingsWnd : public CDialog
{
    DECLARE_DYNAMIC(CUDSSettingsWnd)

public:
    CUDSSettingsWnd(CWnd* pParent/* = NULL*/, CUDS_Protocol* pEngine);   // standard constructor
    virtual ~CUDSSettingsWnd();

    /** Associated to the Diagnostic Standard ComboBox */
    CComboBox  m_omStdDiag;
    /** Associated to the Interface ComboBox */
    CComboBox  m_omInterface;
    /** Associated to the Flow Control Length ComboBox */
    CComboBox m_omFCLength;
    /** Associated to CAN ID (Resq from ECU) */
    CRadixEdit m_omReqCanID;
    /** Associated to CAN ID (Resq from ECU) */
    CString StringReqCanID;
    /** Associated to CAN ID (Resp from ECU) */
    CRadixEdit m_omRespCanID;
    /** Associated to CAN ID (Resp from ECU) */
    CString StringRespCanID;

    /** Associated to BroadcAddress: Not implemented in this Version */
    CRadixEdit m_omReqBroadcAddress;
    CRadixEdit m_omRespBroadcAddress;

    /** Associated to BaseAddress (Resq from ECU) */
    CRadixEdit m_omReqBaseAddress;
    /** Associated to BaseAddress (Resp from ECU) */
    CRadixEdit m_omRespBaseAddress;

    /** Associated to the CheckBox Length*/
    CButton m_omCheckMsgDLC;
    /** Associated to the "No response Required CheckBox*/
    CButton m_omTPNoRespReq;

    //CButton m_omFlowControl;

    /** Associated to STmin edit Box */
    CRadixEdit m_STMin;
    /** Associated to S3Client time edit Box */
    CRadixEdit m_S3Client;
    /** Associated to S3Server time edit Box */
    CRadixEdit m_S3_Server;
    CRadixEdit P2TimeOut;
    CRadixEdit P2extended;
    CRadixEdit m_BSize;
    CString StringReqBaseAddress;
    CString StringRespBaseAddress;
    int m_nInterfaceIndex;
    int m_nDiagnosticsIndex;
    UINT ResCanID;
    UINT ReqCanID;
    int Prev_InterfaceIndex;
    int Prev_DiagnosticsIndex;

    long TempReq_CanID;
    long TempResp_CanID;
    long TempResp_BaseAddress;
    long TempReq_BaseAddress;
    CString Temp_SSTMin;
    CString Temp_BSize;




    /**
    * This function is called to initialize the settings Window
    */
    void vInitializeUDSSettingsfFields();

    /**
    * This function is called by the framework when the user selects an option in the
    * interface combo box.
    */
    void OnCbnSelchangeComboInterface();

    void CUDSSettingsWnd::OnCbnSelchangeStandardDiag();

    /**
    \brief It saves all the settings settled previously in the settings window    and also closes the Wnd
    * This function is called by the framework when the user press the OK button in the
    * Settings Window to save all the changes made  previously.
    */
    void CUDSSettingsWnd::OnBnOKPressed();

    /**
    \brief It saves all the settings settled previously in the settings window
    * This function is called by the framework when the user press the Apply button in the
    * Settings Window to save all the changes made  previously.
    */
    void CUDSSettingsWnd::OnBnApplyPressed();

    /** It sets in the settings Window the default values according to the standard */
    void CUDSSettingsWnd::OnDefaultValuesPressed();

    /**
    \brief It closes the settings window without saving the changes
    */
    void CUDSSettingsWnd::OnBnCancelPressed();

    /** This function is called when the user modifies the value of the parameter
    * STmin. It verifies that the user does not insert an invalid value.
    */
    void CUDSSettingsWnd::OnEnChangeSTmin();

    /**  This function is called when the user press the OK to get the value of the parameter
    * STmin inserted in the SettingsWnd.
    */
    void CUDSSettingsWnd::OnSaveSTmin();

    /**
    \brief It takes the value of the Flow Control Length combo box
    * This function is called when the user press the OK button and it saves the new value
    * of  FCLength
    */
    void CUDSSettingsWnd::OnCbnSelchangeFCLength();

    /**
    \brief It takes the value of the parameter Bsize
    * This function is called when the user press the OK button and it saves the new value
    * of the ISO TP parameter BSize
    */
    void CUDSSettingsWnd::OnEnChangeBSize();

    /**  This function is called when the user press the OK to get the value of the parameter
    * P2_TimeOut inserted in the SettingsWnd.
    */
    void CUDSSettingsWnd::OnEnChangeP2_TimeOut();

    /**  This function is called when the user press the OK to get the value of the parameter
    * P2_Extended inserted in the SettingsWnd.
    */
    void CUDSSettingsWnd::OnEnChangeP2_Extended();

    /**  This function is called when the user press the OK to get the value of the parameter
    * S3_Client inserted in the SettingsWnd.
    */
    void CUDSSettingsWnd::OnEnChangeS3_Client();
    void CUDSSettingsWnd::OnEnChangeS3_Server();
    afx_msg  void CUDSSettingsWnd::OnCloseWnd();




    // Dialog Data
    enum { IDD = IDD_SETTINGS_UDS };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
};
