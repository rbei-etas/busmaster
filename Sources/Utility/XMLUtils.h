#pragma once

/* C++ includes */
#include <list>
#include <map>
#include <string>

/* libxml includes */
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <include/BaseDefs.h>
#include <DataTypes/MsgSignal_Datatypes.h>
#include <include/XMLDefines.h>

struct columnInfo
{
    std::string strId;
    int nOrder;
    int nWidth;
    bool isVisble;
};
typedef std::map<std::string, columnInfo> ColumnInfoMap;
typedef std::list<std::string> stringList;

enum eLineType
{
    Solid = 0,
    Dash = 1,
    Dot = 2,
    DashDot = 3,
    DashDotDot = 4,
    Null = 5,
    XYStep = 6,
    YXStep = 7,
    Bars = 8,
    Stick = 9
};

class xmlUtils
{
public:
    static xmlXPathObjectPtr pGetNodes (xmlDocPtr pDoc, xmlChar* pchXpath)
    {
        xmlXPathContextPtr pDocContext;
        xmlXPathObjectPtr pXpathNodePtr;
        //Get Context
        pDocContext = xmlXPathNewContext(pDoc);
        if ( nullptr == pDocContext )
        {
            pXpathNodePtr = nullptr;
        }
        else
        {
            //Evaluate Xml xPath
            pXpathNodePtr = xmlXPathEvalExpression(pchXpath, pDocContext);

            if(pXpathNodePtr != nullptr)
            {

                //Close The DOC Context
                xmlXPathFreeContext(pDocContext);
                if(xmlXPathNodeSetIsEmpty(pXpathNodePtr->nodesetval))
                {
                    xmlXPathFreeObject(pXpathNodePtr);
                    pXpathNodePtr = nullptr;
                }
            }
        }
        return pXpathNodePtr;
    }

    static int nGetNodeCount(xmlNodePtr pNode)
    {
        int nCnt =0;
        xmlNodePtr pNewNode = pNode->children;
        while(pNewNode)
        {
            nCnt++;
            pNewNode = pNewNode->next;
        }
        return nCnt;
    }

    static xmlXPathObjectPtr pGetChildNodes(xmlNodePtr pNode, xmlChar* pchRelXpath)
    {
        xmlXPathContextPtr pDocContext;
        xmlXPathObjectPtr pXpathNodePtr;
        //Get Context
        pDocContext = xmlXPathNewContext(pNode->doc);
        if ( nullptr == pDocContext )
        {
            pXpathNodePtr = nullptr;
        }
        else
        {
            //Evaluate Xml xPath
            pDocContext->node = pNode;
            pXpathNodePtr = xmlXPathEvalExpression(pchRelXpath, pDocContext);

            //Close The DOC Context
            xmlXPathFreeContext(pDocContext);
            if(xmlXPathNodeSetIsEmpty(pXpathNodePtr->nodesetval))
            {
                xmlXPathFreeObject(pXpathNodePtr);
                pXpathNodePtr = nullptr;
            }
        }
        return pXpathNodePtr;
    }

