#ifndef __SBECOMMON
#define __SBECOMMON

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ENGINE_PORT		8000

#define MAX_COM_PORTS	16
#define MAX_DEVICES		32
#define MAX_CLIENTS		32

const unsigned char h2s[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
const unsigned char s2h[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

typedef struct 
{
  unsigned __int32 Size;    //4
  unsigned __int8  Command; //1
  unsigned __int8  Data[1]; //1
} COMMAND_DATA_PACKET;      //Size = 8, Data[1] + 2 free bytes = 3 free bytes

typedef struct 
{
  int	DeviceID;            //4
  unsigned __int32 Size;   //4
  unsigned __int8 Data[1]; //1
} DEVICE_DATA_PACKET;      //Size = 12, Data[1] + 3 free bytes = 4 free bytes

typedef struct 
{
  unsigned __int32 years;
  unsigned __int8  months;
  unsigned __int8  days;
  unsigned __int8  hours;
  unsigned __int8  minutes;
  unsigned __int8  seconds;
  unsigned __int16 milliseconds;
} ABS_TIMESTAMP;

typedef enum
{
  CLIENT_CONNECT = 0,
  CLIENT_DISCONNECT = 1,
  CLIENT_KEEPALIVE = 2,
  REQUEST_DEVICES = 3,
  CONNECT_TO_DEVICE = 4,
  DISCONNECT_FROM_DEVICE = 5,
  DEVICE_DATA = 6,
  REQUEST_VERSION = 7,
  DEVICE_LIST_CHANGED = 8,
  DEVICE_RAW_DATA = 9,
  OPEN_ALL_DEVICES = 10,
  CLOSE_ALL_DEVICES = 11,
  OPEN_DEVICE_BY_NAME = 12,
  CLOSE_DEVICE_BY_NAME = 13,
  OPEN_DEVICE_BY_ID = 14,
  CLOSE_DEVICE_BY_ID = 15,
  AM_I_FIRST = 16,
  SET_FILTER = 17,
  CLOSE_FILTER = 18,
  REQUEST_FILTERS = 19,
  GLOBAL_ENABLE_FILTERS = 20,
  FTDI_DATA = 21,
  FTDI_DATA_R = 22,
  CREATE_VS = 23,
  DESTROY_VS = 24,
  UPDATE_TIMESTAMP = 25
} COMMAND;

typedef struct DATA_NODE 
{
  void *pData;
  DATA_NODE *pNext;
  DATA_NODE *pPrev;

} DATA_NODE;

/********************************************************************
Function:	  Hex2String
---------------------------------------------------------------------
Description:	

  This function converts a hex buffer into a string.

********************************************************************/
inline void Hex2String( unsigned __int8 *in, char *out, DWORD len, bool space )
{
  DWORD strpos = 0;
  for( DWORD i = 0; i < len; ++i )
  {
    out[strpos] = h2s[( in[i] >> 4 )];   //Get upper nibble.
    ++strpos;
    out[strpos] = h2s[( in[i] & 0x0f )]; //Get lower nibble;
    ++strpos;
    if( space )
    {
      out[strpos] = ' ';
      ++strpos;
    }		
  }
  out[strpos] = '\0'; //Add null char for string termination	
}

/********************************************************************
Function:	  String2Hex
---------------------------------------------------------------------
Description:	

  This function converts a string to a hex buffer

********************************************************************/
inline void String2Hex( char *in, unsigned __int8 *out, DWORD len, DWORD *hexLen )
{
  unsigned __int32 pos = 0;
  *hexLen = 0;
  unsigned __int8 hex;
  char str;
  bool upper = true;
  while( pos < len )
  {		
    str = in[pos];
    if( str >= '0' && str <= '9' )
    {
      str = str - '0';
    }
    else if ( str >= 'A' && str <= 'F' )
    {
      str = str - 'A' + 10;
    }
    else if ( str >= 'a' && str <= 'f' )
    {
      str = str - 'a' + 10;
    }

    if( upper )
    {
      hex = 0;
      hex = hex | ( s2h[str] << 4 );
      upper = false;
    }
    else
    {
      hex |= s2h[str];
      out[*hexLen] = hex;
      *hexLen = *hexLen + 1;
      upper = true;
    }

    ++pos;
  }	
}

/********************************************************************
Class:	      CDataList
---------------------------------------------------------------------
Description:	

  This is a generic FIFO queue class.

********************************************************************/

class CDataList
{
public:
  /********************************************************************
  Function:	  CDataList
  ---------------------------------------------------------------------
  Description:	

    This is the object constructor.  All object members are initialized
    here.

  ********************************************************************/	
  CDataList()
  {
    this->pHead = NULL;
    this->pTail = NULL;
    InitializeCriticalSection( &this->Mutex );
    length = 0;
  }

  /********************************************************************
  Function:	  ~CDataList
  ---------------------------------------------------------------------
  Description:	

    This is the object destructor.  All memory allocated during the 
    life of the object must be freed here.

  ********************************************************************/
  ~CDataList()
  {
    DATA_NODE *pTmp;
    DATA_NODE *pNext;
    pTmp = pHead;

    while( pTmp != NULL )
    {
      pNext = pTmp->pNext;
      free( pTmp->pData );
      delete pTmp;			
      pTmp = pNext;
    }
  }

  /********************************************************************
  Function:	  RemoveHead
  ---------------------------------------------------------------------
  Description:	

    This function removes an object from the top of the list.

  ********************************************************************/
  void *RemoveHead( void )
  {
    DATA_NODE *pTmp;
    void *pRetVal;

    EnterCriticalSection( &this->Mutex );

    pRetVal = NULL;
    pTmp = pHead;

    if( pTmp != NULL )
    {
      pRetVal = pTmp->pData;
      pHead = pHead->pNext;
      if( pHead == NULL )
      {
        pTail = NULL;
      }			
      delete pTmp;

      length--;
    }
    LeaveCriticalSection( &this->Mutex );

    return pRetVal;
  }

  /********************************************************************
  Function:	  AddHead
  ---------------------------------------------------------------------
  Description:	

    This function adds a data object to the front of the list.

  ********************************************************************/
  void AddHead( void *pData )
  {
    DATA_NODE *pNewNode;

    EnterCriticalSection( &this->Mutex );
    pNewNode = new DATA_NODE;
    pNewNode->pData = pData;
    pNewNode->pNext = NULL;
    pNewNode->pPrev = NULL;

    if( pHead == NULL )
    {
      pHead = pNewNode;
      pTail = pHead;
    }
    else
    {
      pHead->pPrev = pNewNode;
      pNewNode->pNext = pHead;
      pHead = pNewNode;
    }
    length++;

    LeaveCriticalSection( &this->Mutex );
  }

  /********************************************************************
  Function:	  AddTail
  ---------------------------------------------------------------------
  Description:	

    This function adds a data object to the end of the list.

  ********************************************************************/
  void AddTail( void *pData )
  {
    DATA_NODE *pNewNode;

    EnterCriticalSection( &this->Mutex );
    pNewNode = new DATA_NODE;
    pNewNode->pData = pData;
    pNewNode->pNext = NULL;
    pNewNode->pPrev = NULL;

    if( pHead == NULL )
    {
      pHead = pNewNode;
      pTail = pHead;
    }
    else
    {
      pTail->pNext = pNewNode;
      pNewNode->pPrev = pTail;
      pTail = pNewNode;
    }
    length++;

    LeaveCriticalSection( &this->Mutex );
  }

  /********************************************************************
  Function:	  InsertAt
  ---------------------------------------------------------------------
  Description:	

    This function inserts the data at the given position.

  ********************************************************************/
  void InsertAt( unsigned __int32 Position, void *pData )
  {
    DATA_NODE *pNewNode;
    DATA_NODE *pTmp;

    EnterCriticalSection( &this->Mutex );
    pNewNode = new DATA_NODE;
    pNewNode->pData = pData;
    pNewNode->pNext = NULL;
    pNewNode->pPrev = NULL;

    if( ( Position == 0 ) || ( pHead == NULL ) )
    {
      if( pHead != NULL )
      {
        pNewNode->pNext = pHead;
        pHead->pPrev = pNewNode;
        pHead = pNewNode;
      }
      else
      {
        pHead = pNewNode;
        pTail = pHead;
      }
    }
    else
    {
      pTmp = pHead;

      while( ( Position > 0 ) && ( pTmp != NULL ) )
      {
        pTmp = pTmp->pNext;
        --Position;
      }

      // Insert item
      if( pTmp != NULL )
      {
        pNewNode->pNext = pTmp->pNext;
        pTmp->pNext = pNewNode;
        pNewNode->pPrev = pTmp;

        if( pNewNode->pNext != NULL )
        {
          pNewNode->pNext->pPrev = pNewNode;
        }
      }
      else // Add to tail
      {
        pTail->pNext = pNewNode;
        pNewNode->pPrev = pTail;
        pTail = pNewNode;
      }
    }
    length++;

    LeaveCriticalSection( &this->Mutex );
  }


  int getLength(){return length;}

private:
  DATA_NODE *pHead;
  DATA_NODE *pTail;
  CRITICAL_SECTION Mutex;
  int length;
};

#endif
