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
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI.cpp
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Implements methods of GIGABOX flex-i FlexRay-DIL class and methods of driver class.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */

/* Project includes */
#include "FLEXRAY_GIGATRONIK_FLEXI_HELPER.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_extern.h"                /* export GetIDIL_FLEXRAY_Controller Function*/
#include "HardwareListing.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_dialog_keyslot.h"
#include "FLEXRAY_GIGATRONIK_FLEXI.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_Log.h"
#include "resource1.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab.h"

/* system includes */
#include "atlconv.h"

/* Applikation includes */
#include "DataTypes\DIL_Datatypes.h"
#include "DataTypes\Base_WrapperErrorLogger.h"
#include "DataTypes\Cluster.h"
#include "DataTypes\MsgBufAll_DataTypes.h"
#include "Include\DIL_CommonDefs.h"
#include "DataTypes\FLEXRAY_Datatypes.h"





/*---- add here macro entries for message-handler functions ----*/
BEGIN_MESSAGE_MAP(CFLEXRAY_GIGATRONIK_FLEXI, CWinApp)
END_MESSAGE_MAP()


/********************** driver class member functions implementation **********************/
/**
 * ctor implementation
 */
CFLEXRAY_GIGATRONIK_FLEXI::CFLEXRAY_GIGATRONIK_FLEXI()
{
    /* Place all significant initialization in InitInstance */
}

/**
 * implementation of overwritten method InitInstance()
 */
BOOL CFLEXRAY_GIGATRONIK_FLEXI::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}


/* The one and only CFLEXRAY_GIGATRONIK_FLEXI object (instance of the application class) */
CFLEXRAY_GIGATRONIK_FLEXI theApp;



/***************************************** init static class members of DIL class *****************************************/
std::mutex                                  CDIL_FLEXRAY_GIGATRONIK_FLEXI::lockLogfile;




/********************** DIL class member functions implementation **********************/
/**
 * ctor implementation
 */
CDIL_FLEXRAY_GIGATRONIK_FLEXI::CDIL_FLEXRAY_GIGATRONIK_FLEXI()
{
    /* init functionpointers to nullpointers */
    flexi_lib = nullptr;
    INITIALISE_DATA(flexi_API);
    INITIALISE_DATA(ProtCfg);
    INITIALISE_DATA(CtrlCfg);
    INITIALISE_DATA(MsgBufCfg);
    INITIALISE_DATA(m_flexi_Keyslot_Settings);
    INITIALISE_DATA(m_flexi_Settings);
    activeInterfaceIndex = 0;

    /* init private variables */
    DialogSettings = nullptr;
    hOwnerWnd = nullptr;
    pIlog = nullptr;
    acErrStr = "";
    hLastError = S_OK;

    /* set name of log file*/
    SYSTEMTIME temp_SysTime;
    GetLocalTime(&temp_SysTime);
    std::stringstream stringstream_LogFile;
    std::string string_directory;
    char char_directory[MAX_PATH];
    /* get current directory and copy it in char_directory */
    GetModuleFileName( NULL, char_directory, MAX_PATH );
    std::string::size_type pos = std::string( char_directory ).find_last_of( "\\/" );
    string_directory = std::string( char_directory ).substr( 0, pos);
    stringstream_LogFile << string_directory << "\\Log_Gigatronik_flex-i_"  << std::to_string(temp_SysTime.wDay) << "_"
                         << std::to_string(temp_SysTime.wMonth) << "_"
                         << std::to_string(temp_SysTime.wYear) << ".txt";
    strLogFile = stringstream_LogFile.str();

    /* start new Log-file entry */
    stringstream_LogFile.str(std::string());
    stringstream_LogFile << "New session: " << std::to_string(temp_SysTime.wHour) << ":"
                         << std::to_string(temp_SysTime.wMinute) << ":"
                         << std::to_string(temp_SysTime.wSecond);
    flexi_Log_String(strLogFile, stringstream_LogFile.str().c_str());

    /* Initialize the selected channel items array to -1
     * -1 represents not selected interface
     */
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        SelectedItemsArray[i] = -1;
    }
}


/*
 * - FLEXRAY_SetAppParams() is called right after loading FlexRay_GIGATRONIK_flex-i.dll in the file DIL_FLEXRAY.cpp.
 * - set hOwnerWnd (not used currently)(Handle of the Windows window) and pIlog (just empty implementations in the application)(instance of logging class) with the called parameter.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* ErrorLogger)
{
    HRESULT hResult = S_FALSE;

    if (ErrorLogger != nullptr)
    {
        /* set hOwnerWnd (not used currently)(Handle of the Windows window) and
            pIlog (just empty implementations in the application)(instance of logging class) with the called parameter. */
        hOwnerWnd = hWndOwner;
        pIlog = ErrorLogger;

        /* Initialise error string and error */
        acErrStr = "";
        hLastError = S_OK;

        hResult = S_OK;
    }
    else
    {
        /* set error values */
        acErrStr = "Null argument value(s) in SetAppParams";
        hLastError = S_FALSE;
    }

    return hResult;
}



