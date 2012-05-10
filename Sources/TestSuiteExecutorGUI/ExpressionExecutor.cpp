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
 * \file      ExpressionExecutor.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "TSExecutorGUI_StdAfx.h"
#include "ExpressionExecutor.h"
extern "C" int bGetExpressionResult(char *pchExpression);
/******************************************************************************
Function Name  :  SubstituteX
Input(s)       :  DWORD dwVal - value that has to be replaced with 
Output         :  BOOL
Functionality  :   
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :  
******************************************************************************/
BOOL CExpressionExecutor::SubstituteX(CString& omFormula, float fVal)
{
    CString omStrVal;
    omStrVal.Format("%f", fVal);
    omFormula.Replace("X", "x");
    omFormula.Replace("x", omStrVal);
    return TRUE;
}
/******************************************************************************
Function Name  :  bGetExpressionValue
Input(s)       :  CString omFormula - Expression
                  DWORD dwValue - Variable Value
Output         :  BOOL
Functionality  :  Evaluates the Expressioin 
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :  
******************************************************************************/
BOOL CExpressionExecutor::bGetExpressionValue(CString omFormula, float fValue)
{
    if(omFormula.GetLength() == 0)
    {
        return FALSE;
    }
    SubstituteX(omFormula, fValue);
	char chBuffer[1024] = {'\n'};
	strcpy(chBuffer, omFormula.GetBuffer(MAX_PATH));
	chBuffer[omFormula.GetLength()-1] = '\n';
	int val = bGetExpressionResult(chBuffer);
	return val;
}
