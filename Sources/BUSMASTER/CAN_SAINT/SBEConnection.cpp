/********************************************************************
Saint Bus Engine 2.0 Connection API
---------------------------------------------------------------------
Revision Log:
  v1.0.0.2 
    - RequestDeviceList returns "ERROR" instead of "" when not connected
  v1.0.0.3 ** Use engine 1.10 or later.	
    - changed Engine Auto-start to use SBE registry values instead of sbe2.cfg
  v1.0.0.4
    - Changed auto-start to only execute with local SBE2
 	v1.0.0.5 ** Use engine v1.10.1.0 or later.
    - Added absolute timestamp to datacallback
	v1.0.0.6 ** Use engine v1.12.0.0 or later.
	-added FTDI functions for virtual Saint

********************************************************************/

#include "CAN_SAINT_stdafx.h"
#include "SBEConnection.h"
#include <io.h> //the next 3 are for checking if we should minimize the engine
#include <stdio.h>
#include <fcntl.h>
#include "Windows.h"
#include <string>

//Saint Bus Engine 2 Class Name
#define CLASS_NAME "#32770"
#define STANDARD_ENGINE_PATH "C:\\Program Files\\Saint Bus Engine 2\\"
#define ENGINE_EXE "Saint Bus Engine 2.exe"

//Error Reporting for autostart
void StartEngine(LPVOID pPtr);
CString GetError(DWORD value);

/********************************************************************
Function: CSBEConnection	
--------------------------------------------------------------------
Description:	

  This is the object constructor.  All member variables are 
  initialized here.  

********************************************************************/

CSBEConnection::CSBEConnection()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int error;

  // Init member variable
  this->ThreadRunning = true;
  this->pDataCallback = NULL;
  this->pFTDICallback = NULL;
  this->Connected = false;

  this->KATThreadRunningEvent = CreateEvent(NULL, FALSE, FALSE, NULL); //KeepAlive
  this->TXThreadRunningEvent = CreateEvent(NULL, FALSE, FALSE, NULL); //Transmit
  this->DCBThreadRunningEvent = CreateEvent(NULL, FALSE, FALSE, NULL); //DataCallback
  this->EngineStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL); //EngineStart

  // Set the version of WinSock that you want 
  wVersionRequested = MAKEWORD( 2, 2 );
  // Initialize WinSock for network access
  error = WSAStartup( wVersionRequested, &wsaData );

  //SBE autostart is here in the constructor and in the connect functions
  //This  allows the engine to start again without restarting the plugin. they just have to reconnect.
  StartEngine(this);
  WaitForSingleObject(this->EngineStartEvent,1000);
}



/********************************************************************
Function: ~CSBEConnection	
--------------------------------------------------------------------
Description:	

  This is the object destructor.  This function is called when the
  object is just about to be destroyed.  All manually allocated
  memory must be freed here.  

********************************************************************/
CSBEConnection::~CSBEConnection()
{
  this->Disconnect();
}

/********************************************************************
Function: SetDataCallback	
--------------------------------------------------------------------
Description:	

  This function sets the data callback function.  The data callback 
  function gets called each time a connected device gets a new
  message.

********************************************************************/
void CSBEConnection::SetDataCallback(DATAEVENTCALLBACK pDataCallback)
{
  this->pDataCallback = pDataCallback;
}

/********************************************************************
Function: SetFTDICallback	
--------------------------------------------------------------------
Description:	

  This function sets the FTDI callback function.  The data callback 
  function gets called each time a connected device gets a new
  message.

********************************************************************/
void CSBEConnection::SetFTDICallback(DATAEVENTCALLBACK pDataCallback)
{
  this->pFTDICallback = pDataCallback;
}

/********************************************************************
Function: RequestDeviceList	
--------------------------------------------------------------------
Description:	

  This function sends a request to the Saint Bus Engine to get a list
  of physically connected devices.

********************************************************************/
CString CSBEConnection::RequestDeviceList(void)
{
  CString result;
  try
  {
    /* Set up the notification event */
    this->RequestDevicesEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
    /* Issue the command */
    SendCommand(REQUEST_DEVICES,NULL,0);
    /* Wait for the response to come back */
    if(WaitForSingleObject(this->RequestDevicesEvent, 1000) == WAIT_OBJECT_0)
    {
      /* Set the result */
      result = this->DeviceList;
    }
    else
    {
      result = "ERROR";
    }
  }
  catch(...)
  {
    TRACE("***** Exception caught in RequestDeviceList *****\n");
  }
  return result;
}

/********************************************************************
Function: AmIFirst
--------------------------------------------------------------------
Description:

  This function determines if the plugin was first to connect
  to the SBE

********************************************************************/
bool CSBEConnection::AmIFirst(void)
{
  bool result;
  result = false;

  /*Set up notification event*/
  this->AmIFirstEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

  /*Issue the command*/
  SendCommand(AM_I_FIRST, NULL, 0);

  /*wait for response from SBE*/
  if(WaitForSingleObject(this->AmIFirstEvent, 1000) == WAIT_OBJECT_0)
  {
    /* Set the result */
    result = this->AmIFirstResult;
  }
  return result;
}

/********************************************************************
Function: GetEngineVersion	
--------------------------------------------------------------------
Description:	

  This function requests the version string from the Saint Bus Engine.

********************************************************************/
CString CSBEConnection::GetEngineVersion(void)
{
  return this->EngineVersion;
}

/********************************************************************
Function: DisconnectFromDevice	
--------------------------------------------------------------------
Description:

  This function disconnects the client from the given device ID.

********************************************************************/
void CSBEConnection::DisconnectFromDevice(int DeviceID)
{
  if(Connected)
  {
    // Send the command to the engine.
    SendCommand(DISCONNECT_FROM_DEVICE, &DeviceID, sizeof(int));
  }
}

