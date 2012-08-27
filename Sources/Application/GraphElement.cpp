/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      GraphElement.cpp
 * \brief     Implementation file for CGraphElement class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CGraphElement class
 */
// For standard Headers
#include "stdafx.h"
// For App declaration
#include "BUSMASTER.h"
// For CGraphElement class declaration
#include "GraphElement.h"
#include "InterfaceGetter.h"

/*******************************************************************************
 Function Name  : CGraphElement
 Description    : Standard default constructor
 Member of      : CGraphElement
 Functionality  : This will initialise local variables
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  :
*******************************************************************************/
CGraphElement::CGraphElement()
{
    // Message ID
    m_nMsgID = -1;
    // Message Name
    m_strMsgName = STR_EMPTY;
    // Format - Standard or Extended
    m_nFrameFormat = -1;
    // Element Name
    m_omStrElementName = STR_EMPTY;
    // Value Type
    m_nValueType = -1;
    // Graph Type
    m_nLineType = 0; // For Solid
    // Default Color
    m_nLineColor = RED_COLOR;
    // Sample point type
    m_nPointType = 0; // None
    // Sample point color
    m_nPointColor = 0; // Black
    // Visible
    m_bVisible = TRUE;
    // Enabled
    m_bEnabled = TRUE;
    //Display Type Normal
    m_eDisplayType = eDISPLAY_NORMAL;
}

/*******************************************************************************
 Function Name  : CGraphElement
 Description    : Standard Destructor
 Member of      : CGraphElement
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 01.12.2004
*******************************************************************************/
CGraphElement::~CGraphElement()
{

}

BYTE* CGraphElement::pbyGetConfigData(BYTE* pbyTrgData, BYTE byVersion)
{
    BYTE* pbyTemp = pbyTrgData;
    if (pbyTemp != NULL)
    {
        // Save properties one after another
        // Message ID
        COPY_DATA(pbyTemp, &m_nMsgID, sizeof(int));
        // Frame Format - Standard
        COPY_DATA(pbyTemp, &m_nFrameFormat, sizeof(short));
        // Element Name String
        //Tobias - venkat
        char acName[MAX_PATH] = {'\0'};
        strcpy_s(acName, MAX_PATH, m_omStrElementName.GetBuffer(MAX_PATH));
        COPY_DATA(pbyTemp, acName, (sizeof(char) * MAX_PATH));
        // Type of the element val
        COPY_DATA(pbyTemp, &m_nValueType, sizeof(int));
        // Line type of the elemen
        COPY_DATA(pbyTemp, &m_nLineType, sizeof(int));
        // Line Color of the eleme
        COPY_DATA(pbyTemp, &m_nLineColor, sizeof(int));
        // Sample point symbol typ
        COPY_DATA(pbyTemp, &m_nPointType, sizeof(int));
        // Sample point symbol col
        COPY_DATA(pbyTemp, &m_nPointColor, sizeof(int));
        // Visible or Not
        COPY_DATA(pbyTemp, &m_bVisible, sizeof(BOOL));
        // Enabled or not
        COPY_DATA(pbyTemp, &m_bEnabled, sizeof(BOOL));

        if(byVersion == 2)
        {
            // Message Name
            char MsgName[MAX_PATH] = {'\0'};
            //Tobias - venkat
            strcpy_s(MsgName,MAX_PATH,  m_strMsgName.GetBuffer(MAX_PATH));
            COPY_DATA(pbyTemp, MsgName, (sizeof(char) * MAX_PATH));
            //Line Display type
            COPY_DATA(pbyTemp, &m_eDisplayType, sizeof(eDISPLAY_TYPE));
        }
    }
    return pbyTemp;
}