/**
 * This function is responsible for loading the flex-i driver,
 * the function gets the address from the registry entry
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_LoadDriverLibrary(void)
{
    /* define variables */
    HRESULT hResult = S_OK;

    std::string libPath = "";

    std::vector<flexi_version> available_versions;
    flexi_version temp_version;
    HKEY hTestKey;

    TCHAR    achKey[MAX_KEY_LENGTH_SUBKEYNAME];            // buffer for subkey name
    DWORD    cbName;                            // size of name string
    TCHAR    achClass[MAX_PATH] = TEXT("");     // buffer for class name
    DWORD    cchClassName = MAX_PATH;           // size of class string
    DWORD    cSubKeys=0;                        // number of subkeys
    DWORD    cbMaxSubKey;                       // longest subkey size
    DWORD    cchMaxClass;                       // longest class string
    DWORD    cValues;                           // number of values for key
    DWORD    cchMaxValue;                       // longest value name
    DWORD    cbMaxValueData;                    // longest value data
    DWORD    cbSecurityDescriptor;              // size of security descriptor
    FILETIME ftLastWriteTime;                   // last write time

    DWORD i, retCode;

    /* initialize variables */
    INITIALISE_DATA(temp_version);


    /* 1: Read all installed and available Versions of the GIGABOX flex-i from the registry */
    if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, FLEXI_REGISTRY_LOCATION, 0, KEY_READ, &hTestKey) == ERROR_SUCCESS)
    {

        /* Get the class name and the value count. */
        retCode = RegQueryInfoKey(
                      hTestKey,                   // key handle
                      achClass,                   // buffer for class name
                      &cchClassName,              // size of class string
                      NULL,                       // reserved
                      &cSubKeys,                  // number of subkeys
                      &cbMaxSubKey,               // longest subkey size
                      &cchMaxClass,               // longest class string
                      &cValues,                   // number of values for this key
                      &cchMaxValue,               // longest value name
                      &cbMaxValueData,            // longest value data
                      &cbSecurityDescriptor,      // security descriptor
                      &ftLastWriteTime);          // last write time

        /* Enumerate the subkeys */
        if (cSubKeys)
        {

            for (i=0; i<cSubKeys; i++)
            {
                cbName = MAX_KEY_LENGTH_SUBKEYNAME;
                retCode = RegEnumKeyEx(hTestKey, i,
                                       achKey,
                                       &cbName,
                                       NULL,
                                       NULL,
                                       NULL,
                                       &ftLastWriteTime);
                if (retCode == ERROR_SUCCESS)
                {
                    /* save every GIGABOX flex-i driver in the vector available_versions */
                    std::string date(achKey);
                    temp_version.Key = date;
                    date.erase(date.begin());

                    std::istringstream stm(date);
                    uint16 first, second, third;
                    char delim = '_';
                    stm >> first >> delim ;
                    stm >> second >> delim ;
                    stm >> third ;

                    temp_version.VersionsNr[0] = first;
                    temp_version.VersionsNr[1] = second;
                    temp_version.VersionsNr[2] = third;

                    available_versions.insert(available_versions.begin(), temp_version);
                }
            }
        }

        RegCloseKey(hTestKey);
    }
    else
    {
        /* log error */
        flexi_Log_String(strLogFile,"ERROR: No GIGABOX flex-i registry entry found!");
        pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Error: No GIGABOX flex-i registry entry found!"));
        acErrStr = "Error: No GIGABOX flex-i registry entry found";
        hResult = S_FALSE;
    }



    /* 2: check if there is a GIGABOX flex-i driver found */
    if(hResult == S_FALSE || !(available_versions.size() > 0))
    {
        /* log error */
        flexi_Log_String(strLogFile,"ERROR: No GIGABOX flex-i driver installed!");
        pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Error: No GIGABOX flex-i driver installed!"));
        acErrStr = "Error: No GIGABOX flex-i driver installed!";
        hResult = S_FALSE;
    }


    /* 3: find the newest GIGABOX flex-i driver */
    if(hResult == S_OK)
    {
        temp_version.Key = available_versions[0].Key;
        temp_version.VersionsNr[0] = available_versions[0].VersionsNr[0];
        temp_version.VersionsNr[1] = available_versions[0].VersionsNr[1];
        temp_version.VersionsNr[2] = available_versions[0].VersionsNr[2];

        for(uint16 i = 1; i < available_versions.size(); i++)
        {
            bool newerVersionFound = false;

            if (available_versions[i].VersionsNr[0] > temp_version.VersionsNr[0] )
            {
                newerVersionFound = true;
            }
            else if (available_versions[i].VersionsNr[0] >= temp_version.VersionsNr[0])
            {
                if (available_versions[i].VersionsNr[1] > temp_version.VersionsNr[1])
                {
                    newerVersionFound = true;
                }
                else if (available_versions[i].VersionsNr[1] >= temp_version.VersionsNr[1])
                {
                    if (available_versions[i].VersionsNr[2] > temp_version.VersionsNr[2])
                    {
                        newerVersionFound = true;
                    }
                }
            }


            if (newerVersionFound == true)
            {
                temp_version.Key = available_versions[i].Key;
                temp_version.VersionsNr[0] = available_versions[i].VersionsNr[0];
                temp_version.VersionsNr[1] = available_versions[i].VersionsNr[1];
                temp_version.VersionsNr[2] = available_versions[i].VersionsNr[2];
            }

        }
    }

    /* 4: check if the newest installed driver is supported or there is a newer version required */
    if (hResult == S_OK)
    {
        bool requiredDriverInstalled = false;

        if (temp_version.VersionsNr[0] > MIN_REQUIRED_FLEXI_DRIVER_VERSION_FIRST)
        {
            requiredDriverInstalled = true;
        }
        else if (temp_version.VersionsNr[0] >= MIN_REQUIRED_FLEXI_DRIVER_VERSION_FIRST)
        {
            if (temp_version.VersionsNr[1] > MIN_REQUIRED_FLEXI_DRIVER_VERSION_SECOND)
            {
                requiredDriverInstalled = true;
            }
            else if (temp_version.VersionsNr[1] >= MIN_REQUIRED_FLEXI_DRIVER_VERSION_SECOND)
            {
                if (temp_version.VersionsNr[2] >= MIN_REQUIRED_FLEXI_DRIVER_VERSION_THIRD)
                {
                    requiredDriverInstalled = true;
                }
            }
        }

        if (requiredDriverInstalled == false)
        {
            /* log error */
            flexi_Log_String(strLogFile, "ERROR: No supported GIGABOX flex-i driver installed! GIGABOX flex-i driver 1.0.8 required!");
            pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("ERROR: No supported GIGABOX flex-i driver installed! GIGABOX flex-i driver 1.0.8 required!"));
            acErrStr = "ERROR: No supported GIGABOX flex-i driver installed! GIGABOX flex-i driver 1.0.8 required!";
            hResult = S_FALSE;
        }
    }

    /* 5: complete the registry entry of the newest driver and find the directory of the newest driver */
    if(hResult == S_OK)
    {
        BYTE acGCCPath[1024];
        HKEY sKey;
        std::string versionPath;

        /* complete the registry entry path */
        versionPath += FLEXI_REGISTRY_LOCATION;
        versionPath += "\\";
        versionPath += temp_version.Key;


        /* If the registry key open successfully, get the value in "path" */
        if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, versionPath.c_str(), 0, KEY_READ, &sKey) == ERROR_SUCCESS)
        {
            ULONG ulType = REG_SZ;
            DWORD dwSize = sizeof(acGCCPath);
            if( RegQueryValueEx(sKey, "LibPath", 0, &ulType, acGCCPath, &dwSize) == ERROR_SUCCESS)
            {
                RegCloseKey(sKey);

                /* complete the directory of the newest driver */
                libPath += reinterpret_cast< char const* >(acGCCPath);
                libPath += "\\x86\\CLIWrapper.dll";
            }
            else
            {
                hResult = S_FALSE;
            }
        }
        else
        {
            hResult = S_FALSE;
        }
    }

    /* 6: check if a GIGABOX flex-i driver got find */
    if(hResult == S_OK)
    {
        flexi_lib = LoadLibraryEx(libPath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
    }
    std::string acErr("INFO: libPath of CLIWrapper.dll: ");
    acErr.append(libPath.c_str());
    flexi_Log_String(strLogFile, acErr.c_str());


    /* 7: check if there is a GIGABOX flex-i driver found */
    if(hResult == S_OK && flexi_lib != nullptr)
    {

        hResult = Flexi_Helper::Get_FLEXI_API_Pointers(flexi_lib, flexi_API);

        if (hResult == S_OK)
        {
            /* call GtFr_EnableAsmResolver to resolve the DLL directories */
            flexi_API.GtFr_EnableAsmResolver();
        }
        else
        {
            /* log error */
            flexi_Log_String(strLogFile, "ERROR: could not get Gigatronik flex-i function pointers!");
            pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Error: could not get Gigatronik flex-i function pointers!"));
            acErrStr = "Error: could not get Gigatronik flex-i function pointers!";
            hResult = S_FALSE;
        }
    }
    else
    {
        /* log error */
        if (flexi_lib == nullptr)
        {
            std::string acErr("ERROR: CLIWrapper.dll: ");
            acErr.append(libPath.c_str());
            flexi_Log_String(strLogFile, "ERROR: flexi_lib == nullptr");
        }
        hResult = S_FALSE;
        std::string acErr(libPath);
        acErr.append(" failed to load");
        flexi_Log_String(strLogFile, acErr.c_str());
        pIlog->vLogAMessage(A2T(__FILE__), __LINE__, acErr);;
        acErrStr = acErr;
        pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support"));
    }

    return hResult;
}