/********************************************************************
Function: SendDataToDevice	
--------------------------------------------------------------------
Description:	

  This function sends the given data to the given device ID.  The
  data is first FF packed and the FF 00 appended.  The data is then
  sent off to the engine.

********************************************************************/
void CSBEConnection::SendFTDIToDevice(int DeviceID, unsigned __int8 *pData, unsigned __int32 Length)
{
  COMMAND_DATA_PACKET *pPacket;
  DEVICE_DATA_PACKET *pDeviceData;
  unsigned __int32   FFCount;
  unsigned int i;
  unsigned int j;
  unsigned int TotalLength;

  try
  {
    if((Length > 0) && Connected)
    {
      /* Scan for FFs */
      FFCount = 0;
      for(i = 0; i < Length; ++i)
      {
        if(pData[i] == 0xFF)
        {
          ++FFCount;
        }
      }

      /* Set up the packet */
      TotalLength = Length + sizeof(COMMAND_DATA_PACKET) + sizeof(DEVICE_DATA_PACKET) + FFCount + 2; //-4
      pPacket = (COMMAND_DATA_PACKET *)malloc(TotalLength);
      memset(pPacket, 0, TotalLength);
      pPacket->Command = FTDI_DATA_R;
      pPacket->Size = TotalLength;
      /* Embed the device ID into the packet */
      pDeviceData = (DEVICE_DATA_PACKET *)pPacket->Data;
      pDeviceData->DeviceID = DeviceID;
      pDeviceData->Size = Length + FFCount + 2;

      /* Copy data into the packet */
      j = 0;
      for(i = 0; i < Length; ++i)
      {
        // Pack FFs
        if(pData[i] == 0xFF)
        {
          pDeviceData->Data[j] = 0xFF;
          ++j;
          pDeviceData->Data[j] = 0xFF;
          ++j;
        }
        else
        {
          pDeviceData->Data[j] = pData[i];
          ++j;
        }
      }

      /* Add message termination */
      pDeviceData->Data[j] = 0xFF;
      ++j;
      pDeviceData->Data[j] = 0x00;

      /* Add the data to the TX list */
      this->TXDataList.AddTail(pPacket);
      /* Notify Thread */
      SetEvent(this->TXThreadRunningEvent);
    }
  }
  catch(...)
  {
    TRACE("***** Exception caught in SendDataToDevice *****\n");
  }
}

/********************************************************************
Function: SendDataToDevice	
--------------------------------------------------------------------
Description:	

  This function sends the given data to the given device ID.  The
  data is first FF packed and the FF 00 appended.  The data is then
  sent off to the engine.

********************************************************************/
void CSBEConnection::SendDataToDevice(int DeviceID, unsigned __int8 *pData, unsigned __int32 Length)
{
  COMMAND_DATA_PACKET *pPacket;
  DEVICE_DATA_PACKET *pDeviceData;
  unsigned __int32   FFCount;
  unsigned int i;
  unsigned int j;
  unsigned int TotalLength;

  try
  {
    if((Length > 0) && Connected)
    {
      /* Scan for FFs */
      FFCount = 0;
      for(i = 0; i < Length; ++i)
      {
        if(pData[i] == 0xFF)
        {
          ++FFCount;
        }
      }

      /* Set up the packet */
      TotalLength = Length + sizeof(COMMAND_DATA_PACKET) + sizeof(DEVICE_DATA_PACKET) + FFCount + 2; //-4
      pPacket = (COMMAND_DATA_PACKET *)malloc(TotalLength);
      memset(pPacket, 0, TotalLength);
      pPacket->Command = DEVICE_DATA;
      pPacket->Size = TotalLength;
      /* Embed the device ID into the packet */
      pDeviceData = (DEVICE_DATA_PACKET *)pPacket->Data;
      pDeviceData->DeviceID = DeviceID;
      pDeviceData->Size = Length + FFCount + 2;

      /* Copy data into the packet */
      j = 0;
      for(i = 0; i < Length; ++i)
      {
        // Pack FFs
        if(pData[i] == 0xFF)
        {
          pDeviceData->Data[j] = 0xFF;
          ++j;
          pDeviceData->Data[j] = 0xFF;
          ++j;
        }
        else
        {
          pDeviceData->Data[j] = pData[i];
          ++j;
        }
      }

      /* Add message termination */
      pDeviceData->Data[j] = 0xFF;
      ++j;
      pDeviceData->Data[j] = 0x00;

      /* Add the data to the TX list */
      this->TXDataList.AddTail(pPacket);
      /* Notify Thread */
      SetEvent(this->TXThreadRunningEvent);
    }
  }
  catch(...)
  {
    TRACE("***** Exception caught in SendDataToDevice *****\n");
  }
}

/********************************************************************
Function: SendDataToDevice	
--------------------------------------------------------------------
Description:	

  This function is the same as the one above, but accommodates a 
  string input instead of raw data.

********************************************************************/
void CSBEConnection::SendDataToDevice(int DeviceID, char *pStrData)
{
  unsigned __int8 *pData;
  DWORD Len;
  unsigned __int32 strLength;
  CString Temp;

  // Convert to CString
  Temp = pStrData;
  // Remove spaces
  Temp.Remove(' ');
  // Get string length
  strLength = Temp.GetLength();
  // Check length
  if(strLength > 0)
  {
    // Alloc RAM for the data
    pData = (unsigned __int8 *)malloc(strLength);
    // Convert the string to data
    String2Hex(Temp.GetBuffer(1), pData, strLength, &Len);
    // Send it out!
    SendDataToDevice(DeviceID, pData, Len);
    // free the RAM
   // free(pData);
  }	
}

/********************************************************************
Function: SendFTDIToDevice	
--------------------------------------------------------------------
Description:	

  This function is the same as the one above, but accommodates a 
  string input instead of raw data.

********************************************************************/
void CSBEConnection::SendFTDIToDevice(int DeviceID, char *pStrData)
{
  unsigned __int8 *pData;
  DWORD Len;
  unsigned __int32 strLength;
  CString Temp;

  // Convert to CString
  Temp = pStrData;
  // Remove spaces
  Temp.Remove(' ');
  // Get string length
  strLength = Temp.GetLength();
  // Check length
  if(strLength > 0)
  {
    // Alloc RAM for the data
    pData = (unsigned __int8 *)malloc(strLength);
    // Convert the string to data
    String2Hex(Temp.GetBuffer(1), pData, strLength, &Len);
    // Send it out!
    SendFTDIToDevice(DeviceID, pData, Len);
    // free the RAM
   // free(pData);
  }	
}

