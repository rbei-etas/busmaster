///////////////////////////////////////////////////////////
//  CanNsCodeGenHelper.h
//  Implementation of the Class CanNsCodeGenHelper
//  Created on:      09-Oct-2015 14:37:41
//  Original author: vme1kor
///////////////////////////////////////////////////////////

#if !defined(EA_F420036B_6DCE_468b_B546_D72F0D122D4C__INCLUDED_)
#define EA_F420036B_6DCE_468b_B546_D72F0D122D4C__INCLUDED_

#include "INsCodeGenHelper.h"

class CanNsCodeGenHelper : public INsCodeGenHelper, public IPduCodeGenHelper, public IFrameCodeGenHelper
{
    ICluster* mCluster;
public:
    CanNsCodeGenHelper();
    virtual ~CanNsCodeGenHelper();
    virtual IPduCodeGenHelper* GetPduCodeGenHelper();
    virtual IFrameCodeGenHelper* GetFrameCodeGenHelper();

    std::string GetFrameBaseClassInitialiser(IFrame* frame);
    std::string GetFrameBaseClassName();
    std::string GetFrameClassName();
    std::string GetFrameName(IFrame* frame);
    std::string GetFramePdusInitialisation(IFrame* frame, IPdu* pdu);
    std::string GetFrameSignalsInitialiser(IFrame* frame);
    std::string getPduBaseClassInitialiser(IPdu* pdu);
    std::string GetPduBaseClassName();
    std::string GetPduClassName();
    std::string GetPduInitialiser(IPdu* pdu);
    std::string GetPduName(IPdu* pdu);
    std::string GetPduSignalInitialiser(IPdu* pdu, ISignal* signal);
    std::string GetSignalName(ISignal* signal);
    std::string GetProtocolIncludesHeaderName(ICluster* pCluster);
    void SetCluster(ICluster* cluster);
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

    std::string GetCheckSumType();
    std::string GenerateSignalConstuctor(int startBit, ISignal* pouSignal, eEndianess ouEndianness, std::string strContainerName);
    std::string GenerateSignalDecl(ISignal* pouSignal, std::string strContainerName);
    void GetSignalType(ISignal* pouSignal, std::string& strSignalType);
    static  INsCodeGenHelper* __stdcall Create();

};
#endif // !defined(EA_F420036B_6DCE_468b_B546_D72F0D122D4C__INCLUDED_)
