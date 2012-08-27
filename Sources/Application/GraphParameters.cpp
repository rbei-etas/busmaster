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
 * \file      GraphParameters.cpp
 * \brief     Implementation file for CGraphParameters class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CGraphParameters class
 */

// For standard Headers
#include "stdafx.h"
//SGW Code commented by Arun 21-10-2010
// For App declaration
#include "BUSMASTER.h"
//SGW Code commented by Arun 21-10-2010
// For CGraphParameters class declaration
#include "GraphParameters.h"
#include "Include/XMLDefines.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*******************************************************************************
 Function Name  : CGraphParameters
 Input(s)       : -
 Output         : -
 Functionality  : This function will Update global structure with the user
                  selected values.
 Member of      : CGraphParameters
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  :
*******************************************************************************/
CGraphParameters::CGraphParameters()
{
    vInitialize();
}
void CGraphParameters::vInitialize(void)
{
    // Set the default values
    m_nBufferSize = defDEFAULT_BUFFER_SIZE;
    // Display refresh rate
    m_nRefreshRate = defDEFAULT_UPDATE_RATE;
    // View Style Configuration
    // Frame Color
    m_nFrameColor = defDEFAULT_FRAME_COLOR;
    // Frame Style
    m_nFrameStyle = defDEFAULT_FRAME_TYPE;
    // Plot Area Color
    m_nPlotAreaColor = defDEFAULT_PLOT_AREA_COLOR;
    // Grid Color
    m_nGridColor = defDEFAULT_GRID_COLOR;
    // Axis Color
    m_nAxisColor = defDEFAULT_AXIS_COLOR;
    // X Grid Lines
    m_nXGridLines = defDEFAULT_X_GRID_LINES; // 10
    // Y Grid Lines
    m_nYGridLines = defDEFAULT_Y_GRID_LINES; // 5
    // User Selected Active Axis
    m_nActiveAxis = defZOOM_XY_AXIS; // XY
    // User Selected Action
    m_nAction = defTRACK_MODE_NONE; // None
    // Grid Setting
    m_bShowGrid = TRUE;
    //Graph Line Display
    m_eDisplayType = eDISPLAY_NORMAL;
}
/*******************************************************************************
 Function Name  : ~CGraphParameters
 Input(s)       : -
 Output         : -
 Functionality  : This function will Update global structure with the user
                  selected values.
 Member of      : CGraphParameters
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  :
*******************************************************************************/
CGraphParameters::~CGraphParameters()
{

}
BYTE* CGraphParameters::pbyGetConfigData(BYTE* pbyTrgtData, BYTE byVersion)
{
    BYTE* pbyTemp = pbyTrgtData;
    if (pbyTemp != NULL)
    {
        // Set the def
        COPY_DATA(pbyTemp, &m_nBufferSize, sizeof(int));
        // Display ref
        COPY_DATA(pbyTemp, &m_nRefreshRate, sizeof(int));
        // View Style
        // Frame Color
        COPY_DATA(pbyTemp, &m_nFrameColor, sizeof(int));
        // Frame Style
        COPY_DATA(pbyTemp, &m_nFrameStyle, sizeof(int));
        // Plot Area C
        COPY_DATA(pbyTemp, &m_nPlotAreaColor, sizeof(int));
        // Grid Color
        COPY_DATA(pbyTemp, &m_nGridColor, sizeof(int));
        // Axis Color
        COPY_DATA(pbyTemp, &m_nAxisColor, sizeof(int));
        // X Grid Line
        COPY_DATA(pbyTemp, &m_nXGridLines, sizeof(int));
        // Y Grid Line
        COPY_DATA(pbyTemp, &m_nYGridLines, sizeof(int));
        // User Select
        COPY_DATA(pbyTemp, &m_nActiveAxis, sizeof(int));
        // User Select
        COPY_DATA(pbyTemp, &m_nAction, sizeof(int));
        // Grid Settin
        COPY_DATA(pbyTemp, &m_bShowGrid, sizeof(BOOL));
        if(byVersion == 2)
        {
            //Line Display type
            COPY_DATA(pbyTemp, &m_eDisplayType, sizeof(eDISPLAY_TYPE));
        }
    }
    return pbyTemp;
}
// PTV XML
void CGraphParameters::pbyGetConfigData(xmlNodePtr pNodePtr, BYTE byVersion)
{
    // Writing Graphical parameters in to xml
    xmlNodePtr pGrphParams = xmlNewNode(NULL, BAD_CAST DEF_GRAPH_PARAMETERS);
    xmlAddChild(pNodePtr, pGrphParams);

    // Writing buffersize in to xml
    CString strBufferSize = "";
    strBufferSize.Format("%d", m_nBufferSize);

    xmlNodePtr pBufferSize = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Buffer_Size, BAD_CAST strBufferSize.GetBuffer(strBufferSize.GetLength()));
    xmlAddChild(pGrphParams, pBufferSize);

    // Writing Refresh rate in to xml
    CString strRefreshRate = "";
    strRefreshRate.Format("%d", m_nRefreshRate);

    xmlNodePtr pRefrshRate = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Refresh_Rate, BAD_CAST strRefreshRate.GetBuffer(strRefreshRate.GetLength()));
    xmlAddChild(pGrphParams, pRefrshRate);

    // Writing Frame Color in to xml
    CString strFrameColor = "";
    strFrameColor.Format("%d", m_nFrameColor);

    xmlNodePtr pFrameColor = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Frame_Color, BAD_CAST strFrameColor.GetBuffer(strFrameColor.GetLength()));
    xmlAddChild(pGrphParams, pFrameColor);

    // Writing to Frame Style in to xml
    CString strFrameStyle = "";
    strFrameStyle.Format("%d", m_nFrameStyle);

    xmlNodePtr pFrameStyle = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Frame_Style, BAD_CAST strFrameStyle.GetBuffer(strFrameStyle.GetLength()));
    xmlAddChild(pGrphParams, pFrameStyle);

    // Writing Plot area color in to xml
    CString strPltAreaClr = "";
    strPltAreaClr.Format("%d", m_nPlotAreaColor);

    xmlNodePtr pPltAreaClr = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Plot_Area_Color, BAD_CAST strPltAreaClr.GetBuffer(strPltAreaClr.GetLength()));
    xmlAddChild(pGrphParams, pPltAreaClr);

    // Writing Grid color in to xml
    CString strGridClr = "";
    strGridClr.Format("%d", m_nGridColor);

    xmlNodePtr pGridClr = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Grid_Color, BAD_CAST strGridClr.GetBuffer(strGridClr.GetLength()));
    xmlAddChild(pGrphParams, pGridClr);

    // Writing Axis Color in to xml
    CString strAxisClr = "";
    strAxisClr.Format("%d", m_nAxisColor);

    xmlNodePtr pAxisClr = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Axis_Color, BAD_CAST strAxisClr.GetBuffer(strAxisClr.GetLength()));
    xmlAddChild(pGrphParams, pAxisClr);

    // Writing XGridLines in to xml
    CString strXGrdAxisLines = "";
    strXGrdAxisLines.Format("%d", m_nXGridLines);

    xmlNodePtr pXGrdAxisLines = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_X_Grid_Lines, BAD_CAST strXGrdAxisLines.GetBuffer(strXGrdAxisLines.GetLength()));
    xmlAddChild(pGrphParams, pXGrdAxisLines);

    // Writing YGridLines in to xml
    CString strYGrdAxisLines = "";
    strYGrdAxisLines.Format("%d", m_nYGridLines);

    xmlNodePtr pYGrdAxisLines = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Y_Grid_Lines, BAD_CAST strYGrdAxisLines.GetBuffer(strYGrdAxisLines.GetLength()));
    xmlAddChild(pGrphParams, pYGrdAxisLines);

    // Writing Active Axis in to xml
    CString strActiveAxis = "";
    strActiveAxis.Format("%d", m_nActiveAxis);

    xmlNodePtr pActiveAxis = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Active_Axis, BAD_CAST strActiveAxis.GetBuffer(strActiveAxis.GetLength()));
    xmlAddChild(pGrphParams, pActiveAxis);

    // Writing Action in to xml
    CString strAction = "";
    strAction.Format("%d", m_nAction);

    xmlNodePtr pActionPtr = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Selected_Action, BAD_CAST strAction.GetBuffer(strAction.GetLength()));
    xmlAddChild(pGrphParams, pActionPtr);

    // Writing Show Grid in to xml
    CString strShowGrid = "";

    if(m_bShowGrid == TRUE)
    {
        strShowGrid = "TRUE";
    }
    else
    {
        strShowGrid = "FALSE";
    }

    // Writing show grid in to xml
    xmlNodePtr pShwGridPtr = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Show_Grid, BAD_CAST strShowGrid.GetBuffer(strShowGrid.GetLength()));
    xmlAddChild(pGrphParams, pShwGridPtr);

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

        // Writing display type in to xml
        xmlNodePtr pDisplayTypePtr = xmlNewChild(pGrphParams, NULL, BAD_CAST DEF_Display_Type, BAD_CAST strDisplayType.GetBuffer(strDisplayType.GetLength()));
        xmlAddChild(pGrphParams, pDisplayTypePtr);
    }

    //BYTE* pbyTemp = pbyTrgtData;
    //if (pbyTemp != NULL)
    //{
    //    // Set the def
    //    COPY_DATA(pbyTemp, &m_nBufferSize, sizeof(int));
    //    // Display ref
    //    COPY_DATA(pbyTemp, &m_nRefreshRate, sizeof(int));
    //    // View Style
    //    // Frame Color
    //    COPY_DATA(pbyTemp, &m_nFrameColor, sizeof(int));
    //    // Frame Style
    //    COPY_DATA(pbyTemp, &m_nFrameStyle, sizeof(int));
    //    // Plot Area C
    //    COPY_DATA(pbyTemp, &m_nPlotAreaColor, sizeof(int));
    //    // Grid Color
    //    COPY_DATA(pbyTemp, &m_nGridColor, sizeof(int));
    //    // Axis Color
    //    COPY_DATA(pbyTemp, &m_nAxisColor, sizeof(int));
    //    // X Grid Line
    //    COPY_DATA(pbyTemp, &m_nXGridLines, sizeof(int));
    //    // Y Grid Line
    //    COPY_DATA(pbyTemp, &m_nYGridLines, sizeof(int));
    //    // User Select
    //    COPY_DATA(pbyTemp, &m_nActiveAxis, sizeof(int));
    //    // User Select
    //    COPY_DATA(pbyTemp, &m_nAction, sizeof(int));
    //    // Grid Settin
    //    COPY_DATA(pbyTemp, &m_bShowGrid, sizeof(BOOL));
    //    if(byVersion == 2)
    //    {
    //        //Line Display type
    //        COPY_DATA(pbyTemp, &m_eDisplayType, sizeof(eDISPLAY_TYPE));
    //    }
    //}
    // return pbyTemp;
}

