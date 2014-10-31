#include <sstream>      // std::ostringstream
#include <fstream>
#include "LDFCluster.h"

#pragma once
class SignalGenerator
{
    LDFCluster* m_pLDFCluster;

public:
    SignalGenerator(void);
    ~SignalGenerator(void);

    ERRORCODE nGenerateSignals(LDFCluster* pCluster, std::ostringstream& omStream);
    ERRORCODE nGenerateSignalEncodingTypes(LDFCluster* ouCluster, std::ostringstream& omSigEncoding);

    ERRORCODE nGenerateSignalRepresentation(LDFCluster* ouCluster, std::ostringstream& omSigRepresentation);
    ERRORCODE nGenerateSignalGroups(LDFCluster* ouCluster, std::ostringstream& omSigGroups);

private:

    ERRORCODE nGenerateNormalSignalProperties(CLINSignal* objSignal, std::ostringstream& omSigProps);
    ERRORCODE nGenerateDiagSignalProperties(CLINSignal* objSignal, std::ostringstream& omSigProps);
    ERRORCODE nGenerateNormalSignals(std::list<CLINSignal*> lstNormalSignals, std::ostringstream& omStream);
    ERRORCODE nGenerateDiagnosticsSignals(std::list<CLINSignal*> lstDiagSignals, std::ostringstream& omStream);
    ERRORCODE nGetSignalInitValues(SignalProps ouSignalProps, std::string& omSigInitValues);
    ERRORCODE nGetSubscribers(std::list<IEcu*> ouECUlst, std::string& strRxEcus);

    ERRORCODE nGeneratePhysicalEncodings(std::string strCodingName, CompuMethodProps ouCompuProps, std::ostringstream& omEncodings);
    ERRORCODE nGenerateLogicalEncodings(std::string strCodingName, CompuMethodProps ouCompuProps, std::ostringstream& omEncodings);


};