    static bool GetDataFrmNode(xmlNodePtr pNodePtr, char* cstrNodeName, std::string& strData)
    {
        xmlChar* xmlNodeName = (xmlChar* )cstrNodeName;

        //char*  cData = nullptr;
        strData.clear();
        if (!xmlStrcmp(pNodePtr->name, xmlNodeName))
        {
            xmlChar* xmlData = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode,1);
            if(nullptr != xmlData)
            {
                strData = (char*)xmlData;
                xmlFree(xmlData);
            }
            return true;
        }
        return false;
    }

    static int ParseWindowsPlacement(xmlNodePtr pNodePtr, WINDOWPLACEMENT& wndPlacement)
    {
        int nRetVal = S_OK;
        char values[6] = {0};
        pNodePtr = pNodePtr->xmlChildrenNode;
        wndPlacement.length = sizeof(WINDOWPLACEMENT);
        while (pNodePtr != nullptr)
        {
            //Visibility
            if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Visibility")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    wndPlacement.showCmd = nGetWindowVisibility((char*)key);
                    xmlFree(key);
                    values[0] = 1;
                }
            }
            //WindowPlacement
            if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"WindowPlacement")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    wndPlacement.flags = unGetWindowPlacement((char*)key);
                    xmlFree(key);
                    values[1] = 1;
                }
            }
            //Top
            if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Top")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    wndPlacement.rcNormalPosition.top = atoi((char*)key);
                    xmlFree(key);
                    values[2] = 1;
                }
            }
            //Left
            if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Left")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    wndPlacement.rcNormalPosition.left = atoi((char*)key);
                    xmlFree(key);
                    values[3] = 1;
                }
            }
            //Bottom
            if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Bottom")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    wndPlacement.rcNormalPosition.bottom = atoi((char*)key);
                    xmlFree(key);
                    values[4] = 1;
                }
            }
            //Right
            if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Right")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    wndPlacement.rcNormalPosition.right = atoi((char*)key);
                    xmlFree(key);
                    values[5] = 1;
                }
            }

            pNodePtr = pNodePtr->next;
        }
        for(int i = 0; i < 6; i++)
        {
            if(values[i] != 1)
            {
                nRetVal = S_FALSE;
                break;
            }
        }
        return nRetVal;
    }

    static UINT unGetWindowPlacement(CString strWndPlacement)
    {
        if(strWndPlacement == "RESTORETOMAXIMIZED")
        {
            return WPF_RESTORETOMAXIMIZED;
        }
        else if(strWndPlacement == "SETMINPOSITION")
        {
            return WPF_SETMINPOSITION;
        }
        else
        {
            return WPF_RESTORETOMAXIMIZED;
        }
    }

    static CString nGetLineTypeForGraph(INT nLineType)
    {
        switch(nLineType)
        {
            case Solid:
                return defLINETYPE_SOLID;
                break;
            case Dash:
                return defLINETYPE_DASH;
                break;
            case Dot:
                return defLINETYPE_DOT;
                break;
            case DashDot:
                return defLINETYPE_DASHDOT;
                break;
            case DashDotDot:
                return defLINETYPE_DASHDOTDOT;
                break;
            case Null:
                return defLINETYPE_NULL;
                break;
            case XYStep:
                return defLINETYPE_XYSTEP;
                break;
            case YXStep:
                return defLINETYPE_YXSTEP;
                break;
            case Bars:
                return defLINETYPE_BARS;
                break;
            case Stick:
                return defLINETYPE_STICK;
                break;
        }

        return defLINETYPE_SOLID;
    }

    static INT nSetLineTypeForGraph(CString strLineType)
    {
        if(strLineType == defLINETYPE_SOLID)
        {
            return Solid;
        }
        else if(strLineType == defLINETYPE_DASH )
        {
            return Dash;
        }
        else if(strLineType == defLINETYPE_DOT)
        {
            return Dot;
        }
        else if(strLineType == defLINETYPE_DASHDOT )
        {
            return DashDot;
        }
        else if(strLineType == defLINETYPE_DASHDOTDOT )
        {
            return DashDotDot;
        }
        else if(strLineType == defLINETYPE_NULL )
        {
            return Null;
        }
        else if(strLineType == defLINETYPE_XYSTEP )
        {
            return XYStep;
        }
        else if(strLineType == defLINETYPE_YXSTEP )
        {
            return YXStep;
        }
        else if(strLineType == defLINETYPE_BARS)
        {
            return Bars;
        }
        else if(strLineType == defLINETYPE_STICK )
        {
            return Stick;
        }

        return Solid;
    }

    static std::string nGetWindowVisibilityInString(int nShowCmd)
    {
        std::string strShowCmd = "";
        if( nShowCmd == -1)
        {
            return strShowCmd;
        }

        char* strTemp = "";

        switch(nShowCmd)
        {
            case SW_HIDE:
                strTemp = "HIDE";
                break;
            case SW_MAXIMIZE:
                strTemp = "MAXIMIZE";
                break;
            case SW_MINIMIZE:
                strTemp = "MINIMIZE";
                break;
            case SW_RESTORE:
                strTemp = "RESTORE";
                break;
            case SW_SHOW:
                strTemp = "SHOW";
                break;
                /*  case SW_SHOWMAXIMIZED:
                      strTemp = "SHOWMAXIMIZED";
                     break;*/
            case SW_SHOWMINIMIZED:
                strTemp = "SHOWMINIMIZED";
                break;
            case SW_SHOWMINNOACTIVE:
                strTemp = "SHOWMINNOACTIVE";
                break;
            case SW_SHOWNA:
                strTemp = "SHOWNA";
                break;
            case SW_SHOWNOACTIVATE:
                strTemp = "SHOWNOACTIVATE";
                break;
            case SW_SHOWNORMAL:
                strTemp = "SHOWNORMAL";
                break;
        }

        return strTemp;
    }

    static CString nSetWindowVisibility(INT nShowCmd)
    {
        CString strTemp = "";

        switch(nShowCmd)
        {
            case SW_HIDE:
                strTemp = "HIDE";
                break;
            case SW_MAXIMIZE:
                strTemp = "MAXIMIZE";
                break;
            case SW_MINIMIZE:
                strTemp = "MINIMIZE";
                break;
            case SW_RESTORE:
                strTemp = "RESTORE";
                break;
            case SW_SHOW:
                strTemp = "SHOW";
                break;
                /*  case SW_SHOWMAXIMIZED:
                      strTemp = "SHOWMAXIMIZED";
                       break;*/
            case SW_SHOWMINIMIZED:
                strTemp = "SHOWMINIMIZED";
                break;
            case SW_SHOWMINNOACTIVE:
                strTemp = "SHOWMINNOACTIVE";
                break;
            case SW_SHOWNA:
                strTemp = "SHOWNA";
                break;
            case SW_SHOWNOACTIVATE:
                strTemp = "SHOWNOACTIVATE";
                break;
            case SW_SHOWNORMAL:
                strTemp = "SHOWNORMAL";
                break;
        }

        return strTemp;
    }

    static int CreateXMLNodeFrmWindowsPlacement(xmlNodePtr pNodePtr, WINDOWPLACEMENT& wndPlacement)
    {
        const char* omcVarChar ;
        std::string strVar;

        //visibility
        strVar = nGetWindowVisibilityInString(wndPlacement.showCmd);
        omcVarChar = strVar.c_str();
        xmlNodePtr pVisibility = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_VISIBILITY,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pVisibility);

        //WindowPlacement
        CString csWindowPlacement;
        //if(wndPlacement.flags == WPF_ASYNCWINDOWPLACEMENT)
        //csWindowPlacement = "ASYNCWINDOWPLACEMENT";
        if(wndPlacement.flags == WPF_SETMINPOSITION)
        {
            csWindowPlacement = "SETMINPOSITION";
        }
        else
        {
            csWindowPlacement = "RESTORETOMAXIMIZED";
        }

        omcVarChar = csWindowPlacement;
        xmlNodePtr pWindowPlacement = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_MWND_PLACEMENT,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pWindowPlacement);

        //Top
        CString csTop;
        csTop.Format("%d",  wndPlacement.rcNormalPosition.top);
        omcVarChar = csTop;
        xmlNodePtr pTop= xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_MWND_TOP,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pTop);

        //Left
        CString csLeft;
        csLeft.Format("%d",  wndPlacement.rcNormalPosition.left);
        omcVarChar = csLeft;
        xmlNodePtr pLeft = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_MWND_LEFT,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pLeft);

        //Bottom
        CString csBottom;
        csBottom.Format("%d",  wndPlacement.rcNormalPosition.bottom);
        omcVarChar = csBottom;
        xmlNodePtr pBottom= xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_BOTTOM,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pBottom);

        //Right
        CString csRight;
        csRight.Format("%d",  wndPlacement.rcNormalPosition.right);
        omcVarChar = csRight;
        xmlNodePtr pRight = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_RIGHT,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pRight);
        return true;
    }

    static int nGetWindowVisibility(char* pchVisibility)
    {
        int nShowCmd = SW_SHOW;
        if( pchVisibility == nullptr)
        {
            return nShowCmd;
        }

        std::string strTemp = pchVisibility;

        if(strTemp == "HIDE")
        {
            nShowCmd = SW_HIDE;
        }
        if(strTemp == "MAXIMIZE")
        {
            nShowCmd = SW_MAXIMIZE;
        }
        if(strTemp == "MINIMIZE")
        {
            nShowCmd = SW_MINIMIZE;
        }
        if(strTemp == "RESTORE")
        {
            nShowCmd = SW_RESTORE;
        }
        if(strTemp == "SHOW")
        {
            nShowCmd = SW_SHOW;
        }
        if(strTemp == "SHOWMAXIMIZED")
        {
            nShowCmd = SW_SHOWMAXIMIZED;
        }
        if(strTemp == "SHOWMINIMIZED")
        {
            nShowCmd = SW_SHOWMINIMIZED;
        }
        if(strTemp == "SHOWMINNOACTIVE")
        {
            nShowCmd = SW_SHOWMINNOACTIVE;
        }
        if(strTemp == "SHOWNA")
        {
            nShowCmd = SW_SHOWNA;
        }
        if(strTemp == "SHOWNOACTIVATE")
        {
            nShowCmd = SW_SHOWNOACTIVATE;
        }
        if(strTemp == "SHOWNORMAL")
        {
            nShowCmd = SW_SHOWNORMAL;
        }
        return nShowCmd;
    }

    static bool bGetBooleanValue(char* pchBoolValue)
    {
        bool bRetValue = true;
        if ( nullptr != pchBoolValue )
        {
            std::string strTemp = pchBoolValue;

            if(strTemp == "FALSE" || strTemp == "0")
            {
                bRetValue = false;
            }
        }
        return bRetValue;
    }

    static eDirection bGetDirection(char* pchBoolValue)
    {
        eDirection bRetValue = DIR_ALL;
        if ( nullptr != pchBoolValue )
        {
            std::string strTemp = pchBoolValue;
            if(strTemp == "TX")
            {
                bRetValue = DIR_TX;
            }
            if(strTemp == "RX")
            {
                bRetValue = DIR_RX;
            }
        }
        return bRetValue;
    }

    static eTimerMode eGetTimerMode(std::string strName)
    {
        eTimerMode eMode = TIME_MODE_SYSTEM;
        if(strName == "RELATIVE")
        {
            eMode = TIME_MODE_RELATIVE;
        }
        else if(strName == "ABSOLUTE")
        {
            eMode = TIME_MODE_ABSOLUTE;
        }
        return eMode;
    }

    static eFormat eGetNumericMode(std::string strName)
    {
        eFormat eMode = DEC;
        if(strName == "HEX")
        {
            eMode = HEXADECIMAL;
        }
        return eMode;
    }

    static eWAVEFORMTYPE eGetWaveType(std::string strName)
    {
        eWAVEFORMTYPE eWaveType = eWave_NONE;
        if(strName == "SINE")
        {
            eWaveType = eWave_SINE;
        }
        else if(strName == "TRIANGLE")
        {
            eWaveType = eWave_TRIANGLE;
        }
        else if(strName == "COS")
        {
            eWaveType = eWave_COS;
        }
        else if(strName == "SAWTOOTH")
        {
            eWaveType = eWave_SAWTOOTH;
        }
        return eWaveType;
    }

    static int parseColumnNode(xmlNodePtr pNode, columnInfo& Info)
    {
        int nRetVal = S_OK;
        if(pNode == nullptr)
        {
            nRetVal = S_FALSE;
        }
        else
        {
            while(pNode != nullptr)
            {
                if ((!xmlStrcmp(pNode->name, (const xmlChar*)"ID")))
                {
                    xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                    if(nullptr != key)
                    {
                        Info.strId = (char*)key;
                        xmlFree(key);
                    }
                }
                else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Order")))
                {
                    xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                    if(nullptr != key)
                    {
                        Info.nOrder = atoi((char*)key);
                        xmlFree(key);
                    }
                }
                else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Width")))
                {
                    xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                    if(nullptr != key)
                    {
                        Info.nWidth = atoi((char*)key);
                        xmlFree(key);
                    }
                }
                else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"IsVisible")))
                {
                    xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                    if(nullptr != key)
                    {
                        Info.isVisble = bGetBooleanValue((char*)key);
                        xmlFree(key);
                    }
                }
                pNode = pNode->next;
            }
        }
        return nRetVal;
    }
    static int parseColumnInfoNode(xmlNodePtr pNode, stringList& columnList, columnInfo& Info)
    {
        int nRetVal = S_OK;
        if (columnList.size() <= 0 || pNode == NULL )
        {
            nRetVal = S_FALSE;
        }
    }
    static int xmlUtils::ParseSplitterWindow(xmlNodePtr pNode, int& nCxCur, int& nCxMax)
    {
        int nRetVal = S_OK;
        if(pNode == nullptr)
        {
            nRetVal = S_FALSE;
        }
        else
        {
            xmlXPathObjectPtr pObject = pGetChildNodes(pNode, (xmlChar*)"CxIdeal");
            if(pObject != nullptr)
            {
                xmlNodePtr pTemp = pObject->nodesetval->nodeTab[0];
                if(pTemp != nullptr)
                {
                    char* pchKey = (char*)xmlNodeListGetString(pTemp->doc, pTemp->xmlChildrenNode, 1);
                    nCxCur = 0;
                    if(pchKey != nullptr)
                    {
                        nCxCur = atoi(pchKey);
                        xmlFree(pchKey);
                    }
                }
                else
                {
                    nRetVal = S_FALSE;
                }
                xmlXPathFreeObject(pObject);
            }
            pObject = pGetChildNodes(pNode, (xmlChar*)"CxMin");
            if(pObject != nullptr)
            {
                xmlNodePtr pTemp = pObject->nodesetval->nodeTab[0];
                if(pTemp != nullptr)
                {
                    char* pchKey = (char*)xmlNodeListGetString(pTemp->doc, pTemp->xmlChildrenNode, 1);
                    nCxMax = 0;
                    if(pchKey != nullptr)
                    {
                        nCxMax = atoi(pchKey);
                        xmlFree(pchKey);
                    }
                }
                else
                {
                    nRetVal = S_FALSE;
                }
                xmlXPathFreeObject(pObject);
            }
        }
        return nRetVal;
    }

    static int parseColumnInfoNode(xmlNodeSetPtr pNodeSet, ColumnInfoMap& InfoMap)
    {
        int nNodes = pNodeSet->nodeNr;

        int* npOrder = new int[pNodeSet->nodeNr];
        memset(npOrder, -1, sizeof(int)*pNodeSet->nodeNr);

        int nRetVal = S_OK;
        for( int i = 0; i < pNodeSet->nodeNr; i++ )
        {
            xmlNodePtr pNode = pNodeSet->nodeTab[i]->children;
            if( pNode != nullptr )
            {
                columnInfo sColumnInfo;
                if( S_OK == parseColumnNode(pNode, sColumnInfo))
                {
                    if(sColumnInfo.nOrder <= pNodeSet->nodeNr && sColumnInfo.nOrder >= 1)
                    {
                        sColumnInfo.nOrder -= 1;
                        InfoMap[sColumnInfo.strId] = sColumnInfo;
                        npOrder[sColumnInfo.nOrder] = 0;
                    }
                    else
                    {
                        nRetVal = S_FALSE;
                        break;
                    }
                }
            }
        }
        if(nRetVal == S_OK )
        {
            for(int i = 0 ; i < pNodeSet->nodeNr; i++)
            {
                if( npOrder[i] == -1 )
                {
                    nRetVal = S_FALSE;
                }
            }
        }

        delete[] npOrder;
        return nRetVal;
    }
};