// PTV XML
void CGraphElement::pbyGetConfigData(xmlNodePtr pNodePtr, BYTE byVersion)
{
    // Writing Graph element in to xml
    xmlNodePtr pGrphElmntPtr = xmlNewNode(NULL, BAD_CAST DEF_Graph_Element);
    xmlAddChild(pNodePtr, pGrphElmntPtr);

    // Writing Message Id in to xml
    CString strMsgId = "";
    strMsgId.Format("%d", m_nMsgID);

    xmlNodePtr pMsgIdPtr = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_MESSAGE_ID, BAD_CAST strMsgId.GetBuffer(strMsgId.GetLength()));
    xmlAddChild(pGrphElmntPtr, pMsgIdPtr);

    // Writing Message Name in to xml
    xmlNodePtr pMsgNamePtr = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_MESSAGE_NAME, BAD_CAST m_strMsgName.GetBuffer(m_strMsgName.GetLength()));
    xmlAddChild(pGrphElmntPtr, pMsgNamePtr);

    // Writing Frame Format in to xml
    CString strFrmeFormat = "";

    if(m_nFrameFormat == FRAME_FORMAT_STD)
    {
        strFrmeFormat = "STANDARD";
    }
    else if(m_nFrameFormat == FRAME_FORMAT_EXND)
    {
        strFrmeFormat = "EXTENDED";
    }

    xmlNodePtr pFrmFormatPtr = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_FRAME_FORMAT, BAD_CAST strFrmeFormat.GetBuffer(strFrmeFormat.GetLength()));
    xmlAddChild(pGrphElmntPtr, pFrmFormatPtr);

    // Writing Element name in to xml
    xmlNodePtr pElmntNamePtr = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_ELEMENT_NAME, BAD_CAST m_omStrElementName.GetBuffer(m_omStrElementName.GetLength()));
    xmlAddChild(pGrphElmntPtr, pElmntNamePtr);

    // Writing Value type in to xml
    CString strValueType = "";

    if(m_nValueType == eSTAT_PARAM)
    {
        strValueType = defSTR_STATISTICS_NAME;
    }
    else if(m_nValueType == eRAW_VALUE)
    {
        strValueType = defSTR_RAW_VALUE;
    }
    else if(m_nValueType == ePHY_VALUE)
    {
        strValueType = defSTR_PHY_VALUE;
    }

    xmlNodePtr pValueType = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_VALUE_TYPE,BAD_CAST  strValueType.GetBuffer(strValueType.GetLength()));
    xmlAddChild(pGrphElmntPtr, pValueType);

    // Writing Line Type in to xml
    CString strLineType = "";

    strLineType = xmlUtils::nGetLineTypeForGraph(m_nLineType);

    xmlNodePtr pLineType = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_LINE_TYPE,BAD_CAST  strLineType.GetBuffer(strLineType.GetLength()));
    xmlAddChild(pGrphElmntPtr, pLineType);

    // Writing Line color in to xml
    CString strLineColor = "";

    strLineColor.Format("%d", m_nLineColor);

    xmlNodePtr pLineColor = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_LINE_COLOR,BAD_CAST  strLineColor.GetBuffer(strLineColor.GetLength()));
    xmlAddChild(pGrphElmntPtr, pLineColor);

    // Writing Point Type in to xml
    CString strPointType = "";
    strPointType.Format("%d", m_nPointType);

    xmlNodePtr pPointType = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_POINT_TYPE, BAD_CAST strPointType.GetBuffer(strPointType.GetLength()));
    xmlAddChild(pGrphElmntPtr, pPointType);

    // Writing Point Color in to xml
    CString strPointColor = "";
    strPointColor.Format("%d", m_nPointColor);

    xmlNodePtr pPointColor = xmlNewChild(pGrphElmntPtr, NULL,BAD_CAST DEF_POINT_COLOR, BAD_CAST strPointColor.GetBuffer(strPointColor.GetLength()));
    xmlAddChild(pGrphElmntPtr, pPointColor);

    // Writing visibility in to xml
    CString strVisibility = "";

    if(m_bVisible == TRUE)
    {
        strVisibility = "TRUE";
    }
    else if(m_bVisible == FALSE)
    {
        strVisibility = "FALSE";
    }

    xmlNodePtr pVisibility = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_VISIBILITY, BAD_CAST strVisibility.GetBuffer(strVisibility.GetLength()));
    xmlAddChild(pGrphElmntPtr, pVisibility);

    // Writing Enabled in to xml
    CString strEnabled = "";
    if(m_bEnabled == TRUE)
    {
        strEnabled = "TRUE";
    }
    else if(m_bEnabled == FALSE)
    {
        strEnabled = "FALSE";
    }

    xmlNodePtr pEnabledPtr = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_ENABLE, BAD_CAST strEnabled.GetBuffer(strEnabled.GetLength()));
    xmlAddChild(pGrphElmntPtr, pEnabledPtr);

    // Writing Display type in to xml
    if(byVersion == 2)
    {
        CString strDisplayType = "";

        if(m_eDisplayType == eDISPLAY_NORMAL)
        {
            strDisplayType = "NORMAL";
        }
        else if(m_eDisplayType == eDISPLAY_STEPMODE_XY)
        {
            strDisplayType = "STEPMODE_XY";
        }
        else if(m_eDisplayType == eDISPLAY_STEPMODE_YX)
        {
            strDisplayType = "STEPMODE_YX";
        }

        xmlNodePtr pDisplayTypePtr = xmlNewChild(pGrphElmntPtr, NULL, BAD_CAST DEF_DISPLAY_TYPE, BAD_CAST strDisplayType.GetBuffer(strDisplayType.GetLength()));
        xmlAddChild(pGrphElmntPtr, pDisplayTypePtr);
    }


    //BYTE* pbyTemp = pbyTrgData;
    //if (pbyTemp != NULL)
    //{
    //    // Save properties one after another
    //    // Message ID
    //    COPY_DATA(pbyTemp, &m_nMsgID, sizeof(int));
    //    // Frame Format - Standard
    //    COPY_DATA(pbyTemp, &m_nFrameFormat, sizeof(short));
    //    // Element Name String
    //    //Tobias - venkat
    //    char acName[MAX_PATH] = {'\0'};
    //    strcpy_s(acName, MAX_PATH, m_omStrElementName.GetBuffer(MAX_PATH));
    //    COPY_DATA(pbyTemp, acName, (sizeof(char) * MAX_PATH));
    //    // Type of the element val
    //    COPY_DATA(pbyTemp, &m_nValueType, sizeof(int));
    //    // Line type of the elemen
    //    COPY_DATA(pbyTemp, &m_nLineType, sizeof(int));
    //    // Line Color of the eleme
    //    COPY_DATA(pbyTemp, &m_nLineColor, sizeof(int));
    //    // Sample point symbol typ
    //    COPY_DATA(pbyTemp, &m_nPointType, sizeof(int));
    //    // Sample point symbol col
    //    COPY_DATA(pbyTemp, &m_nPointColor, sizeof(int));
    //    // Visible or Not
    //    COPY_DATA(pbyTemp, &m_bVisible, sizeof(BOOL));
    //    // Enabled or not
    //    COPY_DATA(pbyTemp, &m_bEnabled, sizeof(BOOL));

    //    if(byVersion == 2)
    //    {
    //        // Message Name
    //        char MsgName[MAX_PATH] = {'\0'};
    //        //Tobias - venkat
    //        strcpy_s(MsgName,MAX_PATH,  m_strMsgName.GetBuffer(MAX_PATH));
    //        COPY_DATA(pbyTemp, MsgName, (sizeof(char) * MAX_PATH));
    //        //Line Display type
    //        COPY_DATA(pbyTemp, &m_eDisplayType, sizeof(eDISPLAY_TYPE));
    //    }
    //}
    //return pbyTemp;
}

