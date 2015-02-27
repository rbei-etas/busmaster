/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**********************************************************************************************************************
 *
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_dialog_keyslot.h
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Defines GIGABOX flex-i FlexRay-DIL class and driver class.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */

#pragma once

/* include afxwin.h to avoid windows.h inclusion error */
#include <afxwin.h>

/* system includes */
#include <string>
#include <mutex>

/* project includes */
#include "FLEXRAY_GIGATRONIK_FLEXI_dialog_keyslot.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_configuration.h"
#include "DIL_Interface\CommanDIL_Flexray.h"




/* registry entry for flex-i api */
#define FLEXI_REGISTRY_LOCATION             "SOFTWARE\\Gigatronik\\GIGABOX_flex-i"
#define MAX_KEY_LENGTH_SUBKEYNAME           256

/* minimal required GIGABOX flex-i driver version */
#define MIN_REQUIRED_FLEXI_DRIVER_VERSION_FIRST     1
#define MIN_REQUIRED_FLEXI_DRIVER_VERSION_SECOND    0
#define MIN_REQUIRED_FLEXI_DRIVER_VERSION_THIRD     8

/* forward declaration */
class Base_WrapperErrorLogger;


/* struct to store version keys, used for finding the newest version */
typedef struct struct_flexi_version
{
    uint16          VersionsNr[3];
    std::string     Key;                    // variable for subkey name
} flexi_version;



/***************************************** class definition *****************************************/
class CDIL_FLEXRAY_GIGATRONIK_FLEXI : public CCommanDIL_Flexray
{
public:
    /* ctor */
    CDIL_FLEXRAY_GIGATRONIK_FLEXI();

    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT FLEXRAY_PerformInitOperations(void) ;
    HRESULT FLEXRAY_PerformClosureOperations(void) ;
    HRESULT FLEXRAY_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp) ;
    HRESULT FLEXRAY_ListHwInterfaces(FLEXRAY_INTERFACE_HW& sSelHwInterface, FLEXRAY_CONFIG_FILES& sAvailableConfigFiles) ;
    HRESULT FLEXRAY_SelectHwInterface(const FLEXRAY_INTERFACE_HW_LIST& sSelHwInterface) ;
    HRESULT FLEXRAY_DeselectHwInterface(void) ;
    HRESULT FLEXRAY_DisplayConfigDlg(PCHAR& InitData, int& Length) ;
    HRESULT FLEXRAY_SetConfigData(CHANNEL_CONFIG& ouAbsSFibexContainer) ;
    HRESULT FLEXRAY_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog) ;
    HRESULT FLEXRAY_StartHardware(void) ;
    HRESULT FLEXRAY_StopHardware(void) ;
    HRESULT FLEXRAY_ResetHardware(void) ;
    HRESULT FLEXRAY_GetCurrStatus(s_FLXSTATUSMSG& StatusData) ;
    HRESULT FLEXRAY_GetFlxTxMsgBuffer(CFlxTxMsgBuffer*& pouFlxTxMsgBuffer) ;
    HRESULT FLEXRAY_GetBoardInfo(s_BOARDINFO& BoardInfo) ;
    HRESULT FLEXRAY_GetFlexRayInfo(s_FLXINFO& FlexRayInfo) ;
    HRESULT FLEXRAY_GetVersionInfo(VERSIONINFO& sVerInfo) ;
    HRESULT FLEXRAY_GetLastErrorString(std::string acErrorStr, HRESULT& nError) ;
    HRESULT FLEXRAY_FilterFrames(FILTER_TYPE FilterType, FLEX_CHANNEL FlexChannel, UINT* punFrames, UINT nLength) ;
    HRESULT FLEXRAY_ConfigMsgBuf(s_MSGBUF sMsgBuf, UINT* punAddress) ;
    HRESULT FLEXRAY_LoadDriverLibrary(void) ;
    HRESULT FLEXRAY_UnloadDriverLibrary(void) ;

    HRESULT FlexRAY_GetControllerCapabilities(s_FLXControllerCapabilities& ouFlexRayCapabilities);
    HRESULT FLEXRAY_SendMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg);
    HRESULT FLEXRAY_DeleteMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg);

    /* following functions already implemented in the class CCommanDIL_Flexray */
    //HRESULT FLEXRAY_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj);
    //HRESULT FLEXRAY_SendFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg);
    //HRESULT FLEXRAY_DeleteFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg);
    //HRESULT FLEXRAY_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);

private:
    /* handle of CLI wrapper (flex-i API) */
    HMODULE flexi_lib;

    /* Complete pointers of flex-i API, collected in this struct */
    S_FLEXI_POINTER_TABLE flexi_API;

    /* data structs for the configuration of the flexi */
    GtFr_ProtocolCfgType ProtCfg;
    GtFr_MsgBufCfgType MsgBufCfg[2];
    GtFr_CtrlCfgType CtrlCfg[2];

    /* activeInterfaceIndex stores interface index of used flex-i */
    uint8 activeInterfaceIndex;

    /* save relation slot/base : MsgBufIdx and slot/bas : repetition */
    std::map<SLOT_BASECYCLE, MSG_BUF_IDX_TYPE> map_MsgBufIdx;
    std::map<SLOT_BASECYCLE, MSG_REPETITION_TYPE> map_MsgRepetition;



    /* instance of flexi settings dialog */
    CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab* DialogSettings;

    /**
     * stores the last payload of corresponding message buffer,
     * needed when transmission mode get changed
     */
    std::map< uint16, std::vector<uint8> > payloadLookup;

    /* windows handle to access the window */
    HWND hOwnerWnd;

    /* logging interface */
    Base_WrapperErrorLogger* pIlog;

    /* Buffer to store transmission error message */
    std::string acErrStr;
    HRESULT hLastError;

    /* this array stores the selected items (hw interfaces) */
    INT SelectedItemsArray[CHANNEL_ALLOWED];

public:
    /* flex-i specific settings */
    flexi_Keyslot_Settings m_flexi_Keyslot_Settings;
    flexi_Settings m_flexi_Settings;

    /* stores name of log file */
    std::string strLogFile;
    /* declare mutex which is responsible to prevent concurrent
     * access to the log file */
    static std::mutex lockLogfile;

public:
    /* callback function, handle new flexi settings */
    void callback_Update_flexi_Settings(flexi_Settings current_flexi_Settings);

};



/***************************************** driver class definition *****************************************/
/**
 * This class presents the necessary CWinApp derived class. It do not have any logic purpose.
 */
class CFLEXRAY_GIGATRONIK_FLEXI : public CWinApp
{
public:
    CFLEXRAY_GIGATRONIK_FLEXI();

public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};


