/**
 * \file      UDS_Extern.h
 * \author     Sanchez Marin Maria Alejandra
 * Designer:    Bentea Radu Mihai
 */

#pragma once

//#include "PSDI_CAN/MsgContainer_CAN.h"
#include "UDS_Protocol.h"
#include "Include/Struct_CAN.h"
//#include "Struct_CAN.h"



#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif
    USAGEMODE HRESULT UDS_Initialise();
    //__declspec(dllexport) HRESULT DIL_UDS_ShowWnd(void); //Función Para mostrar Main Window

    USAGEMODE HRESULT DIL_UDS_ShowWnd(HWND hParent,int TotalChannels);//; //Función Para mostrar Main Window
    USAGEMODE HRESULT EvaluateMessage( STCAN_MSG  Mensaje  );
    USAGEMODE HRESULT UpdateChannelUDS(HWND hParent);
    USAGEMODE HRESULT DIL_UDS_ShowSettingWnd(HWND hParent);
    USAGEMODE HRESULT TX_vSetDILInterfacePtrUDS(void* ptrDILIntrf);
    USAGEMODE HRESULT DIL_MainPanel(HWND hParent,int TotalChannels);
    //int respID;

#ifdef __cplusplus
}
#endif