/**
 * Do some initialization.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_PerformInitOperations(void)
{
    HRESULT hResult = S_FALSE;

    /* Call GtFr_Init() necessary, because there will be GtFr_GetInterfaceInfo called in the next Method FLEXRAY_ListHwInterfaces */
    GtFr_ReturnType ret = flexi_API.GtFr_Init();
    if(ret == GTFR_SUCCESS)
    {
        hResult = S_OK;
    }

    return hResult;
}

/**
 * This function select the hardware interface.
 * If there are too many interfaces connected, the function opens a dialog for the selection.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_ListHwInterfaces(FLEXRAY_INTERFACE_HW& sSelHwInterface, FLEXRAY_CONFIG_FILES&  sAvailableConfigFiles)
{
    /* define variables */
    HRESULT hResult = S_OK;
    uint8 InterfaceCount = 0;

    /* search for flex-i devices */
    if (flexi_API.GtFr_GetInterfaceCount(&InterfaceCount) == GTFR_SUCCESS)
    {
        /* Success only if there exists alteast one interface */
        if (InterfaceCount > 0)
        {
            /* log message */
            std::stringstream ss;
            ss << "INFO: There are/is " << (uint16) InterfaceCount << " device(s).";
            flexi_Log_String(strLogFile, ss.str().c_str());

            /* define variables */
            GtFr_InterfaceInfoType iInfo;
            GtFr_InterfaceExtInfoType iExtInfo;

            /* define interface description data structure */
            INTERFACE_HW psHWInterface[defNO_OF_CHANNELS];
            /* set the current number of channels */
            InterfaceCount = min(InterfaceCount, defNO_OF_CHANNELS);

            /* create description for every found interface for the dialog */
            for (uint8 i = 0; i < InterfaceCount; i++)
            {
                /* get information about the certain interface */
                flexi_API.GtFr_GetInterfaceInfo(i, &iInfo);
                flexi_API.GtFr_GetInterfaceExtInfo(i, &iExtInfo);

                psHWInterface[i].m_dwIdInterface = 0;
                psHWInterface[i].m_dwVendor = 0x2918;
                psHWInterface[i].m_acDeviceName = "GIGABOX flex-i";
                psHWInterface[i].m_bytNetworkID = iInfo.iIdx;
                ss.str("");
                ss << iExtInfo.serial;
                psHWInterface[i].m_acNameInterface = ss.str();
                ss.str("");
                ss << "Firmware: " << iInfo.hardwareInfo.firmwareVersion << "; Hardware: " << iInfo.hardwareInfo.hardwareVersion;
                psHWInterface[i].m_acDescription = ss.str();
                //psHWInterface[i].m_acAdditionalInfo;          /* there is no use currently */

                /* log message */
                std::stringstream ss;
                ss << "INFO: Device description: serial: " << psHWInterface[i].m_acNameInterface
                   << "; interface index: " << (uint16) psHWInterface[i].m_bytNetworkID
                   << "; " << psHWInterface[i].m_acDescription;
                flexi_Log_String(strLogFile, ss.str().c_str());
            }


            /* check if there is at least one interface connected */
            if (InterfaceCount > 1)
            {

                /* to switch the resource handle to the used DLL, important to use dialogs in a DLL */
                AFX_MANAGE_STATE(AfxGetStaticModuleState());

                /* create dialog for hardware selection and show it modal */
                CHardwareListing HwList(psHWInterface, InterfaceCount, SelectedItemsArray, nullptr);
                INT nRet = HwList.DoModal();

                /* interprete return value of the dialog */
                if (nRet == -1)
                {
                    /* log message */
                    std::stringstream ss;
                    ss << "ERROR: Hardware selection dialog box could not be created! Error: " << GetLastError();
                    flexi_Log_String(strLogFile, ss.str().c_str());

                    hResult = S_FALSE;
                }
                else if (nRet == IDOK)
                {
                    /* In case of successful dialog: save count and the list which stores the corresponding ID (respectivly index).
                     * In this case there can only be one interface selected. The first value of the list matches the interface index.
                     */
                    HwList.nGetSelectedList(SelectedItemsArray);
                    hResult = S_OK;
                }
                else
                {
                    /* return if user cancels hardware selection */
                    hResult = HW_INTERFACE_NO_SEL;
                }

            }
            /* if there is only one interface, there is no need to offer a selection option */
            else
            {
                /* The index of the only interface is 0 */
                SelectedItemsArray[0] = 0;
            }

            if(hResult == S_OK)
            {
                strcpy_s(sSelHwInterface.m_acNameInterface, psHWInterface[SelectedItemsArray[0]].m_acNameInterface.c_str());
                strcpy_s(sSelHwInterface.m_acDescription , psHWInterface[SelectedItemsArray[0]].m_acNameInterface.c_str());
                strcpy_s(sSelHwInterface.m_acDeviceName , psHWInterface[SelectedItemsArray[0]].m_acDeviceName.c_str());
                sSelHwInterface.m_dwIdInterface = psHWInterface[SelectedItemsArray[0]].m_bytNetworkID;
                sSelHwInterface.m_dwVendor = FLEXRAY_DRIVER_GIGATRONIK_FLEXI;
            }
        }
        else
        {
            /* There is no flex-i connected */
            hResult = S_FALSE;
        }
    }
    else
    {
        /* GtFr_GetInterfaceCount indicates no success */
        hResult = S_FALSE;
    }

    if(hResult == S_FALSE)
    {
        /* log message */
        flexi_Log_String(strLogFile, "ERROR: No Gigatronik flex-i device connected.");

        /* deinit api, to ensure getting the newest number of connected interfaces next time the flex-i driver get selected */
        flexi_API.GtFr_DeInit();
    }
    else if (hResult == HW_INTERFACE_NO_SEL)
    {
        /* log message */
        flexi_Log_String(strLogFile, "WARNING: No Gigatronik flex-i device selected.");

        /* deinit api, to ensure getting the newest number of connected interfaces next time the flex-i driver get selected */
        flexi_API.GtFr_DeInit();
    }

    return hResult;
}