/********************************************************************
Function: SendDataToDeviceRaw	
--------------------------------------------------------------------
Description:	

  This function sends a buffer just as it is, no FF packing and no
  FF 00.

********************************************************************/
void CSBEConnection::SendDataToDeviceRaw(int DeviceID, unsigned __int8 *pData, unsigned __int32 Length)
{
  COMMAND_DATA_PACKET *pPacket;
  DEVICE_DATA_PACKET *pDeviceData;
  try
  {
    if((Length > 0) && Connected)
    {
      /* Set up the packet */
      pPacket = (COMMAND_DATA_PACKET *)malloc(Length + sizeof(COMMAND_DATA_PACKET) + sizeof(DEVICE_DATA_PACKET));
      pPacket->Command = DEVICE_DATA;
      pPacket->Size = Length + sizeof(COMMAND_DATA_PACKET) + sizeof(DEVICE_DATA_PACKET);
      /* Embed the device ID into the packet */
      pDeviceData = (DEVICE_DATA_PACKET *)pPacket->Data;
      pDeviceData->DeviceID = DeviceID;
      pDeviceData->Size = Length;
      /* Copy data into the packet */
      memcpy(pDeviceData->Data,pData,Length);
      /* Add the data to the TX list */
      this->TXDataList.AddTail(pPacket);
      /* Notify thread */
      SetEvent(this->TXThreadRunningEvent);
    }
  }
  catch(...)
  {
    TRACE("***** Exception caught in SendDataToDevice *****\n");
  }
}

/********************************************************************
Function: SendDataToDeviceRaw	
--------------------------------------------------------------------
Description:	

  This function is the same as above but accommodates a string input.

********************************************************************/
void CSBEConnection::SendDataToDeviceRaw(int DeviceID, char *pStrData)
{
  unsigned __int8 *pData;
  DWORD Len;
  unsigned __int32 strLength;
  CString Temp;

  // Convert to CString
  Temp = pStrData;
  // remove spaces
  Temp.Remove(' ');
  // Get length
  strLength = Temp.GetLength();


  if(strLength > 0)
  {
    // Alloc RAM for the data
    pData = (unsigned __int8 *)malloc(strLength);
    // Convert the string to data
    String2Hex(Temp.GetBuffer(1), pData, strLength, &Len);
    // Send it out!
    SendDataToDeviceRaw(DeviceID, pData, Len);
    // free the RAM
   // free(pData);
  }
}

/********************************************************************
Function: OpenAllDevices	
--------------------------------------------------------------------
Description:	

  This function sends the command to the engine to open all devices.

********************************************************************/
void CSBEConnection::OpenAllDevices(void)
{
  SendCommand(OPEN_ALL_DEVICES,NULL,0);		
}

/********************************************************************
Function: CloseAllDevices	
--------------------------------------------------------------------
Description:	

  This function sends the command to the engine to close all devices.

********************************************************************/
void CSBEConnection::CloseAllDevices(void)
{
  SendCommand(CLOSE_ALL_DEVICES,NULL,0);
}

/********************************************************************
Function: OpenDeviceByName	
--------------------------------------------------------------------
Description:	

  This function 

********************************************************************/
bool CSBEConnection::OpenDeviceByName(CString DeviceName)
{
  bool result;

  result = false;
  OpenDeviceResult = false;

  if(DeviceName.GetLength() > 0)
  {
    this->OpenCloseDeviceEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

    SendCommand(OPEN_DEVICE_BY_NAME, DeviceName.GetBuffer(1), DeviceName.GetLength() + 1);

    /* Wait for the response to come back */
    if(WaitForSingleObject(this->OpenCloseDeviceEvent, 1000) == WAIT_OBJECT_0)
    {
      /* Set the result */
      result = OpenDeviceResult;
    }
  }

  return(result);
}

/********************************************************************
Function: CloseDeviceByName	
--------------------------------------------------------------------
Description:	

  This function 

********************************************************************/
void CSBEConnection::CloseDeviceByName(CString DeviceName)
{
  if(DeviceName.GetLength() > 0)
  {		
    SendCommand(CLOSE_DEVICE_BY_NAME, DeviceName.GetBuffer(1), DeviceName.GetLength() + 1);
  }
}

/********************************************************************
Function: OpenDeviceByID	
--------------------------------------------------------------------
Description:	

  This function 

********************************************************************/
bool CSBEConnection::OpenDeviceByID(int DeviceID)
{
  bool result;

  result = false;
  OpenDeviceResult = false;

  if(DeviceID < MAX_DEVICES)
  {
    this->OpenCloseDeviceEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

    SendCommand(OPEN_DEVICE_BY_ID, &DeviceID, sizeof(DeviceID));

    /* Wait for the response to come back */
    if(WaitForSingleObject(this->OpenCloseDeviceEvent, 1000) == WAIT_OBJECT_0)
    {
      /* Set the result */
      result = OpenDeviceResult;
    }
  }
  return(result);
}

/********************************************************************
Function: CreateVirtualSaint
--------------------------------------------------------------------
Description:	

  This function creates a virtual Saint

********************************************************************/
bool CSBEConnection::CreateVirtualSaint()
{
	SendCommand(CREATE_VS,  NULL, NULL);
	return true;
}

/********************************************************************
Function: DestroyVirtualSaint	
--------------------------------------------------------------------
Description:	

  This function creates a virtual Saint

********************************************************************/
bool CSBEConnection::DestroyVirtualSaint(int DeviceID)
{
	SendCommand(DESTROY_VS, &DeviceID, sizeof(DeviceID));
	return true;
}


/********************************************************************
Function: CloseDeviceByID	
--------------------------------------------------------------------
Description:	

  This function 

********************************************************************/
void CSBEConnection::CloseDeviceByID(int DeviceID)
{
  if(DeviceID < MAX_DEVICES)
  {
    SendCommand(CLOSE_DEVICE_BY_ID, &DeviceID, sizeof(DeviceID));
  }
}

