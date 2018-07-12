/* This is an ALWAYS automatically generated file based on the database ... */
/*It is recommended to change the database directly. If you want to prevent overwritten header files, make a copy of it and ensure you include the intended version in your node simulation.*/

#pragma once
#pragma pack(pop,8)

#ifndef MESSAGENAME_Msg_Rqst
#define MESSAGENAME_Msg_Rqst
typedef union 
{
	unsigned char      m_aucData[8];

	struct
	{
	    unsigned int TS_signal        :  1 ;
	    unsigned int                  : 32 ;
	    unsigned int                  : 31 ;

	} __attribute__((__packed__));
}Msg_Rqst_;
typedef struct 
{
	STJ1939_MSG_PROPERTIES m_sMsgProperties;
	unsigned int m_unDLC;
	Msg_Rqst_*    m_sWhichBit     ;

} Msg_Rqst;
#endif /* MESSAGENAME_Msg_Rqst */


#ifndef MESSAGENAME_Msg_Data
#define MESSAGENAME_Msg_Data
typedef union 
{
	unsigned char      m_aucData[8];

	struct
	{
	    __int64      XBR_sig          : 64 ;

	} __attribute__((__packed__));
}Msg_Data_;
typedef struct 
{
	STJ1939_MSG_PROPERTIES m_sMsgProperties;
	unsigned int m_unDLC;
	Msg_Data_*    m_sWhichBit     ;

} Msg_Data;
#endif /* MESSAGENAME_Msg_Data */


#ifndef MESSAGENAME_Msg_Broadcast
#define MESSAGENAME_Msg_Broadcast
typedef union 
{
	unsigned char      m_aucData[8];

	struct
	{
	    unsigned int sig1             :  1 ;
	    __int64      sig2             : 63 ;

	} __attribute__((__packed__));
}Msg_Broadcast_;
typedef struct 
{
	STJ1939_MSG_PROPERTIES m_sMsgProperties;
	unsigned int m_unDLC;
	Msg_Broadcast_*  m_sWhichBit     ;

} Msg_Broadcast;
#endif /* MESSAGENAME_Msg_Broadcast */