/**
 * This function save corresponding index from selected interface.
 * Additionally the two dialogs (the modal keyslot settings dialog and the non modal flexi settings) will be opened here.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_SelectHwInterface(const FLEXRAY_INTERFACE_HW_LIST& sSelHwInterface)
{
    HRESULT hResult = S_FALSE;

    /* save corresponding index from selected interface
     * take first interface of the array, because it represents the one and only selected interface
     */
    activeInterfaceIndex = (uint8) sSelHwInterface[0].m_dwIdInterface;

    /* to switch the resource handle to the used DLL, important to use dialogs in a DLL */
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    /* open modal dialog for keyslot settings */
    CflexiKeyslotListing modalerDialog(&m_flexi_Keyslot_Settings);
    if(modalerDialog.DoModal() == IDOK)
    {
        /* In case user closes dialog with OK Button, return value is IDOK */
        hResult = S_OK;
        flexi_Log_String(strLogFile, "INFO: custom keyslot configuration.");
    }
    else
    {
        /* In case user closes dialog with CANCEL Button */
        hResult = S_OK;
        flexi_Log_String(strLogFile, "INFO: default keyslot configuration.");
    }

    /* show flexi settings dialog */
    DialogSettings = new CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab();
    DialogSettings->Create(IDD_DIALOG_FLEXI_SETTINGS_TAB, nullptr);

    /* get default settings before configure flexi/FlexRay-bus */
    m_flexi_Settings = DialogSettings->getSettings();
    /* activate Update-Button */
    DialogSettings->readyToUpdate();

    return hResult;
}




/**
 * This function configures the flex-i completly. This function will be called with the selection of the flexi driver and with the start-button!
 * The 3 different data structures will be send to the flexi separatly for each CC:
 * - ProtCfg: This structure defines the protocol configuration of a FlexRay communication controller.
 * - CtrlCfg: This structure defines the controller specific configuration of a FlexRay communciation controller.
 * - MsgBufCfg: This structure defines the configuration of all message buffers of a FlexRay controller.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_SetConfigData(CHANNEL_CONFIG& ouAbsSFibexContainer)
{
    /* define variables */
    HRESULT hResult = S_OK;
    GtFr_ReturnType ret;
    GtFr_CfgType cfg;

    /* Init the flex-i API in case the API has been deinitialized in the method FLEXRAY_StopHardware() before.
     * The GtFr_Init() reinitialize the API in this case. If the flex-i is already initialized the flex-i works as well.
     */
    ret = flexi_API.GtFr_Init();

    if(ret != GTFR_SUCCESS)
    {
        std::stringstream ss;
        ss << "ERROR: GtFr_Init(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* 1. load ProtCfg */
    Flexi_Helper::Get_ProtCfg(ProtCfg, ouAbsSFibexContainer);

    /* 2. load CtrlCfg */
    Flexi_Helper::Get_CtrlCfg(CtrlCfg);

    /* 3. create/load MsgBufCfg
     * At first create arrays of message buffer, because sMsgBufDat represents a pointer to an array of message buffer.
     * The arrays have to be created here to ensure the access to the data structure of the message buffer.
     */
    GtFr_MsgBufType staticMsgBufDat0[GTFR_ERAY_MAX_MSGBUF_COUNT];
    GtFr_MsgBufType staticMsgBufDat1[GTFR_ERAY_MAX_MSGBUF_COUNT];
    GtFr_MsgBufType dynamicMsgBufDat0[GTFR_ERAY_MAX_MSGBUF_COUNT];
    GtFr_MsgBufType dynamicMsgBufDat1[GTFR_ERAY_MAX_MSGBUF_COUNT];
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat = staticMsgBufDat0;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].sMsgBuf.sMsgBufDat = staticMsgBufDat1;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat = dynamicMsgBufDat0;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].dMsgBuf.dMsgBufDat = dynamicMsgBufDat1;
    /* call the Get_MsgBufCfg function with several parameters, which determine the resulting MsgBufCfg */
    Flexi_Helper::Get_MsgBufCfg(MsgBufCfg,
                                ProtCfg,
                                ouAbsSFibexContainer,
                                map_MsgBufIdx,
                                map_MsgRepetition,
                                m_flexi_Settings.transmissionMode_staticSeg,
                                m_flexi_Settings.transmissionMode_dynSeg,
                                m_flexi_Keyslot_Settings,
                                strLogFile);

    /* 4. Coldstart Settings for both CCs */
    CtrlCfg[GTFR_COMMUNICATION_CONTROLLER_0].ColdstartNode =              GTFR_DISABLED;
    CtrlCfg[GTFR_COMMUNICATION_CONTROLLER_1].ColdstartNode =              GTFR_DISABLED;
    if(m_flexi_Keyslot_Settings.CC0_Keyslot_used_for_Startup == true)
    {
        CtrlCfg[GTFR_COMMUNICATION_CONTROLLER_0].ColdstartNode =          GTFR_ENABLED;
    }
    if(m_flexi_Keyslot_Settings.CC1_Keyslot_used_for_Startup == true)
    {
        CtrlCfg[GTFR_COMMUNICATION_CONTROLLER_1].ColdstartNode =          GTFR_ENABLED;
    }

    /* 5. Keyslot Settings for CC0 */
    ProtCfg.PKeySlotUsedForStartup = GTFR_DISABLED;
    ProtCfg.PKeySlotUsedForSync = GTFR_DISABLED;
    if(m_flexi_Keyslot_Settings.CC0_Keyslot_used_for_Startup == true)
    {
        ProtCfg.PKeySlotUsedForStartup = GTFR_ENABLED;
    }
    if(m_flexi_Keyslot_Settings.CC0_Keyslot_used_for_Sync == true)
    {
        ProtCfg.PKeySlotUsedForSync = GTFR_ENABLED;
    }

    /* 6. transfer settings for CC0 */
    /* 6.1. transfer ProtCfg for CC0 */
    cfg.cfgTag = GTFR_PROT_CFG;
    cfg.cfgData.protCfg = ProtCfg;
    ret = flexi_API.GtFr_InitComCtrl(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, cfg);

    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC0, GtFr_InitComCtrl(), ProtCfg, error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* 6.2. transfer MsgBufCfg for CC0 */
    cfg.cfgTag = GTFR_MSGBUF_CFG;
    cfg.cfgData.msgBufCfg = MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0];
    ret = flexi_API.GtFr_InitComCtrl(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, cfg);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC0, GtFr_InitComCtrl(), MsgBufCfg, error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* 6.3. transfer CtrlCfg for CC0 */
    cfg.cfgTag = GTFR_CTRL_CFG;
    cfg.cfgData.ctrlCfg = CtrlCfg[GTFR_COMMUNICATION_CONTROLLER_0];
    ret = flexi_API.GtFr_InitComCtrl(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, cfg);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC0, GtFr_InitComCtrl(), CtrlCfg, error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* 7. Keyslot Settings for CC1 */
    ProtCfg.PKeySlotUsedForStartup = GTFR_DISABLED;
    ProtCfg.PKeySlotUsedForSync = GTFR_DISABLED;
    if(m_flexi_Keyslot_Settings.CC1_Keyslot_used_for_Startup == true)
    {
        ProtCfg.PKeySlotUsedForStartup = GTFR_ENABLED;
    }
    if(m_flexi_Keyslot_Settings.CC1_Keyslot_used_for_Sync == true)
    {
        ProtCfg.PKeySlotUsedForSync = GTFR_ENABLED;
    }

    /* 8. transfer settings for CC1 */
    /* 8.1. transfer ProtCfg for CC1 */
    cfg.cfgTag = GTFR_PROT_CFG;
    cfg.cfgData.protCfg = ProtCfg;
    ret = flexi_API.GtFr_InitComCtrl(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_1, cfg);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC1, GtFr_InitComCtrl(), ProtCfg, error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* 8.2. transfer MsgBufCfg for CC1 */
    cfg.cfgTag = GTFR_MSGBUF_CFG;
    cfg.cfgData.msgBufCfg = MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1];
    ret = flexi_API.GtFr_InitComCtrl(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_1, cfg);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC1, GtFr_InitComCtrl(), MsgBufCfg, error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* 8.3. transfer CtrlCfg for CC1 */
    cfg.cfgTag = GTFR_CTRL_CFG;
    cfg.cfgData.ctrlCfg = CtrlCfg[GTFR_COMMUNICATION_CONTROLLER_1];
    ret = flexi_API.GtFr_InitComCtrl(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_1, cfg);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC1, GtFr_InitComCtrl(), CtrlCfg, error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }


    return hResult;
}


