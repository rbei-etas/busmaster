///////////////////////////////////////////////////////////
//  NSCodeGenHelperFactory.h
//  Implementation of the Class NSCodeGenHelperFactory
//  Created on:      09-Oct-2015 14:23:06
//  Original author: vme1kor
///////////////////////////////////////////////////////////

#if !defined(EA_8164F5FE_3301_424e_AC25_9B4DC580F29E__INCLUDED_)
#define EA_8164F5FE_3301_424e_AC25_9B4DC580F29E__INCLUDED_

#include "INsCodeGenHelper.h"

typedef INsCodeGenHelper* (__stdcall* CreateNsCodeGenHelper)(void);

class NSCodeGenHelperFactory
{
    typedef std::map<ETYPE_BUS, CreateNsCodeGenHelper> NsCodeGenFactoryMap;
    NsCodeGenFactoryMap m_FactoryMap;
public:
    NSCodeGenHelperFactory();
    virtual ~NSCodeGenHelperFactory();
    INsCodeGenHelper* m_INsCodeGenHelper;

    INsCodeGenHelper* GetNsCodeGenHelper( ETYPE_BUS bus );
private:
    void CreateMap();
};
#endif // !defined(EA_8164F5FE_3301_424e_AC25_9B4DC580F29E__INCLUDED_)
