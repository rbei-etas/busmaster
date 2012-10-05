//MVN
#pragma once
//libxml file includes
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <include/BaseDefs.h>
#include <DataTypes\MsgSignal_Datatypes.h>
#include <string>
#include <map>
#include <list>
#include <include\XMLDefines.h>
using namespace std;
struct columnInfo
{
    string strId;
    int nOrder;
    int nWidth;
    bool isVisble;
};
typedef map<string, columnInfo> ColumnInfoMap;
typedef list<string> stringList;

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

using namespace std;
class xmlUtils
{
public:
    static xmlXPathObjectPtr pGetNodes (xmlDocPtr pDoc, xmlChar* pchXpath)
    {
        xmlXPathContextPtr pDocContext;
        xmlXPathObjectPtr pXpathNodePtr;
        //Get Context
        pDocContext = xmlXPathNewContext(pDoc);
        if ( NULL == pDocContext )
        {
            pXpathNodePtr = NULL;
        }
        else
        {
            //Evaluate Xml xPath
            pXpathNodePtr = xmlXPathEvalExpression(pchXpath, pDocContext);

            if(pXpathNodePtr != NULL)
            {

                //Close The DOC Context
                xmlXPathFreeContext(pDocContext);
                if(xmlXPathNodeSetIsEmpty(pXpathNodePtr->nodesetval))
                {
                    xmlXPathFreeObject(pXpathNodePtr);
                    pXpathNodePtr = NULL;
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
        if ( NULL == pDocContext )
        {
            pXpathNodePtr = NULL;
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
                pXpathNodePtr = NULL;
            }
        }
        return pXpathNodePtr;
    }
    static bool GetDataFrmNode(xmlNodePtr pNodePtr, char* cstrNodeName, string& strData)
    {
        xmlChar* xmlNodeName = (xmlChar* )cstrNodeName;

        xmlChar* xmlData;
        //char*  cData = NULL;
        CString     csData = "";
        strData = "";
        if (!xmlStrcmp(pNodePtr->name, xmlNodeName))
        {
            xmlData = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode,1);
            csData.Format( "%s",xmlData);
            xmlFree(xmlData);
            strData = csData.GetBuffer(0);
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
        while (pNodePtr != NULL)
        {
            //Visibility
            if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Visibility")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
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
                if(NULL != key)
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
                if(NULL != key)
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
                if(NULL != key)
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
                if(NULL != key)
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
                if(NULL != key)
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
        if(nLineType == Solid)
        {
            return defLINETYPE_SOLID;
        }
        else if(nLineType == Dash)
        {
            return defLINETYPE_DASH;
        }
        else if(nLineType == Dot)
        {
            return defLINETYPE_DOT;
        }
        else if(nLineType == DashDot)
        {
            return defLINETYPE_DASHDOT;
        }
        else if(nLineType == DashDotDot)
        {
            return defLINETYPE_DASHDOTDOT;
        }
        else if(nLineType == Null)
        {
            return defLINETYPE_NULL;
        }
        else if(nLineType == XYStep)
        {
            return defLINETYPE_XYSTEP;
        }
        else if(nLineType == YXStep)
        {
            return defLINETYPE_YXSTEP;
        }
        else if(nLineType == Bars)
        {
            return defLINETYPE_BARS;
        }
        else if(nLineType == Stick)
        {
            return defLINETYPE_STICK;
        }
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
            return Bars ;
        }
        else if(strLineType == defLINETYPE_STICK )
        {
            return Stick;
        }
    }

    static string nGetWindowVisibilityInString(int nShowCmd)
    {
        string strShowCmd = "";
        if( nShowCmd == -1)
        {
            return strShowCmd;
        }

        char* strTemp = "";

        if(nShowCmd == SW_HIDE)
        {
            strTemp = "HIDE";
        }
        if(nShowCmd == SW_MAXIMIZE)
        {
            strTemp = "MAXIMIZE";
        }
        if(nShowCmd == SW_MINIMIZE)
        {
            strTemp = "MINIMIZE";
        }
        if(nShowCmd == SW_RESTORE)
        {
            strTemp = "RESTORE";
        }
        if(nShowCmd == SW_SHOW)
        {
            strTemp = "SHOW";
        }
        if(nShowCmd == SW_SHOWMAXIMIZED)
        {
            strTemp = "SHOWMAXIMIZED";
        }
        if(nShowCmd == SW_SHOWMINIMIZED)
        {
            strTemp = "SHOWMINIMIZED";
        }
        if(nShowCmd == SW_SHOWMINNOACTIVE)
        {
            strTemp = "SHOWMINNOACTIVE";
        }
        if(nShowCmd == SW_SHOWNA)
        {
            strTemp = "SHOWNA";
        }
        if(nShowCmd == SW_SHOWNOACTIVATE)
        {
            strTemp = "SHOWNOACTIVATE";
        }
        if(nShowCmd == SW_SHOWNORMAL)
        {
            strTemp = "SHOWNORMAL";
        }
        return strTemp;
    }

    static CString nSetWindowVisibility(INT nShowCmd)
    {
        CString strTemp = "";

        if(nShowCmd == SW_HIDE)
        {
            strTemp = "HIDE";
        }
        if(nShowCmd == SW_MAXIMIZE)
        {
            strTemp = "MAXIMIZE";
        }
        if(nShowCmd == SW_MINIMIZE)
        {
            strTemp = "MINIMIZE";
        }

        if(nShowCmd == SW_RESTORE)
        {
            strTemp = "RESTORE";
        }

        if(nShowCmd == SW_SHOW)
        {
            strTemp = "SHOW";
        }
        if(nShowCmd == SW_SHOWMAXIMIZED)
        {
            strTemp = "SHOWMAXIMIZED";
        }
        if(nShowCmd == SW_SHOWMINIMIZED)
        {
            strTemp = "SHOWMINIMIZED";
        }
        if(nShowCmd == SW_SHOWMINNOACTIVE)
        {
            strTemp = "SHOWMINNOACTIVE";
        }
        if(nShowCmd == SW_SHOWNA)
        {
            strTemp = "SHOWNA";
        }
        if(nShowCmd == SW_SHOWNOACTIVATE)
        {
            strTemp = "SHOWNOACTIVATE";
        }
        if(nShowCmd == SW_SHOWNORMAL)
        {
            strTemp = "SHOWNORMAL";
        }

        return strTemp;
    }

    static int CreateXMLNodeFrmWindowsPlacement(xmlNodePtr pNodePtr, WINDOWPLACEMENT& wndPlacement)
    {
        const char* omcVarChar ;
        string      strVar;

        //visibility
        strVar = nGetWindowVisibilityInString(wndPlacement.showCmd);
        omcVarChar = strVar.c_str();
        xmlNodePtr pVisibility = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_VISIBILITY,BAD_CAST omcVarChar);
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
        xmlNodePtr pWindowPlacement = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_MWND_PLACEMENT,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pWindowPlacement);

        //Top
        CString csTop;
        csTop.Format("%d",  wndPlacement.rcNormalPosition.top);
        omcVarChar = csTop;
        xmlNodePtr pTop= xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_MWND_TOP,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pTop);