void CGraphParameters::pbySetConfigData(xmlNodePtr& pNodePtr, xmlDocPtr xmlConfigFiledoc)
{
    if(xmlConfigFiledoc != NULL)
    {
        pNodePtr = pNodePtr->xmlChildrenNode;

        while(pNodePtr != NULL)
        {
            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Buffer_Size")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {

                    m_nBufferSize = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Refresh_Rate")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nRefreshRate = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Frame_Color")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nFrameColor = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Frame_Style")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nFrameStyle = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Plot_Area_Color")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nPlotAreaColor = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Grid_Color")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nGridColor = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Axis_Color")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nAxisColor = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"X_Grid_Lines")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nXGridLines = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Y_Grid_Lines")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nYGridLines = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Active_Axis")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nActiveAxis = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Selected_Action")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    m_nAction = atoi((CString)ptext);
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Show_Grid")))
            {
                xmlChar* ptext = xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != ptext)
                {
                    CString strSG = ptext;
                    if(strSG == "TRUE")
                    {
                        m_bShowGrid = TRUE;
                    }
                    else
                    {
                        m_bShowGrid = FALSE;
                    }
                    xmlFree(ptext);
                }
            }

            if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Display_Type")))
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
    //BYTE* pbyTemp = pbyTrgtData;
    //   if (pbyTemp != NULL)
    //   {
    //       // Set the def
    //       COPY_DATA_2(&m_nBufferSize, pbyTemp, sizeof(int));
    //       // Display ref
    //       COPY_DATA_2(&m_nRefreshRate, pbyTemp, sizeof(int));
    //       // View Style
    //       // Frame Color
    //       COPY_DATA_2(&m_nFrameColor, pbyTemp, sizeof(int));
    //       // Frame Style
    //       COPY_DATA_2(&m_nFrameStyle, pbyTemp, sizeof(int));
    //       // Plot Area C
    //       COPY_DATA_2(&m_nPlotAreaColor, pbyTemp, sizeof(int));
    //       // Grid Color
    //       COPY_DATA_2(&m_nGridColor, pbyTemp, sizeof(int));
    //       // Axis Color
    //       COPY_DATA_2(&m_nAxisColor, pbyTemp, sizeof(int));
    //       // X Grid Line
    //       COPY_DATA_2(&m_nXGridLines, pbyTemp, sizeof(int));
    //       // Y Grid Line
    //       COPY_DATA_2(&m_nYGridLines, pbyTemp, sizeof(int));
    //       // User Select
    //       COPY_DATA_2(&m_nActiveAxis, pbyTemp, sizeof(int));
    //       // User Select
    //       COPY_DATA_2(&m_nAction, pbyTemp, sizeof(int));
    //       // Grid Settin
    //       COPY_DATA_2(&m_bShowGrid, pbyTemp, sizeof(BOOL));
    //       if(byVersion == 2)
    //       {
    //           //Line Display type
    //           COPY_DATA_2(&m_eDisplayType, pbyTemp, sizeof(eDISPLAY_TYPE));
    //       }
    //       else if(byVersion == 1)
    //       {
    //           m_eDisplayType = eDISPLAY_NORMAL;
    //       }
    //   }
}