void CGraphElement::pbySetConfigData(xmlNodePtr pNodePtr, xmlDocPtr xmlConfigFiledoc)
{
    if(xmlConfigFiledoc != NULL)
    {
        pNodePtr = pNodePtr->xmlChildrenNode;

        while(pNodePtr != NULL)
        {
            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Message_ID")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strMsgId = "";
                    strMsgId = ptext;
                    m_nMsgID = atoi(strMsgId);

                    xmlFree(ptext);
                }
            }
            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Message_Name")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strMsgName = ptext;
                    m_strMsgName = strMsgName;
                    xmlFree(ptext);
                }
            }
            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Frame_Format")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strFrmeFormat = ptext;

                    if(strFrmeFormat == "STANDARD")
                    {
                        m_nFrameFormat = FRAME_FORMAT_STD;
                    }
                    else if(strFrmeFormat == "EXTENDED")
                    {
                        m_nFrameFormat = FRAME_FORMAT_EXND;
                    }

                    xmlFree(ptext);
                }
            }
            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Element_Name")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_omStrElementName = (CString)ptext;
                    xmlFree(ptext);
                }
            }
            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Value_Type")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strValueType = ptext;

                    if(strValueType == defSTR_STATISTICS_NAME)
                    {
                        m_nValueType = eSTAT_PARAM;
                    }
                    else if(strValueType == defSTR_RAW_VALUE)
                    {
                        m_nValueType = eRAW_VALUE;
                    }
                    else if(strValueType == defSTR_PHY_VALUE)
                    {
                        m_nValueType = ePHY_VALUE;
                    }

                    xmlFree(ptext);
                }
            }

            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Line_Type")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    // Writing Line Type in to xml
                    CString strLineType = ptext;

                    m_nLineType = xmlUtils::nSetLineTypeForGraph(strLineType);

                    xmlFree(ptext);
                }
            }

            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Line_Color")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strLineColor = ptext;
                    m_nLineColor = atoi(strLineColor);

                    xmlFree(ptext);
                }
            }

            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Point_Type")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strPointType = ptext;
                    m_nPointType = atoi(strPointType);

                    xmlFree(ptext);
                }
            }

            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Point_Color")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strPointColor = ptext;
                    m_nPointColor = atoi(strPointColor);

                    xmlFree(ptext);
                }
            }

            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"VIsibility")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strVisibility = ptext;

                    if(strVisibility == "TRUE")
                    {
                        m_bVisible = TRUE;
                    }
                    else if(strVisibility == "FALSE")
                    {
                        m_bVisible = FALSE;
                    }

                    xmlFree(ptext);
                }
            }
            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Enable")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strEnable = ptext;

                    if(strEnable == "TRUE")
                    {
                        m_bEnabled = TRUE;
                    }
                    else if(strEnable == "FALSE")
                    {
                        m_bEnabled = FALSE;
                    }

                    xmlFree(ptext);
                }
            }

            else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Display_Type")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strDisplayType = ptext;

                    if(strDisplayType == "NORMAL")
                    {
                        m_eDisplayType = eDISPLAY_NORMAL;
                    }
                    else if(strDisplayType == "STEPMODE_XY")
                    {
                        m_eDisplayType = eDISPLAY_STEPMODE_XY;
                    }
                    else if(strDisplayType == "STEPMODE_YX")
                    {
                        m_eDisplayType = eDISPLAY_STEPMODE_YX;
                    }

                    xmlFree(ptext);
                }
            }
            pNodePtr = pNodePtr->next;
        }
    }


    //BYTE* pbyTemp = pbyTrgData;
    //if (pbyTemp != NULL)
    //{
    //    // Save properties one after another
    //    // Message ID
    //    COPY_DATA_2(&m_nMsgID, pbyTemp, sizeof(int));
    //    // Frame Format - Standard
    //    COPY_DATA_2(&m_nFrameFormat, pbyTemp, sizeof(short));
    //    // Element Name String
    //    char acName[MAX_PATH] = {_T('\0')};
    //    COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
    //    m_omStrElementName.Format("%s", acName);
    //    // Type of the element val
    //    COPY_DATA_2(&m_nValueType, pbyTemp, sizeof(int));
    //    // Line type of the elemen
    //    COPY_DATA_2(&m_nLineType, pbyTemp, sizeof(int));
    //    // Line Color of the eleme
    //    COPY_DATA_2(&m_nLineColor, pbyTemp, sizeof(int));
    //    // Sample point symbol typ
    //    COPY_DATA_2(&m_nPointType, pbyTemp, sizeof(int));
    //    // Sample point symbol col
    //    COPY_DATA_2(&m_nPointColor, pbyTemp, sizeof(int));
    //    // Visible or Not
    //    COPY_DATA_2(&m_bVisible, pbyTemp, sizeof(BOOL));
    //    // Enabled or not
    //    COPY_DATA_2(&m_bEnabled, pbyTemp, sizeof(BOOL));

    //    if(byVersion == 2)
    //    {
    //        //Message Name
    //        char MsgName[MAX_PATH] = {_T('\0')};
    //        COPY_DATA_2(MsgName, pbyTemp, (sizeof(char) * MAX_PATH));
    //        m_strMsgName.Format("%s", MsgName);
    //        //Line Display type
    //        COPY_DATA_2(&m_eDisplayType, pbyTemp, sizeof(eDISPLAY_TYPE));
    //    }
    //    else if(byVersion == 1)
    //    {
    //        m_strMsgName = GetIMsgDB()->omStrGetMessageNameFromMsgCode(m_nMsgID);
    //        m_eDisplayType = eDISPLAY_NORMAL;
    //    }
    //}
    //return pbyTemp;
}
// PTV XML
BYTE* CGraphElement::pbySetConfigData(BYTE* pbyTrgData, BYTE byVersion)
{
    BYTE* pbyTemp = pbyTrgData;
    if (pbyTemp != NULL)
    {
        // Save properties one after another
        // Message ID
        COPY_DATA_2(&m_nMsgID, pbyTemp, sizeof(int));
        // Frame Format - Standard
        COPY_DATA_2(&m_nFrameFormat, pbyTemp, sizeof(short));
        // Element Name String
        char acName[MAX_PATH] = {_T('\0')};
        COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
        m_omStrElementName.Format("%s", acName);
        // Type of the element val
        COPY_DATA_2(&m_nValueType, pbyTemp, sizeof(int));
        // Line type of the elemen
        COPY_DATA_2(&m_nLineType, pbyTemp, sizeof(int));
        // Line Color of the eleme
        COPY_DATA_2(&m_nLineColor, pbyTemp, sizeof(int));
        // Sample point symbol typ
        COPY_DATA_2(&m_nPointType, pbyTemp, sizeof(int));
        // Sample point symbol col
        COPY_DATA_2(&m_nPointColor, pbyTemp, sizeof(int));
        // Visible or Not
        COPY_DATA_2(&m_bVisible, pbyTemp, sizeof(BOOL));
        // Enabled or not
        COPY_DATA_2(&m_bEnabled, pbyTemp, sizeof(BOOL));

        if(byVersion == 2)
        {
            //Message Name
            char MsgName[MAX_PATH] = {_T('\0')};
            COPY_DATA_2(MsgName, pbyTemp, (sizeof(char) * MAX_PATH));
            m_strMsgName.Format("%s", MsgName);
            //Line Display type
            COPY_DATA_2(&m_eDisplayType, pbyTemp, sizeof(eDISPLAY_TYPE));
        }
        else if(byVersion == 1)
        {
            m_strMsgName = GetIMsgDB()->omStrGetMessageNameFromMsgCode(m_nMsgID);
            m_eDisplayType = eDISPLAY_NORMAL;
        }
    }
    return pbyTemp;
}
/*******************************************************************************
 Function Name  : nSerialize
 Input(s)       : omArchive - CArchive class. Serialisation stream
 Output         : -
 Functionality  : This function will serialise this class using CArchive stream
 Member of      : CGraphElement
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  : ArunKumar K
                  28.10.2010
                  Addition of m_eDisplayType variable for serialization.
*******************************************************************************/
int CGraphElement::nSerialize(CArchive& omArch)
{
    int nReturn = 0;
    // If it is storing
    try
    {
        if( omArch.IsStoring() )
        {
            // Save properties one after another
            // Message ID
            omArch << m_nMsgID  ;
            // Frame Format - Standard
            omArch << m_nFrameFormat;
            // Element Name String
            omArch << m_omStrElementName;
            // Type of the element val
            omArch << m_nValueType;
            // Line type of the elemen
            omArch << m_nLineType;
            // Line Color of the eleme
            omArch << m_nLineColor;
            // Sample point symbol typ
            omArch << m_nPointType;
            // Sample point symbol col
            omArch << m_nPointColor;
            // Visible or Not
            omArch << m_bVisible;
            // Enabled or not
            omArch << m_bEnabled;
        }
        // This is for loading
        else
        {
            // Load properties one after another
            omArch >> m_nMsgID;
            // Frame Format - Standard
            omArch >> m_nFrameFormat;
            // Element Name String
            omArch >> m_omStrElementName;
            // Type of the element val
            omArch >> m_nValueType;
            // Line type of the elemen
            omArch >> m_nLineType;
            // Line Color of the eleme
            omArch >> m_nLineColor;
            // Sample point symbol typ
            omArch >> m_nPointType;
            // Sample point symbol col
            omArch >> m_nPointColor;
            // Visible or Not
            omArch >> m_bVisible;
            // Enabled or not
            omArch >> m_bEnabled;
        }
    }
    catch(CArchiveException* poArchExcep)
    {
        // Get the Exception code and delete the dynamic object
        nReturn = poArchExcep->m_cause;
        poArchExcep->Delete();
    }
    // Return the result
    return nReturn;
}

