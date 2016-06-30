
#define _ECHO
#pragma pack(push,2)
#include "..\CANex\Candefex.h"
#pragma pack(pop)
#include "..\CANex\Echoex.h"

int __cdecl _output ( FILE* , const TCHAR *format, va_list argptr );

SYSTEMTIME lastTime;

void __cdecl Echo( const char* format, ... )
{	
	DWORD dword;
	char buffer[0x100];
	va_list list;
	va_start( list, format );
	memset( buffer, 0x00, sizeof( buffer ));
	if( echoOn )
	{
		FILE str;
        str._flag = _IOWRT|_IOSTRG;
        str._ptr = str._base = buffer;
        str._cnt = sizeof( buffer );
        _output( &str,format,list);
		WaitForSingleObject( hMutex, INFINITE );
		puts( buffer );
		ReleaseMutex( hMutex );

		if( echoFile )
		{
			strcat( buffer, "\r\n" );
			WriteFile( hEchoFile, buffer, strlen( buffer ), &dword, NULL );
		}
	}
}

void __cdecl ShowTime( )
{
	SYSTEMTIME time;
	GetLocalTime( &time );

	Echo( "Time : %02d:%02d.%02d      Date %02d/%02d/%04d",
		time.wHour,
		time.wMinute,
		time.wSecond,
		time.wDay,
		time.wMonth,
		time.wYear );

	lastTime = time;
}

void __cdecl ShowLength( )
{
	LONG length;
	SYSTEMTIME time;
	GetLocalTime( &time );

	length = 0;
	length += (time.wHour - lastTime.wHour) * 3600;
	length += (time.wMinute - lastTime.wMinute) * 60;
	length += (time.wSecond - lastTime.wSecond);

	if( length < 300 ) 
		Echo("Duration : %d sec",length);
	else 
		Echo("Duration : %d min",length / 60 );
}


void __cdecl CloseEcho( )
{
	if (hMutex) CloseHandle( hMutex );
	if( echoFile ) CloseHandle( hEchoFile );
}


void __cdecl InitEcho( char* fileName )
{
	hMutex = CreateMutex( NULL, FALSE, NULL );

	echoOn = TRUE;
	if( fileName )
	{
		hEchoFile = CreateFile(
			fileName,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_ARCHIVE,
			NULL );

		if( hEchoFile != INVALID_HANDLE_VALUE )
		{
			echoFile = TRUE;
		}
		else
		{
			echoFile = FALSE;
		}
	}
}


char* GetFrameType( t_CANframeType frameType )
{
	switch( frameType )
	{
	case _CAN_TX_DATA : return "TXD";
	case _CAN_TX_SEG_DATA : return "TXS";
	case _CAN_RX_DATA : return "RXD";
	case _CAN_TX_FC_DATA : return "TXF" ;
	case _CAN_RX_FC_DATA : return "RXF" ;
	case _CAN_RX_SEG_DATA : return "RXS";
	case _CAN_RX_FF_DATA : return "RX1";
	case _CAN_TX_RX_REMOTE : return "TRR";
	case _CAN_TX_REMOTE : return "TXR"; 
	case _CAN_RX_DATA_REMOTE : return "RXR";
	case _CAN_TX_AUTO_REMOTE : return "TAR";
	case _CAN_RX_REMOTE : return "RRM";
	case _CAN_LOST_MSG : return "LST";
	default : return "??? ";				
	}
}


// GetCodeString
// -------------
// Retourne le nom explicite du code erreur passé en paramètre
//
// Returns the explicit name of the given error code
//
char* GetCodeString( short cr )
{
	switch( cr )
	{
	case _OK : return "_OK";
	case _MEM_ERR : return "_MEM_ERR";
	case _SEQ_ERR : return "_SEQ_ERR";
	case _PARAM_ERR : return "_PARAM_ERR";
	case _CHIP_ERR : return "_CHIP_ERR";
	case _BOARD_ERR : return "_BOARD_ERR";
	case _ID_OVERFLOW : return "_ID_OVERFLOW";
	case _UNKNOWN_ID : return "_UNKNOWN_ID";
	case _UNKNOWN_PERIOD :  return "_UNKNOWN_PERIOD";
	case _PERIODIC_OVERFLOW :  return "_PERIODIC_OVERFLOW";
	case _EMPTY_BUF : return "_EMPTY_BUF";
	case _EMPTY_FIFO : return "_EMPTY_FIFO";
	case _FRAME_TYPE_ERR : return "_FRAME_TYPE_ERR";
	case _INTERFACE_ERR : return "_INTERFACE_ERR";
	case _BUF_OCCUPIED : return "_BUF_OCCUPIED";
	case _INVALID_OP : return "_INVALID_OP";
	case _DRV_PARAM_ERR : return "_DRV_PARAM_ERR";
	case _OPENING_DRV_ERR : return "_OPENING_DRV_ERR";
	case _SLEEP_MODE : return "_SLEEP_MODE";
	case _USB_ERR : return "_USB_ERR";
	case _RS232_ERR : return "_RS232_ERR";
	case _BOARD_TIMEOUT : return "_BOARD_TIMEOUT";
	default : return "_???";
	}
}


