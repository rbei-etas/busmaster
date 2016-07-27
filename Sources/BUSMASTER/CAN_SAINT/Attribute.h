#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <tchar.h>
#include "windows.h"
//#include "mem.h"
class CAttribute
{
public:
	CAttribute(void);
	CAttribute(CAttribute* a);
	~CAttribute(void);
private:
	// The string value of the attribute
	_TCHAR* m_szValue;
	// The name of the attribute
	_TCHAR* m_szName;
public:
	// Gets The name of the attribute
	_TCHAR* GetName(void);
	// gets the value of the attribute
	_TCHAR* GetValue(void);
	// Setter for the value of the attribute
	bool SetValue(_TCHAR* szValue);
	// Setter for the name of the attribute
	bool SetName(_TCHAR* szName);
	// Operator for sorting
	bool operator < (CAttribute& a);
	bool operator == (CAttribute& a);
	bool operator == (_TCHAR* a);
	bool operator > (CAttribute& a);
	CAttribute &operator = ( CAttribute& a);
	CAttribute *operator = ( CAttribute* a);

private:
	// falg var for knowing if the value has been previously allocated
	bool m_bValAlloc;
	// falg var for knowing if the name has been previously allocated
	bool m_bNameAlloc;
};
