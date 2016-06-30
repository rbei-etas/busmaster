///////////////////////////////////////////////////////////
//  NSCodeGenHelperFactory.cpp
//  Implementation of the Class NSCodeGenHelperFactory
//  Created on:      09-Oct-2015 14:23:07
//  Original author: vme1kor
///////////////////////////////////////////////////////////
#include "DataTypes_stdafx.h"
#include "NSCodeGenHelperFactory.h"
#include "CanNsCodeGenHelper.h"
#include "LinNsCodeGenHelper.h"
#include "J1939NsCodeGenHelper.h"

NSCodeGenHelperFactory::NSCodeGenHelperFactory()
{
    CreateMap();
}



NSCodeGenHelperFactory::~NSCodeGenHelperFactory()
{
    m_FactoryMap.clear();
}





INsCodeGenHelper* NSCodeGenHelperFactory::GetNsCodeGenHelper( ETYPE_BUS bus )
{
    auto itr = m_FactoryMap.find(bus);

    if ( m_FactoryMap.end() != itr )
    {
        return itr->second();
    }

    return nullptr;
}

void NSCodeGenHelperFactory::CreateMap()
{
    m_FactoryMap[ETYPE_BUS::CAN] = &CanNsCodeGenHelper::Create;
    m_FactoryMap[ETYPE_BUS::LIN] = &LinNsCodeGenHelper::Create;
    m_FactoryMap[ETYPE_BUS::J1939] = &J1939NsCodeGenHelper::Create;
}