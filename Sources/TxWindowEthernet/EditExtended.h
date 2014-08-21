#pragma once
#include "TxWindowEthernet_stdafx.h"

class CEditExtended: public CEdit
{
public:
	CEditExtended(void);
	~CEditExtended(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	 virtual BOOL PreTranslateMessage(MSG* pMsg);
};

