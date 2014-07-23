/* This is an ALWAYS automatically generated file based on the database ... */
/*It is recommended to change the database directly. If you want to prevent overwritten header files, make a copy of it and ensure you include the intended version in your node simulation.*/

#pragma once
#pragma pack(pop,8)

#ifndef MESSAGENAME_ABC
#define MESSAGENAME_ABC
typedef union 
{
	unsigned char      m_aucData[8];

	struct
	{
	    __int64      Sig              : 64 ;

	} __attribute__((__packed__));
}ABC_;
typedef struct 
{
	STJ1939_MSG_PROPERTIES m_sMsgProperties;
	unsigned int m_unDLC;
	ABC_*         m_sWhichBit     ;

} ABC;
#endif /* MESSAGENAME_ABC */


#ifndef MESSAGENAME_XYZ
#define MESSAGENAME_XYZ
typedef union 
{
	unsigned char      m_aucData[8];

	struct
	{
	    int          CFD              :  8 ;
	    unsigned int                  : 32 ;
	    unsigned int                  : 24 ;

	} __attribute__((__packed__));
}XYZ_;
typedef struct 
{
	STJ1939_MSG_PROPERTIES m_sMsgProperties;
	unsigned int m_unDLC;
	XYZ_*         m_sWhichBit     ;

} XYZ;
#endif /* MESSAGENAME_XYZ */