/**
 * This function starts the FlexRay-Bus eventually and sets the Event Queue Notifier (callback function).
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_StartHardware(void)
{
    /* define variables */
    HRESULT hResult = S_OK;
    GtFr_ReturnType ret;


    /**
     * start FlexRay bus:
     * - execute GTFR_CMD_INIT and than GTFR_CMD_START for both CCs
     */
    ret = flexi_API.GtFr_ExecuteCommand(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, GTFR_CMD_INIT);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC0, GtFr_ExecuteCommand(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    ret = flexi_API.GtFr_ExecuteCommand(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_1, GTFR_CMD_INIT);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC1, GtFr_ExecuteCommand(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    ret = flexi_API.GtFr_ExecuteCommand(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, GTFR_CMD_START);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC0, GtFr_ExecuteCommand(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    ret = flexi_API.GtFr_ExecuteCommand(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_1, GTFR_CMD_START);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: CC1, GtFr_ExecuteCommand(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* set callback function for receiving events */
    void (*callback_functionpointer_EventQueue)(GtFr_EventQueueStatusType, GtFr_EventType*) = &(Flexi_Helper::callback_EventQueue);
    ret = flexi_API.GtFr_SetEventQueueNotification(activeInterfaceIndex, EVENTQUEUE_THRESHOLD, callback_functionpointer_EventQueue);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: GtFr_SetEventQueueNotification(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }


    /* set callback function for queue overflow */
    void (*callback_functionpointer_queueoverflow)(GtFr_QueueOverflowArgsType) = &(Flexi_Helper::callback_EventQueueOverflow);
    ret = flexi_API.GtFr_SetEventQueueOverflow(activeInterfaceIndex, callback_functionpointer_queueoverflow);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: GtFr_SetEventQueueOverflow(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    return hResult;
}


/**
 * This function will be called when the flexi driver gets deactivated.
 * Here the instance of the flexi settings dialog will be closed.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

    /* close the flexi settings dialog */
    delete DialogSettings;
    DialogSettings = nullptr;


    /* Stop the FlexRay communication and deinit the flex-i API */
    GtFr_ReturnType ret = GTFR_INTERFACE_NOT_CONNECTED;
    if (NULL != flexi_API.GtFr_DeInit)
    {
        ret = flexi_API.GtFr_DeInit();
    }
    else
    {
        hResult = S_FALSE;;
    }

    if (ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "WARNING: GtFr_DeInit(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    return hResult;
}

/**
 * This function initialises the flex-i clock and synchronize the busmaster timestamp
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp)
{
    HRESULT hResult = S_OK;


    /* Restarts the clock counting beginning from 0. */
    GtFr_ReturnType ret = flexi_API.GtFr_ClockCommand(GTFR_CLOCK_RESTART);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "ERROR: GtFr_ClockCommand(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* initialize system time and timestamp */
    SYSTEMTIME temp_SysTime;
    GetLocalTime(&temp_SysTime);
    CurrSysTime = temp_SysTime;
    TimeStamp = 0;

    return hResult;
}


/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_DeselectHwInterface(void)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}

/**
 * This function is called when the 'Disconnect'-button gets pushed.
 * - Stop the FlexRay communication and deinit the flex-i API
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_StopHardware(void)
{
    HRESULT hResult = S_OK;

    /* Stop the FlexRay communication and deinit the flex-i API */
    GtFr_ReturnType ret = flexi_API.GtFr_DeInit();

    /* reset map, which stores payload of corresponding message buffer*/
    payloadLookup.clear();

    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "WARNING: GtFr_DeInit(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    return hResult;
}

/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_GetCurrStatus(s_FLXSTATUSMSG& /*StatusData*/)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}