/********************************************************************
Function: SendCommand	
--------------------------------------------------------------------
Description:	

  This function sends a command to the Saint Bus Engine.

********************************************************************/
void CSBEConnection::SendCommand(COMMAND Command, void *pData, unsigned __int32 len)
{
  COMMAND_DATA_PACKET *pPacket;

  if(Connected || (Command == CLIENT_CONNECT))
  {
    /* Set up the packet */
    pPacket = (COMMAND_DATA_PACKET *)malloc(len + sizeof(COMMAND_DATA_PACKET)); 
    /* Set the command */
    pPacket->Command = Command;
    /* Set the length of the command */
    pPacket->Size = len + sizeof(COMMAND_DATA_PACKET); // - 3 due to struct layout

    if(pData != NULL) /* Some commands can have no data (CLIENT_DISCONNECT) */
    {
      memcpy(pPacket->Data, pData, len); /* copy data to the packet */
    }
    /* Add the data to the TX list */
    this->TXDataList.AddTail(pPacket);
    /* Notify Thread */
    SetEvent(this->TXThreadRunningEvent);
  }
}

/********************************************************************
Function: ConnectToDevice	
--------------------------------------------------------------------
Description:	

  This function connects the client to a device connected to the 
  Saint Bus Engine.  If the call is successful, then a valid device
  ID is returned, otherwise 0xFF is returned.

********************************************************************/
int CSBEConnection::ConnectToDevice(CString DeviceName)
{
  try
  {
    if(DeviceName.GetLength() > 0)
    {
      /* Set up the notification event */
      this->ConnectToDeviceEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
      /* Issue the command to the engine */
      SendCommand(CONNECT_TO_DEVICE, DeviceName.GetBuffer(1), DeviceName.GetLength() + 1);
      /* Wait for the response to come back */
      if(WaitForSingleObject(this->ConnectToDeviceEvent, 1000) == WAIT_OBJECT_0)
      {
        /* the command returns FF if the connect failed */
        if(DeviceID < 0xFF)
        {
          //TRACE("***** Connected to %s *****\n",DeviceName);
        }
        else
        {
          TRACE("***** Failed to connect *****\n");
        }
        /* return the device ID */
        return this->DeviceID;
      }
      else
      {
        TRACE("***** Failed to get response to device connect *****\n");
      }
    }
  }
  catch(...)
  {
    TRACE("***** Exception caught in ConnectToDevice *****\n");
  }

  /* return failure */
  return 0xFF;
}

/********************************************************************
Function: Disconnect	
-------------------------------------------------------------------
Description:	


********************************************************************/
void CSBEConnection::Disconnect(void)
{
  try
  {
    TRACE("***** Disconnect Called *****\n");

    if(Connected)
    {	
      /* Send command to disconnect from the engine */
      SendCommand(CLIENT_DISCONNECT, NULL, 0); 
      Connected = false;
      Sleep(100);
      /* Kill threads */
      ThreadRunning = false;
      SetEvent(this->KATThreadRunningEvent);
      SetEvent(this->TXThreadRunningEvent);
      SetEvent(this->DCBThreadRunningEvent);
      Sleep(100);
      /* Close engine connection */
      closesocket(this->Connection);
    }
  }
  catch(...)
  {
    TRACE("***** Exception caught in Disconnect *****\n");
  }
}

/********************************************************************
Function: SetFilters
--------------------------------------------------------------------
Description:

  This function allows the plugins to set filters for the SBE2.
  It will return true if the filter was set successfully or false
  if the filter was not valid.

********************************************************************/
bool CSBEConnection::SetFilter(CString filter)
{
  bool result;
  result = false;

  /*Set up notification event*/
  this->SetFilterEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

  /*Issue the command*/
  SendCommand(SET_FILTER, filter.GetBuffer(1), filter.GetLength() + 1);

  /*wait for response from SBE*/
  if(WaitForSingleObject(this->SetFilterEvent, 1000) == WAIT_OBJECT_0)
  {
    /* Set the result */
    result = this->SetFilterResult;
  }

  return result;
}

/********************************************************************
Function: CloseFilters
--------------------------------------------------------------------
Description:

  This function allows the plugins to close filters for the SBE2.

********************************************************************/
bool CSBEConnection::CloseFilter(int handle)
{
  bool result;
  result = true;

  /*Set up notification event*/
  this->CloseFilterEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

  /*Issue the command*/
  SendCommand(CLOSE_FILTER, &handle, sizeof(handle));

  if(WaitForSingleObject(this->CloseFilterEvent, 1000) == WAIT_OBJECT_0)
  {
    result = this->CloseFilterResult;
  }
  return result;
}

/********************************************************************
Function: RequestFilterList
-------------------------------------------------------------------
Description:	 

  This function sends a request to the Saint Bus Engine to get a list
  of currently activated filters.

********************************************************************/
CString CSBEConnection::RequestFilterList(void)
{
  CString result;
  try
  {
    /* Set up the notification event */
    this->RequestFiltersEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
    /* Issue the command */
    SendCommand(REQUEST_FILTERS,NULL,0);
    /* Wait for the response to come back */
    if(WaitForSingleObject(this->RequestFiltersEvent, 1000) == WAIT_OBJECT_0)
    {
      /* Set the result */
      result = this->FilterList;
    }
    else
    {
      result = "ERROR";
    }
  }
  catch(...)
  {
    TRACE("***** Exception caught in RequestFilterList *****\n");
  }
  return result;
}

/********************************************************************
Function: Enable	
--------------------------------------------------------------------
Description:	

  This function will either activate or deactivate currently 
  enabled filters.

********************************************************************/
bool CSBEConnection::Enable(bool enable)
{
  bool result;
  result = false;
  /*Set up notification event*/
  this->SetEnableEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

  /*Issue the command*/
  SendCommand(GLOBAL_ENABLE_FILTERS, &enable, sizeof(enable));

  /*wait for response from SBE*/
  if(WaitForSingleObject(this->SetEnableEvent, 1000) == WAIT_OBJECT_0)
  {
    /* Set the result */
    result = this->SetEnableResult;
  }
  return result;
}

/********************************************************************
Function: IsConnected	
--------------------------------------------------------------------
Description:	

  This function returns the connection state of the object.

********************************************************************/
bool CSBEConnection::IsConnected(void)
{
  return Connected;
}

/********************************************************************
Function: Connect	
--------------------------------------------------------------------
Description:	

  This is a simplified Connect function that connects to a local 
  SBE2 on the default port.

********************************************************************/
bool CSBEConnection::Connect(CString ApplicationName)
{
  return(this->Connect("localhost", ENGINE_PORT, ApplicationName));
}

