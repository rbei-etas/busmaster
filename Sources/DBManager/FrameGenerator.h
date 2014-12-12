#pragma once
#include "Defines.h"
#include "LDFTagDefines.h"
#include "IClusterProps.h"
#include "LDFCluster.h"
#include <sstream>      // std::ostringstream
#include <fstream>

class CFrameGenerator
{
    LDFCluster* m_pLDFCluster;

public:
    CFrameGenerator(void);
    ~CFrameGenerator(void);

    ERRORCODE nGenerateFrames(LDFCluster* ouCluster, std::ostringstream& omFrameStream);

private:
    ERRORCODE nGenerateUnconditionalFrames(std::list<CLINFrame*> lstUncondFrames, std::ostringstream& omStringStream);
    ERRORCODE nGenerateDiagFrames(std::list<CLINFrame*> lstDiagFrames, std::ostringstream& omStringStream);
    ERRORCODE nGenerateEventTriggFrames(std::list<CLINFrame*> lstUncondFrames, std::ostringstream& omStringStream);
    ERRORCODE nGenerateSporadicFrames(std::list<CLINFrame*> lstUncondFrames, std::ostringstream& omStringStream);

    // Frame Generation
    ERRORCODE nGenerateUncondFrameProperties(CLINFrame* objFrame, std::ostringstream& omFrameProps);
    ERRORCODE nGenerateDiagFrameProperties(CLINFrame* objFrame, std::ostringstream& omFrameProps);
    ERRORCODE nGenerateEventTrigFrameProperties(CLINFrame* pFrame, std::ostringstream& omFrameProps);
    ERRORCODE nGenerateSporadicFrameProperties(CLINFrame* pFrame, std::ostringstream& omFrameProps);
};