/*******************************************************************************
 Function Name  : unGetConfigSize
 Input(s)       : byVersion
 Output         : -
 Functionality  : This function will get the size of the class data members.
 Member of      : CGraphElement
 Author(s)      : ArunKumar K
 Date Created   : 16.12.2010
 Modifications  :
*******************************************************************************/
UINT CGraphElement::unGetConfigSize(BYTE byVersion)
{
    UINT unSize = 0;

    unSize += sizeof(int);//     m_nMsgID;
    // Frame Format - Standard or Extended
    unSize += sizeof(short);//   m_nFrameFormat;
    // Element Name String
    unSize += (sizeof(char) * MAX_PATH);//m_omStrElementName
    // Type of the element value
    // 0 - network statistics parameter
    // 1 - Raw Value of the Signal
    // 2 - Physical Value of the signal
    unSize += sizeof(int);//   m_nValueType;
    // Line type of the element
    unSize += sizeof(int);//   m_nLineType;
    // Line Color of the element
    unSize += sizeof(int);//     m_nLineColor;
    // Sample point symbol type
    unSize += sizeof(int);//     m_nPointType;
    // Sample point symbol color
    unSize += sizeof(int);//    m_nPointColor;
    // Visible or Not
    unSize += sizeof(BOOL);//    m_bVisible;
    // Enabled or not
    unSize += sizeof(BOOL);//    m_bEnabled;

    if(byVersion == 2)
    {
        // Line Display
        unSize += sizeof(int);//    m_eDisplayType;
        //Message Name String
        unSize += (sizeof(char) * MAX_PATH);//m_strMsgName
    }

    return unSize;
}

