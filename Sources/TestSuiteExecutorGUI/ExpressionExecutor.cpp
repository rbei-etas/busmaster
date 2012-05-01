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
#include <stack>
#include <queue>

using namespace std;
#define defBITS_IN_BYTE         8
#define EMPTY_STRING            _T("")
#define MAX_VAL_OF_BYTE         256
#define defMAX_BYTE 8
#define LP 10
#define RP 20
#define OPERATOR 30
#define OPERAND 40


/******************************************************************************
Function Name  :  nPrecedence
Input(s)       :  CString omStr - Operator input
Output         :  int
Functionality  :  Returns the precedence of the operator
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
int CExpressionExecutor::nPrecedence(CString omStr)
{
    int nResult = 0;

    if(omStr == "*"||omStr == "/")
    {
        nResult = 9;
    }
    else if(omStr == "+"||omStr == "-")
    {
        nResult = 8;
    }
    else if(omStr == "<"||omStr == ">"||omStr == ">="||omStr == "<=")
    {
        nResult = 7;
    }
    else if(omStr == "=="||omStr == "!=")
    {
        nResult = 6;
    }
    else if(omStr == "&")
    {
        nResult = 5;
    }
    else if(omStr == "^")
    {
        nResult = 4;
    }
    else if(omStr == "|")
    {
        nResult = 3;
    }
    else if(omStr == "&&")
    {
        nResult = 2;
    }
    else if(omStr == "||")
    {
        nResult = 1;
    }

    return (nResult);
}


/******************************************************************************
Function Name  :  nGetType
Input(s)       :  char ch - AlphaNumeric Character
Output         :  int
Functionality  :  ReturnS the one of the character type (OPERATOR,LP,RP,OPERAND)
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
int CExpressionExecutor::nGetType(char ch)
{
    int nResult = 0;

    if (isdigit(ch))//if (isalpha(ch))
    {
        ch = 0;
    }

    switch (ch)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '&':
        case '|':
        case '%':
        case '<':
        case '>':
        case '=':
        case '!':
        {
            nResult = OPERATOR;
            break;
        }

        case '(':
        {
            nResult = LP;
            break;
        }

        case ')':
        {
            nResult = RP;
            break;
        }

        case 0:
        {
            nResult = OPERAND;
            break;
        }
    }

    return nResult;
}

/******************************************************************************
Function Name  :  isOperator
Input(s)       :  char ch
Output         :  BOOL
Functionality  :  Verifies the character is operator or not.
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
BOOL CExpressionExecutor::isOperator(char ch)
{
    switch(ch)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '&':
        case '|':
        case '%':
        case '<':
        case '>':
        case '=':
        case '!':
            return TRUE;

        default:
            return FALSE;
    }
}

/******************************************************************************
Function Name  :  nGetTypeOfSymbol
Input(s)       :  int index - index in Expression
                  CString omString - Expression
                  CString &NumValue - Numeric value
Output         :  int
Functionality  :  Retrive the numeric value into new string and returns
                  the new index
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
int CExpressionExecutor::nGetTypeOfSymbol(int nindex, CString omString, CString& NumValue )
{
    int index = nindex;
    char* pString = omString.GetBuffer(MAX_PATH);
    int nLength = omString.GetLength()+1-index;
    char* pRealString = new char[nLength];
    memset(pRealString, 0, nLength*sizeof(char));
    strncpy(pRealString, pString+index, (nLength));
    char* pStopString;
    double value = strtod(pRealString, &pStopString);
    index = omString.Find(pStopString, nindex);

    if(index == 0||index==nindex)
    {
        index = omString.GetLength();
    }

    NumValue.Format("%f",value);
    delete []pRealString;
    return index;
}

/******************************************************************************
Function Name  :  nGetTypeOfOperator
Input(s)       :  int index - index in Expression
                  CString omString - Expression
                  CString &NumValue - Numeric value
Output         :  int
Functionality  :  Retrives the operator into seperate string and return new index.
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
int CExpressionExecutor::nGetTypeOfOperator(int index, CString omString, CString& NumValue)
{
    char Num[3] = {0};
    int i = -1;
    char ch = omString.GetAt(index);

    while(isOperator(ch))
    {
        i++;
        Num[i%2] = ch;
        ch = omString.GetAt(++index);
    }

    NumValue.Format("%s",Num);
    return index;
}

/******************************************************************************
Function Name  :  CExpressionExecutor
Input(s)       :  -
Output         :  -
Functionality  :  Constructor of CExpressionExecutor
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
CExpressionExecutor::CExpressionExecutor()
{
}

/******************************************************************************
Function Name  :  ~CExpressionExecutor
Input(s)       :  -
Output         :  -
Functionality  :  Destructor of ~CExpressionExecutor
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
CExpressionExecutor::~CExpressionExecutor()
{
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CExpressionExecutor& RefObj - Refreance object.
Output         :  CExpressionExecutor&
Functionality  :  Copies the data from the refrance object.
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
CExpressionExecutor& CExpressionExecutor::operator=(const CExpressionExecutor& RefObj)
{
    m_omFormula = RefObj.m_omFormula;
    return *this;
}

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
Function Name  :  omGetEnggValue
Input(s)       :  CString omFormula - Expression String
Output         :  CString - Value
Functionality  :  Returns the expression value in string
Member of      :  CExpressionExecutor
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
CString CExpressionExecutor::omGetEnggValue(CString omFormula)
{
    m_omFormula = omFormula;
    CString omEnggValue = _T("");
    int nLenght = m_omFormula.GetLength();
    int i = 0;
    stack <CString> PostfixStack;
    queue <CString> PostfixExpression;

    while (i < nLenght)
    {
        char temp = m_omFormula.GetAt(i);
        CString next;
        int choice = nGetType(temp);

        switch (choice)
        {
            case LP:
            {
                CString omStrTemp;
                omStrTemp = temp;
                PostfixStack.push(omStrTemp);
                break;
            }

            case RP:
            {
                while ((next = PostfixStack.top()) != "(")
                {
                    //CString str;
                    //str = next;
                    PostfixExpression.push(next);
                    PostfixStack.pop();
                }

                PostfixStack.pop();
                break;
            }

            case OPERAND:
            {
                CString number;
                int index = nGetTypeOfSymbol(i, m_omFormula, number);
                PostfixExpression.push(number);
                i = index - 1;
                break;
            }

            case OPERATOR:
            {
                CString omStr;
                int nIndex = nGetTypeOfOperator(i, m_omFormula, omStr);

                while (!PostfixStack.empty() &&
                        (nPrecedence(omStr) <= nPrecedence(PostfixStack.top())))
                {
                    CString str = PostfixStack.top();
                    PostfixExpression.push(str);
                    PostfixStack.pop();
                }

                CString str;
                str = temp;
                PostfixStack.push(omStr);
                i = nIndex - 1;
                break;
            }
        }

        i++;
    }

    while (!PostfixStack.empty())
    {
        CString omStr = PostfixStack.top();
        PostfixExpression.push(omStr);
        PostfixStack.pop();
    }

    stack <float> s;
    float value, value1, value2;

    while (!PostfixExpression.empty())
    {
        CString token = PostfixExpression.front();
        PostfixExpression.pop();
        char ch = token.GetAt(0);

        if(isdigit(ch))
        {
            char* stopstring;
            value = (float)strtod(token.GetBuffer(MAX_PATH), &stopstring);
            s.push(value);
        }
        else
        {
            value2 = s.top();
            s.pop();
            value1 = s.top();
            s.pop();

            if(token == "+")
            {
                value = value1 + value2;
            }
            else if(token == "-")
            {
                value = value1 - value2;
            }
            else if(token == "*")
            {
                value = value1 * value2;
            }
            else if(token == "/")
            {
                value = value1 / value2;
            }
            else if(token == "%")
            {
                value = (float)((int)value1 % (int)value2);
            }
            else if(token == "&")
            {
                value = (float)((int)value1 & (int)value2);
            }
            else if(token == "<")
            {
                value = value1 < value2;
            }
            else if(token == ">")
            {
                value = value1 > (int)value2;
            }
            else if(token == "==")
            {
                value = value1 == value2;
            }
            else if(token == ">=")
            {
                value = value1 >= value2;
            }
            else if(token == "<=")
            {
                value = value1 <= value2;
            }
            else if(token == "||")
            {
                value = (int)value1 || (int)value2;
            }
            else if(token == "&&")
            {
                value = (int)value1 && (int)value2;
            }
            else if(token == "!=")
            {
                value = (value1 != value2);
            }

            s.push(value);
        }
    }

    value = s.top();
    s.pop();
    omEnggValue.Format("%lf", value);
    return omEnggValue;
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
    CString omStrValue = omGetEnggValue(omFormula);
    DWORD dwResultValue = (DWORD)_atoi64(omStrValue.GetBuffer(MAX_PATH));
    return (dwResultValue > 0);
}