/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_GetFlxTxMsgBuffer(CFlxTxMsgBuffer*& /*pouFlxTxMsgBuffer*/)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}



HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_GetLastErrorString(std::string acErrorStr, HRESULT& nError)
{
    HRESULT hResult;

    /* return last error string and error number */
    acErrorStr = acErrStr;
    nError = hLastError;

    hResult = S_OK;
    return hResult;
}


/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_UnloadDriverLibrary(void)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}


/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FlexRAY_GetControllerCapabilities(s_FLXControllerCapabilities& /*ouFlexRayCapabilities*/)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}


/**
 * This function transmits a certain message to the flex-i:
 * At first the corresponding message buffer will be reconfigured, because the same message buffer
 * can have a wrong transmission mode as a result of a DeleteMsg()-calling.
 * After the reconfigurering the payload will be transmitted.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_SendMsg(DWORD /*dwClientID*/, s_FLXTXMSG* pouFlxTxMsg)
{
    /* define variables */
    HRESULT hResult = S_OK;
    GtFr_ReturnType ret;

    SLOT_BASECYCLE nSlotBase;
    uint16 usedMsgBufIdx;
    uint32 MsgRepetition;

    uint8 lSduLength;
    uint8 lSdu[254];

    GtFr_MsgBufType msgBufCfg;


    /* slotID and the basecycle identify the corresponding MsgBuf */
    nSlotBase = MAKELONG( pouFlxTxMsg->m_sFlxMsg.m_nSlotID, MAKEWORD(pouFlxTxMsg->m_sFlxMsg.m_nBaseCycle, pouFlxTxMsg->m_sFlxMsg.m_eChannel) );
    usedMsgBufIdx = (uint16) (map_MsgBufIdx.find(nSlotBase))->second;
    MsgRepetition = (map_MsgRepetition.find(nSlotBase))->second;

    /* 1. reconfigure msg buffer */
    /* load MsgBuf data */
    msgBufCfg.tag = GTFR_MSGBUF_TX;

    if(pouFlxTxMsg->m_sFlxMsg.m_eChannel == 'A')
    {
        msgBufCfg.txMsgBuf.ChannelAEnable          = GTFR_ENABLED;
        msgBufCfg.txMsgBuf.ChannelBEnable          = GTFR_DISABLED;
    }
    else if(pouFlxTxMsg->m_sFlxMsg.m_eChannel == 'B')
    {
        msgBufCfg.txMsgBuf.ChannelAEnable          = GTFR_DISABLED;
        msgBufCfg.txMsgBuf.ChannelBEnable          = GTFR_ENABLED;
    }
    else
    {
        msgBufCfg.txMsgBuf.ChannelAEnable          = GTFR_ENABLED;
        msgBufCfg.txMsgBuf.ChannelBEnable          = GTFR_ENABLED;
    }

    msgBufCfg.txMsgBuf.BaseCycle                = pouFlxTxMsg->m_sFlxMsg.m_nBaseCycle;
    msgBufCfg.txMsgBuf.FrameId                  = pouFlxTxMsg->m_sFlxMsg.m_nSlotID;
    msgBufCfg.txMsgBuf.HeaderCrc                = 0u;
    msgBufCfg.txMsgBuf.MsgBufInterruptEnable    = 1u;
    msgBufCfg.txMsgBuf.PayloadLength            = pouFlxTxMsg->m_sFlxMsg.m_nDLC / 2;       //pouFlxTxMsg->m_sFlxMsg.m_nDLC indicates the length in bytes and not, as claimed, in words !!
    msgBufCfg.txMsgBuf.PayloadPreamble          = 0u;
    msgBufCfg.txMsgBuf.Repetition               = (uint16) MsgRepetition;
    msgBufCfg.txMsgBuf.TransmissionMode         = GTFR_TX_MSGBUF_CONTINOUS;
    /* check if the selected message is a dynamic or static message */
    if(MsgBufCfg[0].sMsgBuf.sMsgBufCount > usedMsgBufIdx)
    {
        if(m_flexi_Settings.transmissionMode_staticSeg == GTFR_TX_MSGBUF_SINGLE_SHOT)
        {
            msgBufCfg.txMsgBuf.TransmissionMode     = GTFR_TX_MSGBUF_SINGLE_SHOT;
        }
    }
    else
    {
        if(m_flexi_Settings.transmissionMode_dynSeg == GTFR_TX_MSGBUF_SINGLE_SHOT)
        {
            msgBufCfg.txMsgBuf.TransmissionMode     = GTFR_TX_MSGBUF_SINGLE_SHOT;
        }
    }

    msgBufCfg.txMsgBuf.MsgBufNr     = usedMsgBufIdx;

    /* reconfigure MsgBuf to ensure right transmission mode */
    ret = flexi_API.GtFr_ReconfigureMsgBuf(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, msgBufCfg);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "WARNING: GtFr_ReconfigureMsgBuf(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* 2. transmit payload */
    /* get payloadLength in bytes and not, as claimed, in words !! */
    lSduLength = (uint8) pouFlxTxMsg->m_sFlxMsg.m_nDLC;

    /* declare vector for lookup to store payload, size depends on the length of the payload */
    std::vector<uint8> payload_for_lookup(lSduLength);

    /* copy payload, for transmission and payload-lookup */
    for(unsigned int i = 0; i < lSduLength; i++)
    {
        lSdu[i] = pouFlxTxMsg->m_sFlxMsg.m_ucData[i];
        payload_for_lookup[i] = pouFlxTxMsg->m_sFlxMsg.m_ucData[i];
    }

    /* finally transmit payload */
    ret = flexi_API.GtFr_TransmitTxLPdu(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, usedMsgBufIdx, lSduLength, lSdu);
    if(ret != GTFR_SUCCESS)
    {
        /* Log message */
        std::stringstream ss;
        ss << "WARNING: GtFr_TransmitTxLPdu(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* store payload for transmission change, to be able to send latest payload */
    /* check if corresponding payload exists already */
    if(payloadLookup.count(msgBufCfg.txMsgBuf.MsgBufNr))
    {
        payloadLookup.erase(msgBufCfg.txMsgBuf.MsgBufNr);
    }
    payloadLookup.insert( std::pair<uint16, std::vector<uint8> >(msgBufCfg.txMsgBuf.MsgBufNr, payload_for_lookup) );

    return hResult;
}

/**
 * This function stops transmitting messages from ceratin message buffer.
 * The function look for the right message buffer and reconfigure it with the transmission mode single shot.
 * Therefore the message will be send only once and then stops transmitting messages.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_DeleteMsg(DWORD /*dwClientID*/, s_FLXTXMSG* pouFlxTxMsg)
{
    /* define variables */
    HRESULT hResult = S_OK;
    GtFr_ReturnType ret;
    SLOT_BASECYCLE nSlotBase;
    uint32 usedMsgBufIdx;
    uint32 MsgRepetition;

    GtFr_MsgBufType msgBufCfg;

    /* slotID and the basecycle identify the corresponding MsgBuf */
    nSlotBase = MAKELONG( pouFlxTxMsg->m_sFlxMsg.m_nSlotID, MAKEWORD(pouFlxTxMsg->m_sFlxMsg.m_nBaseCycle, pouFlxTxMsg->m_sFlxMsg.m_eChannel) );
    usedMsgBufIdx = (map_MsgBufIdx.find(nSlotBase))->second;
    MsgRepetition = (map_MsgRepetition.find(nSlotBase))->second;

    /* load MsgBuf data */
    msgBufCfg.tag = GTFR_MSGBUF_TX;

    if(pouFlxTxMsg->m_sFlxMsg.m_eChannel == 'A')
    {
        msgBufCfg.txMsgBuf.ChannelAEnable          = GTFR_ENABLED;
        msgBufCfg.txMsgBuf.ChannelBEnable          = GTFR_DISABLED;
    }
    else if(pouFlxTxMsg->m_sFlxMsg.m_eChannel == 'B')
    {
        msgBufCfg.txMsgBuf.ChannelAEnable          = GTFR_DISABLED;
        msgBufCfg.txMsgBuf.ChannelBEnable          = GTFR_ENABLED;
    }
    else
    {
        msgBufCfg.txMsgBuf.ChannelAEnable          = GTFR_ENABLED;
        msgBufCfg.txMsgBuf.ChannelBEnable          = GTFR_ENABLED;
    }

    msgBufCfg.txMsgBuf.BaseCycle                = pouFlxTxMsg->m_sFlxMsg.m_nBaseCycle;
    msgBufCfg.txMsgBuf.FrameId                  = pouFlxTxMsg->m_sFlxMsg.m_nSlotID;
    msgBufCfg.txMsgBuf.HeaderCrc                = 0u;
    msgBufCfg.txMsgBuf.MsgBufInterruptEnable    = GTFR_ENABLED;
    msgBufCfg.txMsgBuf.PayloadLength            = pouFlxTxMsg->m_sFlxMsg.m_nDLC / 2;       //pouFlxTxMsg->m_sFlxMsg.m_nDLC indicates the length in bytes and not, as claimed, in words !!
    msgBufCfg.txMsgBuf.PayloadPreamble          = 0u;
    msgBufCfg.txMsgBuf.Repetition               = (uint16) MsgRepetition;
    msgBufCfg.txMsgBuf.TransmissionMode         = GTFR_TX_MSGBUF_SINGLE_SHOT;

    msgBufCfg.txMsgBuf.MsgBufNr = (uint16) usedMsgBufIdx;

    /* finally reconfigure corresponding message buffer */
    ret = flexi_API.GtFr_ReconfigureMsgBuf(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, msgBufCfg);
    if(ret != GTFR_SUCCESS)
    {
        std::stringstream ss;
        ss << "WARNING: GtFr_ReconfigureMsgBuf(), error code: " << ret;
        flexi_Log_String(strLogFile, ss.str().c_str());
        hResult = S_FALSE;
    }

    /* delete corresponding payload lookup entry */
    if(payloadLookup.count(msgBufCfg.txMsgBuf.MsgBufNr))
    {
        payloadLookup.erase(msgBufCfg.txMsgBuf.MsgBufNr);
    }

    return hResult;
}

