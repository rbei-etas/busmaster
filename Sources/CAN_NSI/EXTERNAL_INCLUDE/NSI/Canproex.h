/*
-----------------------------------------------------------
 CANPROEX.H  Copyright (c) 2002 NSI (Annecy France)
-----------------------------------------------------------
Author :   CRO
Product :  CANPC2000EX Interface
File    :  DLL functions declarations
Target  :  Windows 32 DLL
History :
  10/06/2002 : Creation (CRO)
  19/02/2003 : First Release
Comments : 
-----------------------------------------------------------

This file contains the definition of :
  - Functions of the CAN interface exported by NSICANEX.DLL

  IMPORTANT WARNING : Make the symbol _CANAPI is properly
  defined in the CANPCEX.H file for the compiler currently used.
  It specifies that these functions are to be imported from 
  a DLL and are using C Calling convention. 
  If not used, THE INTERFACE FUNCTIONS WILL NOT WORK PROPERLY.


Ce fichier contient les definitions :
  - Des fonction de l'interface CAN exportées par NSICANEX.DLL

  MISE EN GARDE IMPORTANTE : Il faut s'assurer que le symbole
  _CANAPI est correctement défini dans le fichier CANPCEX.H pour
  le compilateur utilisé. Ce symbole spécifie que les fonctions
  doivent être importées d'une DLL et qu'elles utilisent la
  convention d'appel C.
  
  Si cette directive n'est pas appliquée, LES FONCTIONS DE 
  L'INTERFACE LOGICIELLE NE FONCTIONNERONT PAS CORRECTEMENT.
*/

#ifdef __cplusplus
	extern "C" {
#endif

short _CANAPI Ic_ActiveId( HANDLE hdrv,unsigned long ident, unsigned short dlc);
short _CANAPI Ic_ChangeId( HANDLE hdrv,unsigned long oldIdent,unsigned long newIdent, unsigned long newFlowControlIdent);
short _CANAPI Ic_ConfigBus( HANDLE hdrv,short compbypass,short polarite,short disconnectTx1,short disconnectRx1,short disconnectRx0);
short _CANAPI Ic_EnumCards( unsigned long* cardcnt,t_CardData* carddata, unsigned long carddatasz);
short _CANAPI Ic_ExitDrv( HANDLE hdrv);
short _CANAPI Ic_GetBoardType( HANDLE hdrv,unsigned long* boardType,short* DLLversion,short *DRVversion);
short _CANAPI Ic_GetAPIinfo( HANDLE hdrv,short* DLLversion,short *DRVversion, unsigned long* reserved);
short _CANAPI Ic_GetBuf( HANDLE hdrv, unsigned long ident, t_CANevent* msg);
short _CANAPI Ic_GetChipState( HANDLE hDrv, t_CANerr* canerr , t_CANstat* etateveil);
short _CANAPI Ic_GetCount( HANDLE hdrv, t_CANcounter* cptu);
short _CANAPI Ic_GetEvent( HANDLE hCanal,t_CANevent* pEvent );
short _CANAPI Ic_GetFifoState( HANDLE hdrv, short* eventCount);
unsigned long _CANAPI Ic_GetCardConf( HANDLE hCanal );
short _CANAPI Ic_InitChip( HANDLE hdrv,t_CANbusParams busParams, t_CANaddressing adressing, t_CANpadding);
short _CANAPI Ic_InitFlowControl( HANDLE hdrv, unsigned long ident, t_CANflowParams flowParams);
short _CANAPI Ic_InitDrv(short cno, HANDLE* hdrv);
short _CANAPI Ic_InitId( HANDLE hdrv,t_CANobj* msg);
short _CANAPI Ic_InitInterface( HANDLE hdrv,t_Interface Interface );
short _CANAPI Ic_DeactivateId( HANDLE hdrv,unsigned long ident);
short _CANAPI Ic_DeleteId( HANDLE hCanal,unsigned long ident);
short _CANAPI Ic_ResetChip( HANDLE hdrv);
short _CANAPI Ic_SetRxMask( HANDLE hdrv,unsigned long code, unsigned long mask, t_CANidentType identType);
short _CANAPI Ic_StartChip( HANDLE hdrv);
short _CANAPI Ic_StopChip( HANDLE hdrv);
short _CANAPI Ic_TxMsg( HANDLE hdrv,unsigned long ident, unsigned short dlc, unsigned char *data);
short _CANAPI Ic_WriteData( HANDLE hdrv,unsigned long ident, unsigned short dlc, unsigned char *data);
void  _CANAPI Ic_WriteChip( HANDLE hdrv, unsigned char ad_reg, unsigned char reg);
short _CANAPI Ic_ResetBoard( HANDLE hdrv );
short _CANAPI Ic_ConfigEvent( HANDLE hdrv, HANDLE hEvent, unsigned long ident );
unsigned char _CANAPI Ic_ReadChip( HANDLE hdrv, unsigned char ad_reg);
short _CANAPI Ic_InitPeriod( HANDLE	hdrv, unsigned long	ident, unsigned short period, BOOL autoStart, long	repeat );
short _CANAPI Ic_StartPeriod( HANDLE hdrv, unsigned long ident );
short _CANAPI Ic_StopPeriod( HANDLE hdrv, unsigned long ident );
short _CANAPI Ic_KillPeriod( HANDLE hdrv, unsigned long ident );
short _CANAPI Ic_InitLineDrv( HANDLE hdrv, short mode, HANDLE hEvent, BOOL autoWakeup );
short _CANAPI Ic_GetMode( HANDLE hdrv, unsigned long* mode );
short _CANAPI Ic_SetMode( HANDLE hdrv, unsigned long mode );
short _CANAPI Ic_GetDiag( HANDLE hdrv, t_CANdiag* CANdiag );
short _CANAPI Ic_GetDeviceInfo( HANDLE hdrv, t_CANdeviceInfo* deviceInfo );
short _CANAPI Ic_GetChipInfo( HANDLE hdrv, t_CANchipInfo* chipInfo );
short _CANAPI Ic_WritePattern( HANDLE hdrv,unsigned long ident, unsigned short size, unsigned char *data, unsigned short dlc);


#ifdef __cplusplus
}
#endif
