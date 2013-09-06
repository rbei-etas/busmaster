
#include <windows.h>
#include <conio.h>
#include <stdio.h>

void __cdecl Echo( const char*, ... );
void __cdecl ShowTime( );
void __cdecl ShowLength( );
void __cdecl InitEcho( char* fileName );
void __cdecl CloseEcho( );

char* GetCodeString( short cr );
char* GetFrameType( t_CANframeType frameType );
char* GetBoardTypeString( DWORD type );
char* GetChipState( int state );
char* GetControlerState( int state );
char* GetLineDrvMode( int mode );
char* GetLineDrvDiag( int diag );
char* GetDiagOnCANstate( int state );
void GetEventString( t_CANevent* pEvent, char* string );


#ifdef _ECHO
#define PUBEXT
#else
#define PUBEXT extern
#endif

PUBEXT HANDLE hMutex;
PUBEXT BOOL echoOn;
PUBEXT BOOL echoFile;
PUBEXT HANDLE hEchoFile;
