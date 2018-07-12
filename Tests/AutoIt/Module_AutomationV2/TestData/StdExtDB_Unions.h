/* This is an ALWAYS automatically generated file based on the database ... */
/*It is recommended to change the database directly. If you want to prevent overwritten header files, make a copy of it and ensure you include the intended version in your node simulation.*/

#pragma once
#pragma pack(pop,8)

#ifndef MESSAGENAME_MsgStdLil
#define MESSAGENAME_MsgStdLil
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    __int64      Sigstd1          : 64 ;

	} __attribute__((__packed__));
}MsgStdLil_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	MsgStdLil_    m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} MsgStdLil;
#endif /* MESSAGENAME_MsgStdLil */


#ifndef MESSAGENAME_MsgStdBig
#define MESSAGENAME_MsgStdBig
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int                  : 32 ;
	    unsigned int                  : 24 ;
	    __int64      Sigstd2          : 64 ;

	} __attribute__((__packed__));
}MsgStdBig_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	MsgStdBig_    m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} MsgStdBig;
#endif /* MESSAGENAME_MsgStdBig */


#ifndef MESSAGENAME_MsgExtLil
#define MESSAGENAME_MsgExtLil
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int SigExt1          :  1 ;
	    unsigned int                  : 32 ;
	    unsigned int                  : 31 ;

	} __attribute__((__packed__));
}MsgExtLil_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	MsgExtLil_    m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} MsgExtLil;
#endif /* MESSAGENAME_MsgExtLil */


#ifndef MESSAGENAME_MsgExtBig
#define MESSAGENAME_MsgExtBig
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int                  : 32 ;
	    unsigned int                  : 24 ;
	    __int64      SigExt2          : 64 ;

	} __attribute__((__packed__));
}MsgExtBig_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	MsgExtBig_    m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} MsgExtBig;
#endif /* MESSAGENAME_MsgExtBig */