/********************************************************************
Function: Connect	
--------------------------------------------------------------------
Description:	

  This function connects to a SBE2 at the given address on the 
  given port.  The address can be a server name or IP address.

********************************************************************/
bool CSBEConnection::Connect(CString address, unsigned __int16 port, CString ApplicationName)
{
  if((address == "localhost") || (address == "127.0.0.1"))
  {
    TRACE("STARTING THE ENGINE...\n");
    StartEngine(this); //This is done in both the constructor and on connect to allow the engine to start again
                       //without restarting the plugin. they just have to reconnect.
    WaitForSingleObject(this->EngineStartEvent, 1000);
  }

  TRACE("CONNECTING...\n");
  int error;
  unsigned long ThreadID;
  try
  {
    // Check if already connected
    if(this->Connected)
    {
      return true;
    }

    // Check for a valid address
    if(address.GetLength() > 0)
    {
      // try to get IP address
      IPAddr = inet_addr(address);
      if(IPAddr != INADDR_NONE)
      {
        // Try to get host
        pHost = gethostbyaddr((char*)&IPAddr, 4, PF_INET);
        if(pHost == NULL)
        {
          TRACE("***** Could not find the given address %04x *****\n", IPAddr);
          return false;
        }
      }
      else
      {
        // Check for host name
        pHost = gethostbyname(address);

        if(pHost == NULL)
        {
          TRACE("***** Could not resolve the given address %s *****\n", address);
          return false;
        }
      }
    }
    else
    {	
      return false;
    }

    // Create connection socket
    Connection = socket(AF_INET, SOCK_STREAM, 0);
    // Copy in the address
    memcpy(&ServerAddr.sin_addr, pHost->h_addr_list[0], pHost->h_length);
    // Set to INET connection type
    ServerAddr.sin_family = AF_INET;
    // Set the port
    ServerAddr.sin_port = htons(port);
    // Attempt to connect
    error = connect(Connection, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));
    // Check for errors
    if(error == 0)
    {
      // Set the application name
      this->ApplicationName = ApplicationName;
      // Set up the thread flags
      this->ThreadRunning = true;
      // Start communication threads
      TRACE("Creating Threads...\n");
      CreateThread(NULL, 0, &ClientTXThread, reinterpret_cast< void* >(this), 0, &ThreadID);      // Start the TX thread
      CreateThread(NULL, 0, &ClientRXThread, reinterpret_cast< void* >(this), 0, &ThreadID);      // Start the RX thread
      CreateThread(NULL, 0, &DataCallBackThread, reinterpret_cast< void* >(this), 0, &ThreadID);  // Start the DCB thread

      this->EngineConnectEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
      // Send connect command to the engine
      SendCommand(CLIENT_CONNECT, this->ApplicationName.GetBuffer(1), this->ApplicationName.GetLength() + 1);

      if(WaitForSingleObject(this->EngineConnectEvent,5000) == WAIT_OBJECT_0)
      {
        TRACE("***** CONNECTED!!! *****\n");
        CreateThread(NULL, 0, &KeepAliveThread, reinterpret_cast< void* >(this), 0, &ThreadID);   // Start the Keep alive thread
        this->EngineVersionReceivedEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
        SendCommand(REQUEST_VERSION,NULL,0);
        WaitForSingleObject(this->EngineVersionReceivedEvent,1000);
        return true;
      }
      else
      {
        TRACE("***** Engine failed to respond to Connect command *****\n");
        return false;
      }

      CloseHandle(this->EngineConnectEvent);
    }
    else
    {
      TRACE("***** CONNECT FAILED!! *****\n");
      return false;
    }
  }
  catch(...)
  {
    return false;
  }
}

/********************************************************************
Function: ClientTXThread	
--------------------------------------------------------------------
Description:	


********************************************************************/
DWORD WINAPI CSBEConnection::ClientTXThread(LPVOID pPtr)
{
  CSBEConnection *pConnection;
  COMMAND_DATA_PACKET *pDataPacket;
  COMMAND_DATA_PACKET KeepAlivePacket;
  int error;

  KeepAlivePacket.Command = CLIENT_KEEPALIVE;
  KeepAlivePacket.Size = sizeof(COMMAND_DATA_PACKET);

  try
  {
    pConnection = (CSBEConnection *)pPtr;

    while(pConnection->ThreadRunning)
    {
      pDataPacket = (COMMAND_DATA_PACKET *)pConnection->TXDataList.RemoveHead();
      while(pDataPacket != NULL || pConnection->SendKeepAlive)
      {
        // Give KeepAlive messages priority
        if(pConnection->SendKeepAlive)
        {
          pConnection->SendKeepAlive = false;
          error = send(pConnection->Connection, (char *)&KeepAlivePacket,KeepAlivePacket.Size,0);
        }
        // Send anything else
        if(pDataPacket != NULL)
        {
          /* Shove the data into the socket */
          error = send(pConnection->Connection, (char *)pDataPacket,pDataPacket->Size,0);

          if(error == SOCKET_ERROR)
          {
            TRACE("***** WINSOCK error %d *****\n", WSAGetLastError());
            pConnection->Disconnect();
            //free(pDataPacket);
            return(0);
          }

          // Client is disconnecting
          if(pDataPacket->Command == CLIENT_DISCONNECT)
          {
           // free(pDataPacket);
            TRACE("***** TX Thread saw CLIENT_DISCONNECT *****\n");
            return 0;			// End the TX thread
          }
          else
          {
            //free(pDataPacket);
          }
        }
        // Get the next packet from the list
        pDataPacket = (COMMAND_DATA_PACKET *)pConnection->TXDataList.RemoveHead();
      }

      WaitForSingleObject(pConnection->TXThreadRunningEvent, 100);
    }
  }
  catch(...)
  {
    TRACE("***** Exception caught in SBEConnection TX Thread *****\n");
  }
  return 0;
}

