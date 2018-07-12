/* This is an ALWAYS automatically generated file based on the database ... */
/*It is recommended to change the database directly. If you want to prevent overwritten header files, make a copy of it and ensure you include the intended version in your node simulation.*/

#pragma once
#pragma pack(pop,8)

#ifndef MESSAGENAME_Msg1
#define MESSAGENAME_Msg1
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    __int64      Signal1          : 64 ;

	} __attribute__((__packed__));
}Msg1_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Msg1_         m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Msg1;
#endif /* MESSAGENAME_Msg1 */


#ifndef MESSAGENAME_Msg2
#define MESSAGENAME_Msg2
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    __int64      Signal2          : 64 ;

	} __attribute__((__packed__));
}Msg2_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Msg2_         m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Msg2;
#endif /* MESSAGENAME_Msg2 */


#ifndef MESSAGENAME_Msg3
#define MESSAGENAME_Msg3
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    __int64      Signal3          : 64 ;

	} __attribute__((__packed__));
}Msg3_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Msg3_         m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Msg3;
#endif /* MESSAGENAME_Msg3 */


#ifndef MESSAGENAME_Msg4
#define MESSAGENAME_Msg4
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    __int64      Signal4          : 64 ;

	} __attribute__((__packed__));
}Msg4_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Msg4_         m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Msg4;
#endif /* MESSAGENAME_Msg4 */


#ifndef MESSAGENAME_Msg5
#define MESSAGENAME_Msg5
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    __int64      Signal5          : 64 ;

	} __attribute__((__packed__));
}Msg5_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Msg5_         m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Msg5;
#endif /* MESSAGENAME_Msg5 */


#ifndef MESSAGENAME_Msg6
#define MESSAGENAME_Msg6
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    __int64      Signal6          : 64 ;

	} __attribute__((__packed__));
}Msg6_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Msg6_         m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Msg6;
#endif /* MESSAGENAME_Msg6 */
