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
 * \file      Utility_Structs.h
 * \brief     This file contains definitions of utility structures
 * \authors   Ravi D., Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains definitions of utility structures
 */

#pragma once

#define SIGNED_VAL  1
#define FLOAT_VAL   2
#define BUDDY_CTRL  4

#define IS_FLOAT_ENABLED(x)  ((x) & (FLOAT_VAL) ? TRUE : FALSE )
#define IS_BUDDY_ENABLED(x)  ((x) & (BUDDY_CTRL) ? TRUE : FALSE )
#define IS_SIGNED_NUMBER(x)  ((x) & (SIGNED_VAL) ? TRUE : FALSE )

#define defBASE_DEC                         10
#define defBASE_HEX                         16
#define defCREATE_FAILED                    -1
#define defFORMAT_INT64_DECIMAL             _T("%I64d")
#define defFORMAT_INT64_HEX                 _T("%I64X")
#define defSTR_FORMAT_PHY_VALUE             _T("%.3f")
#define defFLC_CREATE_FAILED                _T("Failed to create %s Control")
#define defCOMBO_ITEM                       _T("ComboItem")
#define defCOMBO_LIST                       _T("ComboLsit")
#define defEDIT_ITEM                        _T("EditItem")
#define defNUM_ITEM                         _T("NumItem")
#define defLIST_ITEM_TYPE_TABLE_SIZE        131
#define defLIST_NUM_DETAILS_TABLE_SIZE      67
#define defVIRTUAL_COL_COUNT                50
#define STR_EMPTY                           _T("")
#define defMAX_BITS                         64
#define defSIGN_MASK                0x8000000000000000

typedef void (*PFCTRLHANDLER)(CListCtrl *pList, int nItem, int nSubItem, void * UParam);

struct sUserProgInfo
{
    PFCTRLHANDLER   m_pfHandler;
    void *          m_pUserParam;
};

typedef sUserProgInfo   SUSERPROGINFO;
typedef CMap<int,int, SUSERPROGINFO, SUSERPROGINFO>   CUserProgList;

enum eListTypes
{
    eText,
    eNumber,
    eBuddy,
    eComboList, // Editable
    eComboItem,  // Non- Editable
    eBool,
    eUser,       // User function will be called
    eBrowser,
    eNoControl
};
typedef eListTypes LIST_TYPES;

struct sListInfo
{
    eListTypes  m_eType;
    CStringArray m_omEntries;
    
    sListInfo();
    sListInfo(sListInfo& sCopy);
    void operator = (sListInfo& sCopy);
};
typedef sListInfo  SLISTINFO;
typedef CMap<int,int, SLISTINFO, SLISTINFO>   CListTypes;

union uDN64Number
{
    double  m_dValue;
    __int64 m_n64Value;
};

typedef uDN64Number UDN64NUMBER;
//
struct sNumericInfo
{
    BYTE    m_byBase;                       //Base value of the number
    //Minimun, Maximun and step values in Double
    UDN64NUMBER  m_uMinVal, m_uMaxVal, m_uDelta;
    BYTE    m_byFlag;                       //Type flag Float, Buddy & Signed
    short int m_nTextLimit;                 //Allowed Text width
    short int m_nSigLength;                 //Length of the num representation.
                                            //This is required for 2s complement
    sNumericInfo() : m_byBase(10),
                     m_byFlag(0),
                     m_nTextLimit(0),
                     m_nSigLength(64)
    {
        m_uMinVal.m_n64Value = 0;
        m_uMaxVal.m_n64Value = 0;
        m_uDelta.m_n64Value = 0;
    }
};

typedef sNumericInfo  SNUMERICINFO;
typedef CMap<int,int, SNUMERICINFO, SNUMERICINFO>   CNumericInfo;
