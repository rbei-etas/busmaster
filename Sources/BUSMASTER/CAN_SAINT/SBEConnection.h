#ifndef _SBECONNECTION
#define _SBECONNECTION

#include "SBECommon.h"
#include <vector>
#include <string>
// Data call back function prototype
typedef void (CALLBACK* DATAEVENTCALLBACK)(int DeviceID, unsigned __int8 *pData, unsigned __int32 Length, ABS_TIMESTAMP* pAbsTimeStamp);

/********************************************************************
Class:	      CSBEConnection
---------------------------------------------------------------------
Description:	

This class encapsulates the connection to the SBE2.  It makes life
simple for client applications to create a connection to the SBE2.

********************************************************************/
class CSBEConnection
{
public:
  CSBEConnection();
  ~CSBEConnection();
  bool	                Connect(CString ApplicationName);
  bool	                Connect(CString address, unsigned __int16 port, CString ApplicationName);
  bool	                IsConnected();
  void	                SetDataCallback(DATAEVENTCALLBACK pDataCallback);
  void	                SetFTDICallback(DATAEVENTCALLBACK pDataCallback);
  void	                Disconnect(void);
  CString               RequestDeviceList(void);
  CString	              GetEngineVersion(void);
  int	                  ConnectToDevice(CString DeviceName);
  void	                DisconnectFromDevice(int DeviceID);
  void	                SendDataToDevice(int DeviceID, unsigned __int8 *pData, unsigned __int32 Length);
  void	                SendDataToDevice(int DeviceID, char *pStrData);
  void	                SendDataToDeviceRaw(int DeviceID, unsigned __int8 *pData, unsigned __int32 Length);
  void	                SendDataToDeviceRaw(int DeviceID, char *pStrData);
  void					SendFTDIToDevice(int DeviceID, unsigned __int8 *pData, unsigned __int32 Length);
  void					SendFTDIToDevice(int DeviceID, char *pStrData);
  void	                OpenAllDevices(void);
  void	                CloseAllDevices(void);
  bool					CreateVirtualSaint(void);
  bool					DestroyVirtualSaint(int DeviceID);
  bool	                OpenDeviceByName(CString DeviceName);
  void	                CloseDeviceByName(CString DeviceName);
  bool	                OpenDeviceByID(int DeviceID);
  void	                CloseDeviceByID(int DeviceID);
  bool	                AmIFirst(void);
  bool	                CloseFilter(int handle);
  bool	                Enable(bool enable);
  CString               RequestFilterList(void);
  bool	                SetFilter(CString filter);

  HANDLE				        EngineStartEvent;

private:

  CDataList             TXDataList;
  CDataList             RXDataList;
  //CDataList             FTDIRXDataList;
  DATAEVENTCALLBACK     pDataCallback;
  DATAEVENTCALLBACK     pFTDICallback;
  bool                  ThreadRunning;
  HANDLE                KATThreadRunningEvent;
  HANDLE                TXThreadRunningEvent;
  HANDLE                DCBThreadRunningEvent;
  bool                  Connected;
  bool                  SendKeepAlive;
  bool                  OpenDeviceResult;
  bool                  AmIFirstResult;
  bool                  SetFilterResult;
  bool                  CloseFilterResult;
  bool                  SetEnableResult;
  SOCKET                Connection;
  HANDLE                EngineVersionReceivedEvent;
  HANDLE                RequestDevicesEvent;
  HANDLE                RequestFiltersEvent;
  HANDLE                EngineConnectEvent;
  HANDLE                ConnectToDeviceEvent;
  HANDLE                OpenCloseDeviceEvent;
  HANDLE                AmIFirstEvent;
  HANDLE                SetFilterEvent;
  HANDLE                CloseFilterEvent;
  HANDLE                SetEnableEvent;
  unsigned __int32      DeviceID;
  CString               DeviceList;
  CString               AllDevices;
  CString               ApplicationName;
  CString               EngineVersion;
  CString               FilterList;
  unsigned long         IPAddr;
  struct hostent        *pHost;
  struct sockaddr_in    ServerAddr;

  void                  SendCommand(COMMAND Command, void *pData, unsigned __int32 len);
  static DWORD WINAPI   ClientTXThread(LPVOID pPtr);
  static DWORD WINAPI   ClientRXThread(LPVOID pPtr);	
  static DWORD WINAPI   KeepAliveThread(LPVOID pPtr);
  static DWORD WINAPI   DataCallBackThread(LPVOID pPtr);
};

#endif