// GetBoardTypeString
// ------------------
// Retourne le nom explicite du type de la carte passé en paramètre
//
// Returns the explicit board type name of the given type
//
char* GetBoardTypeString( DWORD type )
{
	switch( type )
	{
	case _CANPC : return "CANPC";
	case _CANPC_OPTO : return "CANPC/OP";
	case _CANPCA : return "CANPCa";
	case _CANPCA_LS : return "CANPCa/LS";
	case _CANPCA_OPTO : return "CANPCa/OPTO";
	case _CANPCMCIA : return "CANPCMCIA";
	case _CANPCMCIA_LS : return "CANPCMCIA/LS";
	case _CANPCMCIA_OPTO : return "CANPCMCIA/OPTO";
	case _CANPCI : return "CANPCI";
	case _CANPCI_LS : return "CANPCI /LS";
	case _CANPCI_OPTO : return "CANPCI /OPTO";
	case _CANUSB : return "CAN-USB";
	case _CAN_OBDUSB : return "MUXy";
	case _CANEX_OBDUSB : return "MUXy (ISO 15765-2)";
	case _MUXYBOX : return "MUXy box";
	case _MUXYBOX_KWPTEST : return "MUXy box (KWP_Test)";
	case _MUXYLIGHT	: return "MUXy light";
	default : return "BOARD ???";
	}
}

char* GetChipState( int state )
{
	switch( state )
	{
	case _CAN_ACTIVE_ERR : return "NORM";
	case _CAN_PASSIVE_ERR : return "PASV";
	case _CAN_BUS_OFF : return "BUS OFF";
	case _CAN_UNKNOWN : return "UNKN";
	default : return "????";
	}
}

char* GetControlerState( int state )
{
	switch( state )
	{
	case _CAN_WAKE_UP : return "WAKE-UP";
	case _CAN_POWER_DOWN : return "POWER DOWN";
	case _CAN_SLEEP : return "SLEEP";
	case _CAN_RESET : return "RESET";
	default : return "????";
	}
}

char* GetLineDrvDiag( int diag )
{
	switch( diag )
	{
	case _CAN_NOMINAL : return "NOMINAL";
	case _CAN_DEGRADED : return "DEGRADED";
	case _CAN_MAJOR_ERROR : return "MAJOR_ERROR";
	case _CAN_UNKNOWN : return "UNKNOWN";
	default : return "????";
	}
}

char* GetLineDrvMode( int mode )
{
	switch( mode )
	{
	case _CAN_NORMAL : return "NORMAL";
	case _CAN_ERROR : return "ERR";
	case _CAN_LD_SLEEP : return "SLEEP";
	case _CAN_NETWORK_WAKEUP : return "NET WU";
	default : return "????";
	}
}

char* GetDiagOnCANstate( int state )
{
	switch( state )
	{
	case _DC_OK			:			return "NO_ERROR";
	case _DC_TIMEOUT_Ar	:			return "TIMEOUT_AR";
	case _DC_TIMEOUT_As	:			return "TIMEOUT_AS";
	case _DC_TIMEOUT_Bs	:			return "TIMEOUT_BS";
	case _DC_TIMEOUT_N_Cr	:		return "TIMEOUT_CR";
	case _DC_WRONG_SN		:		return "INVALID_SN";
	case _DC_UNEXP_PDU		:		return "UNEX_PDU";
	case _DC_WFT_OVRN		:		return "WFT_OVRN";
	case _DC_FIFO_OVRN		:		return "FIFO_OVRN";
	case _DC_DATA_LAYER_ERROR	:	return "DL_ERR";
	case _DC_USER_RESET			:	return "USER_RESET";
	case _DC_UNEXPECTED_ERROR	:	return "UNEX_ERR";
	default : return "????";
	}
}

// GetEventString
// --------------
// Cette fonction retourne une ligne de texte formaté décrivant les 
// paramètres principaux de l'event pointé par "pEvent". "string" doit 
// pointer sur un buffer suffisamment grand pour contenir le texte.
//
// This function returns a formated char string describing the main
// data in the event structure pointed by "pEvent". "string" must point
// to a buffer large enough to contain the text.
//
void GetEventString( t_CANevent* pEvent, char* string )
{
	int i;
	char tmp[0x100];
	static DWORD time;
	
	*string = (char)0;
	
	// Affiche la datation. La résolution est 100µs. L'affichage est en millisecondes.
	// Display event time. The resolution is 100µs. The display is in milliseconds.
	sprintf( string, " %.1f ", (pEvent->timeStamp - time ) / 10.0);
	time = pEvent->timeStamp;

	sprintf( string, " %s ", GetChipState( pEvent->CANerr ));

	// Affiche le type de l'évènement
	// Display the event type
	strcat( string, GetFrameType( pEvent->eventType ));

	if( pEvent->eventType == _CAN_RX_SEG_DATA	||
		pEvent->eventType == _CAN_TX_SEG_DATA	||
		pEvent->eventType == _CAN_RX_FF_DATA	|| 
		pEvent->eventType == _CAN_RX_FC_DATA	||
		pEvent->eventType == _CAN_TX_FC_DATA	)
	{
		sprintf( tmp, " {%s} ", GetDiagOnCANstate( pEvent->DCerr ) );
		strcat( string, tmp );
	}
	
	// Affiche la valeur de l'identificateur et la taille de la trame
	// Display the event value and the frame data size
	sprintf( tmp, " %03xh [% 2d] ", pEvent->ident, pEvent->dlc );
	strcat( string, tmp );
	if( pEvent->eventType == _CAN_RX_DATA		 || 
		pEvent->eventType == _CAN_RX_DATA_REMOTE || 
		pEvent->eventType == _CAN_RX_SEG_DATA	 ||
		pEvent->eventType == _CAN_RX_FC_DATA	 )
	{
	// Affiche les octets de donnée
	// Display the data bytes
		for( i=0; i<pEvent->dlc && i<=_CAN_MAX_DATA_EX; i++ )
		{
			if(i < 0x1F)
			{
				sprintf( tmp, "%02x ", pEvent->data[i] );
				strcat( string, tmp );
			}
			else
			{
				strcat( string, "...." );
				break;
			}
			
		}
	}

}
