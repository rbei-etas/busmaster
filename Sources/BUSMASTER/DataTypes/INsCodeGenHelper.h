///////////////////////////////////////////////////////////
//  INsCodeGenHelper.h
//  Implementation of the Class INsCodeGenHelper
//  Created on:      09-Oct-2015 14:23:05
//  Original author: vme1kor
///////////////////////////////////////////////////////////

#if !defined(EA_BB1CC86F_8A01_442a_B260_69EC5411A7BD__INCLUDED_)
#define EA_BB1CC86F_8A01_442a_B260_69EC5411A7BD__INCLUDED_

#include "ICluster.h"

class IPduCodeGenHelper
{
public:
    //.cpp
    virtual std::string GetConstructorDef(IPdu*) = 0;
    virtual std::string GetBaseClassConstructorDef(IPdu*) = 0;
    virtual std::string GetSignalsConstructorDef(IPdu*) = 0;
    virtual std::string GetUniqueName(IPdu*) = 0;
    //.h
    virtual std::string GetDecl(IPdu*) = 0;
    virtual std::string GetBaseClassDecl(IPdu*) = 0;
    virtual std::string GetSignalsDecl(IPdu*) = 0;
};
class IFrameCodeGenHelper
{
public:
    //.cpp
    virtual std::string GetConstructorDef(IFrame*) = 0;
    virtual std::string GetBaseClassConstructorDef(IFrame*) = 0;
    virtual std::string GetPdusConstructorDef(IFrame*) = 0;
    virtual std::string GetSignalsConstructorDef(IFrame*) = 0;
    virtual std::string GetUniqueName(IFrame*) = 0;
    //.h
    virtual std::string GetDecl(IFrame*) = 0;
    virtual std::string GetBaseClassDecl(IFrame*) = 0;
    virtual std::string GetPdusDecl(IFrame*) = 0;
    virtual std::string GetSignalsDecl(IFrame*) = 0;
};

class INsCodeGenHelper
{

public:
    INsCodeGenHelper() {};
    virtual ~INsCodeGenHelper() {};

    virtual IPduCodeGenHelper* GetPduCodeGenHelper() = 0;
    virtual IFrameCodeGenHelper* GetFrameCodeGenHelper() = 0;

    virtual std::string GetProtocolIncludesHeaderName(ICluster* pCluster) = 0;
    virtual void SetCluster(ICluster* cluster) =0;

};
#endif // !defined(EA_BB1CC86F_8A01_442a_B260_69EC5411A7BD__INCLUDED_)