        //Left
        CString csLeft;
        csLeft.Format("%d",  wndPlacement.rcNormalPosition.left);
        omcVarChar = csLeft;
        xmlNodePtr pLeft = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_MWND_LEFT,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pLeft);

        //Bottom
        CString csBottom;
        csBottom.Format("%d",  wndPlacement.rcNormalPosition.bottom);
        omcVarChar = csBottom;
        xmlNodePtr pBottom= xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_BOTTOM,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pBottom);

        //Right
        CString csRight;
        csRight.Format("%d",  wndPlacement.rcNormalPosition.right);
        omcVarChar = csRight;
        xmlNodePtr pRight = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_RIGHT,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pRight);
        return true;
    }


    static int nGetWindowVisibility(char* pchVisibility)
    {
        int nShowCmd = SW_SHOW;
        if( pchVisibility == NULL)
        {
            return nShowCmd;
        }

        string strTemp = pchVisibility;

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
    static BOOL bGetBooleanValue(char* pchBoolValue)
    {
        BOOL bRetValue = TRUE;
        if ( NULL != pchBoolValue )
        {
            string strTemp = pchBoolValue;

            if(strTemp == "FALSE" || strTemp == "0")
            {
                bRetValue = FALSE;
            }
        }
        return bRetValue;
    }
    static eDirection bGetDirection(char* pchBoolValue)
    {
        eDirection bRetValue = DIR_ALL;
        if ( NULL != pchBoolValue )
        {
            string strTemp = pchBoolValue;
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
    static eTimerMode eGetTimerMode(string strName)
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
    static eFormat eGetNumericMode(string strName)
    {
        eFormat eMode = DEC;
        if(strName == "HEX")
        {
            eMode = HEXADECIMAL;
        }
        return eMode;
    }
    static eWAVEFORMTYPE eGetWaveType(string strName)
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
        return eWaveType;
    }
    static int parseColumnNode(xmlNodePtr pNode, columnInfo& Info)
    {
        int nRetVal = S_OK;
        if(pNode == NULL)
        {
            nRetVal = S_FALSE;
        }
        else
        {
            while(pNode != NULL)
            {
                if ((!xmlStrcmp(pNode->name, (const xmlChar*)"ID")))
                {
                    xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                    if(NULL != key)
                    {
                        Info.strId = (char*)key;
                        xmlFree(key);
                    }
                }
                else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Order")))
                {
                    xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                    if(NULL != key)
                    {
                        Info.nOrder = atoi((char*)key);
                        xmlFree(key);
                    }
                }
                else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Width")))
                {
                    xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                    if(NULL != key)
                    {
                        Info.nWidth = atoi((char*)key);
                        xmlFree(key);
                    }
                }
                else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"IsVisible")))
                {
                    xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                    if(NULL != key)
                    {
                        Info.isVisble = (bool)bGetBooleanValue((char*)key);
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
        if(pNode == NULL)
        {
            nRetVal = S_FALSE;
        }
        else
        {
            xmlXPathObjectPtr pObject = pGetChildNodes(pNode, (xmlChar*)"CxIdeal");
            if(pObject != NULL)
            {
                xmlNodePtr pTemp = pObject->nodesetval->nodeTab[0];
                if(pTemp != NULL)
                {
                    char* pchKey = (char*)xmlNodeListGetString(pTemp->doc, pTemp->xmlChildrenNode, 1);
                    nCxCur = 0;
                    if(pchKey != NULL)
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
            if(pObject != NULL)
            {
                xmlNodePtr pTemp = pObject->nodesetval->nodeTab[0];
                if(pTemp != NULL)
                {
                    char* pchKey = (char*)xmlNodeListGetString(pTemp->doc, pTemp->xmlChildrenNode, 1);
                    nCxMax = 0;
                    if(pchKey != NULL)
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
            if( pNode != NULL )
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
    /*static int nGetRGBColor(int nValue, COLORREF& colorRef)
    {
        RGB(
    }*/
};

//~MVN