/********************************************************************
Function: ClientRXThread
--------------------------------------------------------------------
Description:	

  This thread continuously receives commands from the SBE2 and 
  processes the data. 

********************************************************************/
DWORD WINAPI CSBEConnection::ClientRXThread(LPVOID pPtr)
{
  CSBEConnection		*pConnection;
  unsigned __int32	Size;
  COMMAND_DATA_PACKET	*pPacket;
  unsigned __int32	len;

  try
  {
    // Get pointer to the main object.
    pConnection = (CSBEConnection *)pPtr;
    // Check to see if the application is still running.
    while(pConnection->ThreadRunning)
    {
      // Attempt to get a command from the socket, first data is the length of the packet
      len = recv(pConnection->Connection,(char *)&Size,sizeof(Size),0);	
      // Check to see if we got anything
      if(len == sizeof(Size))
      {			
        //TRACE("***** Trying to get packet of size %d *****\n", Size);
        // Allocate memory for the incoming packet
        pPacket = (COMMAND_DATA_PACKET *)malloc(Size);
        // Get the rest of the packet
        len = recv(pConnection->Connection, (char *)&pPacket->Command, Size - sizeof(Size), 0);
        // Set the size
		if(!pPacket){
			continue;
		}
        pPacket->Size = Size;
        // Check for data
        if(len == (Size - sizeof(Size)))
        {
          switch(pPacket->Command)
          {
          case DEVICE_DATA:
            {
              // This is a message from a device, add to list
              if(pConnection->pDataCallback != NULL)
              {
                pConnection->RXDataList.AddTail(pPacket);
                SetEvent(pConnection->DCBThreadRunningEvent);
              }
              else
              {
                // No callback yet
               // free(pPacket);
              }
            }
            break;
          case REQUEST_VERSION:
            {
              // Version information from SBE2
              pConnection->EngineVersion = (char *)pPacket->Data;
              TRACE("***** Got Engine Version = %s *****\n", pConnection->EngineVersion);
              SetEvent(pConnection->EngineVersionReceivedEvent);
             // free(pPacket);
            }
            break;
          case REQUEST_DEVICES:
            {
              // SBE2 responded to the REQUEST_DEVICES command
              //TRACE("***** Got REQUEST_DEVICES response *****\n");
              pConnection->DeviceList = (char *)pPacket->Data;
              SetEvent(pConnection->RequestDevicesEvent);
              //free(pPacket);
            }
            break;
          case CONNECT_TO_DEVICE:
            {
              // SBE2 responded to the CONNECT_TO_DEVICE command
              pConnection->DeviceID = *(unsigned __int32 *)pPacket->Data;
              SetEvent(pConnection->ConnectToDeviceEvent);
              //free(pPacket);
            }
            break;
          case CLIENT_CONNECT:
            {
              // SBE2 acked our connection attempt
              TRACE("***** Connect Ack!!! *****\n");
              pConnection->Connected = true;
              SetEvent(pConnection->EngineConnectEvent);
             // free(pPacket);
            }
            break;
          case OPEN_DEVICE_BY_NAME:
            {
              pConnection->OpenDeviceResult = *(bool *)pPacket->Data;
              SetEvent(pConnection->OpenCloseDeviceEvent);
             // free(pPacket);
            }
            break;
          case OPEN_DEVICE_BY_ID:
            {
              pConnection->OpenDeviceResult = *(bool *)pPacket->Data;
              SetEvent(pConnection->OpenCloseDeviceEvent);
            //  free(pPacket);
            }
            break;
          case AM_I_FIRST:
            {
              pConnection->AmIFirstResult = *(bool *)pPacket->Data;
              SetEvent(pConnection->AmIFirstEvent);
             // free(pPacket);
            }
            break;
          case SET_FILTER:
            {
              pConnection->SetFilterResult = *(bool *)pPacket->Data;
              SetEvent(pConnection->SetFilterEvent);
            //  free(pPacket);
            }
            break;
          case CLOSE_FILTER:
            {
              pConnection->CloseFilterResult = *(bool *)pPacket->Data;
              SetEvent(pConnection->CloseFilterEvent);
             // free( pPacket );
            }
            break;
          case REQUEST_FILTERS:
            {
              pConnection->FilterList = (char *)pPacket->Data;
              SetEvent(pConnection->RequestFiltersEvent);
            //  free(pPacket);
            }
            break;
          case GLOBAL_ENABLE_FILTERS:
            {
              pConnection->SetEnableResult = *(bool *)pPacket->Data;
              SetEvent(pConnection->SetEnableEvent);
             // free(pPacket);
            }
          case DEVICE_LIST_CHANGED:
            {
              // Figure out what to do here...
            }
		  case FTDI_DATA:
			{
				// This is a message from a device, add to list
              if(pConnection->pFTDICallback != NULL)
              {
                pConnection->RXDataList.AddTail(pPacket);
                SetEvent(pConnection->DCBThreadRunningEvent);
              }
			  else if(pConnection->pDataCallback != NULL)
              {
                pConnection->RXDataList.AddTail(pPacket);
                SetEvent(pConnection->DCBThreadRunningEvent);
              }
              else
              {
                // No callback yet
              //  free(pPacket);
              }
			  
			}	
          default:
            TRACE("You received: %d\n",pPacket->Command);
          }
        }
        else if(len == SOCKET_ERROR)
        {
          TRACE("***** Socket error in RX Thread *****\n");
          //free(pPacket);
          pConnection->Disconnect();
          return(0);
        }
      }
      else if (len == SOCKET_ERROR)
      {
        pConnection->Disconnect();
        TRACE("***** Socket error in RX Thread *****\n");
        return(0);
      }
    }
  }
  catch(...)
  {
    TRACE("***** Exception caught in SBEConnection RX Thread *****\n");
  }
  return 0;
}