/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_GetBoardInfo(s_BOARDINFO& /*BoardInfo*/)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}

/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_DisplayConfigDlg(PCHAR& /*InitData*/, int& /*Length*/)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}

/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_ResetHardware(void)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}

/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_GetFlexRayInfo(s_FLXINFO& /*FlexRayInfo*/)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}

/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_GetVersionInfo(VERSIONINFO& /*sVerInfo*/)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}

/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_FilterFrames(FILTER_TYPE /*FilterType*/, FLEX_CHANNEL /*FlexChannel*/, UINT* /*punFrames*/, UINT /*nLength*/)
{
    /* define variables */
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}

/**
 * This function is currently not used.
 */
HRESULT CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_ConfigMsgBuf(s_MSGBUF /*sMsgBuf*/, UINT* /*punAddress*/)
{
    HRESULT hResult = S_FALSE;

    // not implemented yet

    return hResult;
}




/**
 * This function will be called if the 'Update'-Button of the flexi-settings dialog gets pushed.
 * If the settings have changed, the corresponding modification takes place:
 * - If the transmission mode of a certain segment have changed, the corresponding MsgBuf will be reconfigured
 *      and in the case the new transmission mode is continuous, the payload have to be sent once.
 * - If the settings of the FIFO have changed, the corresponding API-function will be called.
 */
