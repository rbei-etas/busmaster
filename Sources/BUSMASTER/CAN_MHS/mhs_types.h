#ifndef __MHS_TYPES_H__
#define __MHS_TYPES_H__

#include "DIL_Interface/IChangeRegisters.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MHS_STR_SIZE 255


	class TMhsCanCfg :public IChangeRegisters
    {
	public:
		char m_CanSnrStr[MHS_STR_SIZE];
		unsigned int m_CanSpeed;
		unsigned int m_CanBtrValue;
        BOOL m_bBitRateSelected;
	public:
		TMhsCanCfg(char CanSnrStr[],unsigned int CanSpeed, unsigned int CanBtrValue, BOOL bBitRateSelected);
		~TMhsCanCfg();
		int InvokeAdavancedSettings(PSCONTROLLER_DETAILS pControllerDetails, UINT nCount, UINT );
		DOUBLE vValidateBaudRate(DOUBLE ,int,UINT );
    };


#ifdef __cplusplus
}
#endif

#endif