// PTV XML
BYTE* CGraphParameters::pbySetConfigData(BYTE* pbyTrgtData, BYTE byVersion)
{
    BYTE* pbyTemp = pbyTrgtData;
    if (pbyTemp != NULL)
    {
        // Set the def
        COPY_DATA_2(&m_nBufferSize, pbyTemp, sizeof(int));
        // Display ref
        COPY_DATA_2(&m_nRefreshRate, pbyTemp, sizeof(int));
        // View Style
        // Frame Color
        COPY_DATA_2(&m_nFrameColor, pbyTemp, sizeof(int));
        // Frame Style
        COPY_DATA_2(&m_nFrameStyle, pbyTemp, sizeof(int));
        // Plot Area C
        COPY_DATA_2(&m_nPlotAreaColor, pbyTemp, sizeof(int));
        // Grid Color
        COPY_DATA_2(&m_nGridColor, pbyTemp, sizeof(int));
        // Axis Color
        COPY_DATA_2(&m_nAxisColor, pbyTemp, sizeof(int));
        // X Grid Line
        COPY_DATA_2(&m_nXGridLines, pbyTemp, sizeof(int));
        // Y Grid Line
        COPY_DATA_2(&m_nYGridLines, pbyTemp, sizeof(int));
        // User Select
        COPY_DATA_2(&m_nActiveAxis, pbyTemp, sizeof(int));
        // User Select
        COPY_DATA_2(&m_nAction, pbyTemp, sizeof(int));
        // Grid Settin
        COPY_DATA_2(&m_bShowGrid, pbyTemp, sizeof(BOOL));
        if(byVersion == 2)
        {
            //Line Display type
            COPY_DATA_2(&m_eDisplayType, pbyTemp, sizeof(eDISPLAY_TYPE));
        }
        else if(byVersion == 1)
        {
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
 Member of      : CGraphParameters
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  :
*******************************************************************************/
int CGraphParameters::nSerialize(CArchive& omArchive)
{
    int nReturn = 0;
    try
    {
        // If it is storing
        if( omArchive.IsStoring() )
        {
            // Set the def
            omArchive << m_nBufferSize;
            // Display ref
            omArchive << m_nRefreshRate;
            // View Style
            // Frame Color
            omArchive << m_nFrameColor;
            // Frame Style
            omArchive << m_nFrameStyle;
            // Plot Area C
            omArchive << m_nPlotAreaColor;
            // Grid Color
            omArchive << m_nGridColor;
            // Axis Color
            omArchive << m_nAxisColor;
            // X Grid Line
            omArchive << m_nXGridLines;
            // Y Grid Line
            omArchive << m_nYGridLines;
            // User Select
            omArchive << m_nActiveAxis;
            // User Select
            omArchive << m_nAction ;
            // Grid Settin
            omArchive << m_bShowGrid;
        }
        // This is for loading
        else
        {
            // Set the def
            omArchive >> m_nBufferSize;
            // Display ref
            omArchive >> m_nRefreshRate;
            // View Style
            // Frame Color
            omArchive >> m_nFrameColor;
            // Frame Style
            omArchive >> m_nFrameStyle;
            // Plot Area C
            omArchive >> m_nPlotAreaColor;
            // Grid Color
            omArchive >> m_nGridColor;
            // Axis Color
            omArchive >> m_nAxisColor;
            // X Grid Line
            omArchive >> m_nXGridLines;
            // Y Grid Line
            omArchive >> m_nYGridLines;
            // User Select
            omArchive >> m_nActiveAxis;
            // User Select
            omArchive >> m_nAction ;
            // Grid Settin
            omArchive >> m_bShowGrid;
        }
    }
    catch(CArchiveException* poArchExcep)
    {
        // Get the Error Code and delete dynamic object
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
 Member of      : CGraphParameters
 Author(s)      : ArunKumar K
 Date Created   : 16.12.2010
 Modifications  :
*******************************************************************************/
UINT CGraphParameters::unGetConfigSize(BYTE byVersion)
{
    UINT unSize = 0;
    // Display perf conf.
    // Buffer size
    unSize += sizeof(int);
    // Display refresh rate
    unSize += sizeof(int);
    // View Style Configuration
    // Frame Color
    unSize += sizeof(int);
    // Frame Style
    unSize += sizeof(int);
    // Plot Area Color
    unSize += sizeof(int);
    // Grid Color
    unSize += sizeof(int);
    // Axis Color
    unSize += sizeof(int);
    // X Grid Lines
    unSize += sizeof(int);
    // Y Grid Lines
    unSize += sizeof(int);
    // User Selected Active Axis
    unSize += sizeof(int);
    // User Selected Action
    unSize += sizeof(int);
    // Grid Setting
    unSize += sizeof(BOOL);
    if(byVersion == 2)
    {
        // Line Display
        unSize += sizeof(int);
    }

    return unSize;
}
