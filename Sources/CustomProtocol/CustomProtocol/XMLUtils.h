//MVN
#pragma once
//libxml file includes
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
//#include <include/BaseDefs.h>
//#include <DataTypes\MsgSignal_Datatypes.h>
#include <string>
#include <map>
#include <list>
//#include <include\XMLDefines.h>
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

        //char*  cData = NULL;
        strData.clear();
        if (!xmlStrcmp(pNodePtr->name, xmlNodeName))
        {
            xmlChar* xmlData = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode,1);
            if(NULL != xmlData)
            {
                strData = (char*)xmlData;
                xmlFree(xmlData);
            }
            return true;
        }
        return false;
    }
 
};

//~MVN