void CDIL_FLEXRAY_GIGATRONIK_FLEXI::callback_Update_flexi_Settings(flexi_Settings new_flexi_Settings)
{

    /* define variables */
    flexi_Settings old_flexi_Settings;
    GtFr_ExtRxFifoFilterCfgType temp_filterCfg;
    GtFr_ReturnType ret;
    GtFr_MsgBufCfgType* msgBufCfgPtrAddr;
    GtFr_MsgBufType temp_msgBuf;
    uint8 temp_lSdu[254];

    /* store old flexi settings for comparison */
    old_flexi_Settings = m_flexi_Settings;
    m_flexi_Settings = new_flexi_Settings;

    /***************************** transmission mode *****************************/
    /* reconfigure Message Buffer if the transmission mode of the static segment has changed */
    if(old_flexi_Settings.transmissionMode_staticSeg != new_flexi_Settings.transmissionMode_staticSeg)
    {
        /* get Message Buffer Configuration from the API to reconfigure all static message buffers */
        flexi_API.GtFr_GetMsgBufConfig(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, &msgBufCfgPtrAddr);

        if(msgBufCfgPtrAddr != nullptr)
        {
            /* run through all static message buffers */
            for(uint16 i = 0; i < msgBufCfgPtrAddr->sMsgBuf.sMsgBufCount; i++)
            {
                /* only consider transmitting messages */
                if(msgBufCfgPtrAddr->sMsgBuf.sMsgBufDat[i].tag == GTFR_MSGBUF_TX)
                {
                    /* copy relevant data from the message buffer configuration */
                    temp_msgBuf.txMsgBuf = msgBufCfgPtrAddr->sMsgBuf.sMsgBufDat[i].txMsgBuf;
                    temp_msgBuf.tag = GTFR_MSGBUF_TX;

                    /* change transmission mode */
                    temp_msgBuf.txMsgBuf.TransmissionMode = new_flexi_Settings.transmissionMode_staticSeg;

                    /* reconfigure selected message buffer */
                    ret = flexi_API.GtFr_ReconfigureMsgBuf(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, temp_msgBuf);
                    if(ret != GTFR_SUCCESS)
                    {
                        /* Log message */
                        std::stringstream ss;
                        ss << "WARNING: GtFr_ReconfigureMsgBuf(), error code: " << ret;
                        flexi_Log_String(strLogFile, ss.str().c_str());
                    }

                    /* if new transmission mode is continuous, send the message to start the transmission */
                    if(new_flexi_Settings.transmissionMode_staticSeg == GTFR_TX_MSGBUF_CONTINOUS &&
                            payloadLookup.count(temp_msgBuf.txMsgBuf.MsgBufNr) > 0)
                    {
                        /* copy payload from lookup-map */
                        for(unsigned int i = 0; i < temp_msgBuf.txMsgBuf.PayloadLength; i++)
                        {
                            temp_lSdu[i] = payloadLookup.at(temp_msgBuf.txMsgBuf.MsgBufNr)[i];
                        }

                        ret = flexi_API.GtFr_TransmitTxLPdu(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, temp_msgBuf.txMsgBuf.MsgBufNr, (uint8) temp_msgBuf.txMsgBuf.PayloadLength, temp_lSdu);
                        if(ret != GTFR_SUCCESS)
                        {
                            /* Log message */
                            std::stringstream ss;
                            ss << "WARNING: GtFr_ReconfigureMsgBuf(), error code: " << ret;
                            flexi_Log_String(strLogFile, ss.str().c_str());
                        }

                    }
                }
            }
        }

    }
    /* reconfigure Message Buffer if the transmission mode of the dynamic segment has changed */
    if(old_flexi_Settings.transmissionMode_dynSeg != new_flexi_Settings.transmissionMode_dynSeg)
    {
        /* get Message Buffer Configuration from the API to reconfigure all dynamic message buffers */
        flexi_API.GtFr_GetMsgBufConfig(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, &msgBufCfgPtrAddr);

        if(msgBufCfgPtrAddr != nullptr)
        {
            /* run through all dynamic message buffers */
            for(uint16 i = 0; i < msgBufCfgPtrAddr->dMsgBuf.dMsgBufCount; i++)
            {
                /* only consider transmitting messages */
                if(msgBufCfgPtrAddr->dMsgBuf.dMsgBufDat[i].tag == GTFR_MSGBUF_TX)
                {
                    temp_msgBuf.txMsgBuf = msgBufCfgPtrAddr->dMsgBuf.dMsgBufDat[i].txMsgBuf;
                    temp_msgBuf.tag = GTFR_MSGBUF_TX;

                    /* change transmission mode */
                    temp_msgBuf.txMsgBuf.TransmissionMode = new_flexi_Settings.transmissionMode_dynSeg;

                    /* reconfigure selected message buffer */
                    ret = flexi_API.GtFr_ReconfigureMsgBuf(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, temp_msgBuf);
                    if(ret != GTFR_SUCCESS)
                    {
                        /* Log message */
                        std::stringstream ss;
                        ss << "WARNING: GtFr_ReconfigureMsgBuf(), error code: " << ret;
                        flexi_Log_String(strLogFile, ss.str().c_str());
                    }

                    /* if new transmission mode is continuous, send the message to start the transmission */
                    if(new_flexi_Settings.transmissionMode_dynSeg == GTFR_TX_MSGBUF_CONTINOUS &&
                            payloadLookup.count(temp_msgBuf.txMsgBuf.MsgBufNr) > 0)
                    {
                        /* copy payload from lookup-map */
                        for(unsigned int i = 0; i < temp_msgBuf.txMsgBuf.PayloadLength; i++)
                        {
                            temp_lSdu[i] = payloadLookup.at(temp_msgBuf.txMsgBuf.MsgBufNr)[i];
                        }

                        ret = flexi_API.GtFr_TransmitTxLPdu(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0, temp_msgBuf.txMsgBuf.MsgBufNr, (uint8) temp_msgBuf.txMsgBuf.PayloadLength, temp_lSdu);
                        if(ret != GTFR_SUCCESS)
                        {
                            /* Log message */
                            std::stringstream ss;
                            ss << "WARNING: GtFr_ReconfigureMsgBuf(), error code: " << ret;
                            flexi_Log_String(strLogFile, ss.str().c_str());
                        }

                    }

                }
            }
        }
    }

    /***************************** FIFO filter *****************************/
    /* The FIFO Filters allow to limit the messages getting in the FIFO of the CC.
     * e.g. if there is a reject-filter enabled, the messages in the adjusted limits do not
     * get in the FIFO and fewer receiving messages from the FIFO take care of the USB-traffic.
     */
    /* there are currently 4 fifo filters available */
    for(uint16 i = 0; i < 4; i++)
    {
        /* check if there are valid fifo filter and changed */
        if(new_flexi_Settings.FifoFilterActive[i] == true ||
                old_flexi_Settings.FifoFilterActive[i] == true && new_flexi_Settings.FifoFilterActive[i] == false )
        {
            temp_filterCfg = new_flexi_Settings.FifoFilter[i];

            /* deactivate filter in case of unchecked filter */
            if(old_flexi_Settings.FifoFilterActive[i] == true && new_flexi_Settings.FifoFilterActive[i] == false)
            {
                temp_filterCfg.isActive = false;
                temp_filterCfg.filterTag = FIFO_FID_ACCEPTANCE_FILTER;          // set filter as acceptance filter to allow rejected frames
            }

            /* send Fifo Filter for CC0 */
            ret = flexi_API.GtFr_SetExtRxFifoFilter(activeInterfaceIndex, temp_filterCfg);
            if(ret != GTFR_SUCCESS)
            {
                /* Log message */
                std::stringstream ss;
                ss << "WARNING: GtFr_SetExtRxFifoFilter(), error code: " << ret;
                flexi_Log_String(strLogFile, ss.str().c_str());
            }

            /* enable Fifo Filter */
            ret = flexi_API.GtFr_EnableExtRxFifoFilter(activeInterfaceIndex, GTFR_COMMUNICATION_CONTROLLER_0);
            if(ret != GTFR_SUCCESS)
            {
                /* Log message */
                std::stringstream ss;
                ss << "WARNING: GtFr_SetExtRxFifoFilter(), error code: " << ret;
                flexi_Log_String(strLogFile, ss.str().c_str());
            }
        }
    }

}

