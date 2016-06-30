
#pragma once

#include "INsCodeGenHelper.h"

class LinNsCodeGenHelper : public INsCodeGenHelper,  public IPduCodeGenHelper, public IFrameCodeGenHelper
{
    ICluster* mCluster;
public:
    LinNsCodeGenHelper();
    virtual ~LinNsCodeGenHelper();


    virtual void SetCluster(ICluster* cluster);
    virtual std::string GetProtocolIncludesHeaderName(ICluster* pCluster);

    virtual IPduCodeGenHelper* GetPduCodeGenHelper();
    virtual IFrameCodeGenHelper* GetFrameCodeGenHelper();


    //pdu cpp
    virtual std::string GetConstructorDef(IPdu*);
    virtual std::string GetBaseClassConstructorDef(IPdu*);
    virtual std::string GetSignalsConstructorDef(IPdu*);
    virtual std::string GetUniqueName(IPdu*);

    //pdu .h
    virtual std::string GetDecl(IPdu*);
    virtual std::string GetBaseClassDecl(IPdu*);
    virtual std::string GetSignalsDecl(IPdu*);


    //Frame .cpp
    virtual std::string GetConstructorDef(IFrame*);
    virtual std::string GetBaseClassConstructorDef(IFrame*);
    virtual std::string GetPdusConstructorDef(IFrame*);
    virtual std::string GetSignalsConstructorDef(IFrame*);
    virtual std::string GetUniqueName(IFrame*);
    //Frame .h
    virtual std::string GetDecl(IFrame*);
    virtual std::string GetBaseClassDecl(IFrame*);
    virtual std::string GetPdusDecl(IFrame*);
    virtual std::string GetSignalsDecl(IFrame*);


    static INsCodeGenHelper* __stdcall Create();
private:
    std::string GetCheckSumType();
    std::string GenerateSignalConstuctor(  int startBit, ISignal* pouSignal, eEndianess ouEndianness, std::string strContainerName);
    std::string GenerateSignalDecl( ISignal* pouSignal, std::string strContainerName);
    void GetSignalType(ISignal* pouSignal, std::string& strSignalType);
};