/********************************************************************
Function: DataCallBackThread
--------------------------------------------------------------------
Description:	

  This function calls the application with the message data that
  is sent from the SBE2. 

********************************************************************/
DWORD WINAPI CSBEConnection::DataCallBackThread(LPVOID pPtr)
{
  CSBEConnection *pConnection;
  COMMAND_DATA_PACKET *pData;
  DEVICE_DATA_PACKET *pDeviceData;
	// Absolute timestamp variables
	unsigned __int8 *bytePtr;
	unsigned __int32 length;
  ABS_TIMESTAMP *pAbsTimestamp = (ABS_TIMESTAMP *)calloc(1, sizeof(ABS_TIMESTAMP));

  TRACE("***** DataCallBackThread starting... *****\n");

	try
	{
		// Get pointer to the main object
		pConnection = (CSBEConnection *)pPtr;

		// Check to see if the application is still running
		while(pConnection->ThreadRunning)
		{
		  // Check if the user has registered a call back yet.
			if(pConnection->pDataCallback != NULL)
			{
				pData = (COMMAND_DATA_PACKET *)pConnection->RXDataList.RemoveHead();
				if(pData != NULL && pData->Command == DEVICE_DATA)
				{
					while(pData != NULL)
					{
						if(pConnection->pDataCallback != NULL)
						{
							TRACE("***** Sending data back to client *****\n");

							// Extract the DEVICE_DATA_PACKET from the COMMAND_DATA_PACKET.
							pDeviceData = (DEVICE_DATA_PACKET *)pData->Data;
							// Get a byte pointer to the beginning of the message in the DEVICE_DATA_PACKET struct.
							bytePtr = (unsigned __int8 *)pDeviceData->Data;
							// Get the length of the message.
							length = pDeviceData->Size;
							// Copy absolute timestamp information into the ABS_TIMESTAMP struct.
							pAbsTimestamp->years = (((bytePtr[length] << 12) & 0x000FF000) | ((bytePtr[length + 1] << 4) & 0x0FF0) | ((bytePtr[length + 2] >> 4) & 0x0F));
							pAbsTimestamp->months = (bytePtr[length + 2] & 0x0F);
							pAbsTimestamp->days = ((bytePtr[length + 3] >> 3) & 0x1F);
							pAbsTimestamp->hours = (((bytePtr[length + 3] << 2) & 0x1C) | ((bytePtr[length + 4] >> 6) & 0x03));
							pAbsTimestamp->minutes = (bytePtr[length + 4] & 0x3F);
							pAbsTimestamp->seconds = ((bytePtr[length + 5] >> 2) & 0x3F);
							pAbsTimestamp->milliseconds = (((bytePtr[length + 5] << 8) & 0x0300) | bytePtr[length + 6]);

							TRACE("years: %d months: %d days: %d hours: %d minutes: %d seconds: %d milliseconds: %d\n",
							  pAbsTimestamp->years, pAbsTimestamp->months, pAbsTimestamp->days, pAbsTimestamp->hours,
							  pAbsTimestamp->minutes, pAbsTimestamp->seconds, pAbsTimestamp->milliseconds);

							// Call back to client with data & absolute timestamp.
							pConnection->pDataCallback(pDeviceData->DeviceID, pDeviceData->Data, pDeviceData->Size, pAbsTimestamp);
						}
						else
						{
							TRACE("***** Callback is null *****\n");
						}
						// Free the memory
						//free(pData);
						// Try to get the next packet
						pData = (COMMAND_DATA_PACKET *)pConnection->RXDataList.RemoveHead();

					}
				// Give it a rest
					WaitForSingleObject(pConnection->DCBThreadRunningEvent, 100);
				}
				else if(pData != NULL && pData->Command == FTDI_DATA)
				{
					while(pData != NULL)
					{
						if(pConnection->pFTDICallback != NULL)
						{
							TRACE("***** Sending data back to client *****\n");

							// Extract the DEVICE_DATA_PACKET from the COMMAND_DATA_PACKET.
							pDeviceData = (DEVICE_DATA_PACKET *)pData->Data;
				
							// Get a byte pointer to the beginning of the message in the DEVICE_DATA_PACKET struct.
							//bytePtr = (unsigned __int8 *)pDeviceData->Data;
							//bytePtr = (unsigned __int8*)pDeviceData->Data[0];
							// Get the length of the message.
							length = pData->Size;
							// Copy absolute timestamp information into the ABS_TIMESTAMP struct.
							/*
							pAbsTimestamp->years = (((bytePtr[length] << 12) & 0x000FF000) | ((bytePtr[length + 1] << 4) & 0x0FF0) | ((bytePtr[length + 2] >> 4) & 0x0F));
							pAbsTimestamp->months = (bytePtr[length + 2] & 0x0F);
							pAbsTimestamp->days = ((bytePtr[length + 3] >> 3) & 0x1F);
							pAbsTimestamp->hours = (((bytePtr[length + 3] << 2) & 0x1C) | ((bytePtr[length + 4] >> 6) & 0x03));
							pAbsTimestamp->minutes = (bytePtr[length + 4] & 0x3F);
							pAbsTimestamp->seconds = ((bytePtr[length + 5] >> 2) & 0x3F);
							pAbsTimestamp->milliseconds = (((bytePtr[length + 5] << 8) & 0x0300) | bytePtr[length + 6]);

							TRACE("years: %d months: %d days: %d hours: %d minutes: %d seconds: %d milliseconds: %d\n",
							pAbsTimestamp->years, pAbsTimestamp->months, pAbsTimestamp->days, pAbsTimestamp->hours,
							pAbsTimestamp->minutes, pAbsTimestamp->seconds, pAbsTimestamp->milliseconds);
							*/
							TRACE( "Virtual Received DATA!!! \n" );
							// Call back to client with data & absolute timestamp.
							pConnection->pFTDICallback(pDeviceData->DeviceID, pDeviceData->Data, pDeviceData->Size, NULL);
						}
						else
						{
							TRACE("***** Callback is null *****\n");
						}
					  // Free the memory
					  //free(pData);
					  // Try to get the next packet
					  pData = (COMMAND_DATA_PACKET *)pConnection->RXDataList.RemoveHead();

					}
					// Give it a rest
					WaitForSingleObject(pConnection->DCBThreadRunningEvent, 100);
				} 

				else  // For FTDI_DATA transmissions
				{
					// Start purging data, the user has not yet registered a callback
					pData = (COMMAND_DATA_PACKET *)pConnection->RXDataList.RemoveHead();

					if(pData != NULL)
					{
					  // Free memory
					//	free(pData);
					}
					// Give it a rest
					WaitForSingleObject(pConnection->DCBThreadRunningEvent, 100);
				}
			}
			else  // For FTDI_DATA transmissions
			{
				// Start purging data, the user has not yet registered a callback
				pData = (COMMAND_DATA_PACKET *)pConnection->RXDataList.RemoveHead();

				if(pData != NULL)
				{
				  // Free memory
				//  free(pData);
				}
				// Give it a rest
				WaitForSingleObject(pConnection->DCBThreadRunningEvent, 100);
			}
		} 
	}
	catch(...)
	{
		TRACE("***** Exception caught in SBEConnection DataCallback Thread *****\n");
	}

	free(pAbsTimestamp);

	TRACE("***** DataCallBackThread exiting *****\n");
	return 0;
}

