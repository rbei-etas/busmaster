#ifndef TEST_UNIONS_H_
#define TEST_UNIONS_H_
#pragma pack(push,8)


typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int                  : 32 ;
	    unsigned int                  : 32 ;

	};
}Message_3_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	Message_3_    m_sWhichBit     ;
	unsigned char m_ucChannel	;

} Message_3;


typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int                  : 32 ;
	    unsigned int                  : 32 ;

	};
}Message_2_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	Message_2_    m_sWhichBit     ;
	unsigned char m_ucChannel	;

} Message_2;


typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int                  : 32 ;
	    unsigned int                  : 32 ;

	};
}Message_1_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	Message_1_    m_sWhichBit     ;
	unsigned char m_ucChannel	;

} Message_1;


#pragma pack(pop,8)
#endif