/********************************************************************
Function: KeepAliveThread
--------------------------------------------------------------------
Description:	

  This function tells the TX thread that its time to send a keep
  alive packet to the SBE2.  The absence of the keep-alive tells
  the SBE2 that the client application has crashed.  The client is
  then removed from the list of connected clients.

********************************************************************/
DWORD WINAPI CSBEConnection::KeepAliveThread(LPVOID pPtr)
{
  CSBEConnection *pConnection;

  try
  {
    // Get pointer to the main object.
    pConnection = (CSBEConnection *)pPtr;

    while(pConnection->ThreadRunning)
    {
      // Set flag
      pConnection->SendKeepAlive = true;
      WaitForSingleObject(pConnection->KATThreadRunningEvent, 500);
    }
  }
  catch (...)
  {
    TRACE("***** Exception caught in KeepAliveThread *****\n");
  }

  return 0;	
}


/********************************************************************
Function: StartEngine
--------------------------------------------------------------------
Description:	

  Starts the engine using the registry key
  LOCAL_MACHINE->SOFTWARE->Saint Bus Engine 2->ProgramPath
  this should be created when the engine is installed.

********************************************************************/
void StartEngine(LPVOID pPtr)
{
  try
  {
    CSBEConnection *pConnection = (CSBEConnection *)pPtr;
    //Create or open Saint Bus Engine registry folder
    CString subKey = "SOFTWARE\\Saint Bus Engine 2"; // Path in the registry
    PHKEY newHandle = (PHKEY)malloc(sizeof(PHKEY));
    DWORD retVal;

    //RegCreateKeyEx will  open the key if it exists.
    retVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE,LPCTSTR(subKey),0,KEY_READ,newHandle);

    //Print out error if there was one
    if(retVal != ERROR_SUCCESS)
    {
      TRACE("Registry Failed: %s",GetError(retVal));
      return;
    }

    //Attempt to open the SBE upon a connection attempt
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si); //Leave si empty becuase engine v1.09 ignores it for some reason
    ZeroMemory( &pi, sizeof(pi) );

    //check to see if the engine is already open
    if( FindWindow(CLASS_NAME,"Saint Bus Engine 2") == NULL)
    {
      LPBYTE pvExeData = (LPBYTE)malloc(sizeof(TCHAR) * _MAX_PATH);
      DWORD word = sizeof(BYTE)*512;
      LPDWORD pcbData = (LPDWORD)(&word);

      //load EXE path
      retVal = RegQueryValueEx(*newHandle,(LPCTSTR)"PathToExe",0,NULL,pvExeData,pcbData);
      if(retVal != ERROR_SUCCESS)
      {
        TRACE("Get Exe Failed: %s",GetError(retVal));
        //free(pvExeData);
        return;
      }

      CString PathToEngine((LPSTR)pvExeData);
      CString PathWithEngine = PathToEngine + ENGINE_EXE;

      TRACE("Exe Path: %s\n",PathWithEngine);

      //Try to start the Saint Bus Engine 2
      if( !CreateProcess( NULL,   // No module name (use command line).
        TEXT((LPSTR)LPCTSTR(PathWithEngine)),	// Command line.
        NULL,             // Process handle not inheritable.
        NULL,             // Thread handle not inheritable.
        FALSE,            // Set handle inheritance to FALSE.
        0,                // No creation flags.
        NULL,             // Use parent's environment block.
        LPSTR(pvExeData),    // Use Saint Bus Engine's Directory
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
        )
      {
        TRACE("Get Value Failed: %s\n",GetError(GetLastError()));
      }
      else
      {
        //Minimize the Engine after it is called, I know its polling but o well
        //This fails if the engine's plugin folder is empty due to it's popups
        HWND Engine;
        WIN32_FIND_DATA FileInformation;
        HANDLE FileHandle;
        DWORD dwMaxValueLen;

        if( RegQueryInfoKey( *newHandle, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &dwMaxValueLen, NULL, NULL ) )
        {
          TRACE( "Get Max Length Failed\n");
        }
        dwMaxValueLen++; //allow for null

        LPBYTE pvPath = ( LPBYTE )malloc( sizeof( TCHAR ) * dwMaxValueLen );
        word = sizeof( BYTE ) * dwMaxValueLen;

        retVal = RegQueryValueEx( *newHandle, ( LPCTSTR )"PluginPath", 0, NULL, pvPath, pcbData );

        if(retVal != ERROR_SUCCESS)
        {
          TRACE("Get Exe Failed: %s",GetError(retVal));
        //  free(pvExeData);
        //  free(pvPath);
          return;
        }

        CString Data((LPSTR)pvPath);

        Data += "*.exe";

        FileHandle = FindFirstFile(Data, &FileInformation);

        if(FileHandle != INVALID_HANDLE_VALUE){
          for(int i = 0; i < 10; i++) //try 5 times
          {
            Sleep(100); //for a total of 1 second to allow Engine to start completely
            Engine = FindWindow(CLASS_NAME,"Saint Bus Engine 2");
            if(Engine != NULL)
            {
              LRESULT result = SendMessage(Engine,WM_SYSCOMMAND,SC_MINIMIZE,NULL);	
              if(RegCloseKey(*newHandle) == ERROR_SUCCESS)
                TRACE("Successfully Closed Registery Handle\n");
              break;
            }
          }
        }
       // free(pvPath);

        SetEvent(pConnection->EngineStartEvent);
      }
      //free(pvExeData);
    }
  }
  catch(...)
  {
    TRACE("***** General Exception in Start Engine *****\n");
  }
}

CString GetError(DWORD value)
{
  //Print out error
  LPVOID lpMsgBuf;

  FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    value,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
    (LPTSTR) &lpMsgBuf,
    0,
    NULL);

  return((char *)lpMsgBuf);
}
