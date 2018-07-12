/*******************************************************************************
  Project       :  Language Converter
  FileName      :  Functions.hpp
  Description   :  Implementation file for parsing functions
  $Log:   X:/Archive/Sources/FormatConverter/LexerHandlers.hpv  $
   
      Rev 1.6   14 Mar 2012 17:37:58   CANMNTTM
   CAPL_TO_CPP Conversion is updated with
   New Design.
   
      Rev 1.1   06 Jan 2012 12:02:08   CANMNTTM
   DBC2DBF Converter Library Name is changed.
   
      Rev 1.0   05 Jan 2012 19:07:40   CANMNTTM
    
   
      Rev 1.0   17 Aug 2005 13:46:12   amb2kor
   Initial Version.

  Author        :  Amit Ranjan
  Date Created  :  21.04.2004
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/
#pragma once
#include "CAPL2CConverter_stdafx.h"
#ifndef _LEXER_H
#define _LEXER_H
#define INVALID 0xffffffff
//#include "capl_c_lexer.h"
#endif
#include "ConstString.h"
//#include "SelectDatabaseDlg.h"
#include "EnvVarHandlerDlg.h"
#include "LexerHandlers.h"
#undef fprintf

extern char yytext[];
extern FILE *yyout;
static int yyinput();
extern int yyleng;
#define ECHO_INPUT (void) fwrite( yytext, yyleng, 1, yyout )

/**
* \brief         Validates the given number is a valid alpha and numeric character
* \param[in]     char - Input Character
* \param[out]    -
* \return        TRUE - if ch is a Alpha Numeric character otherwise False
* \authors       Venkatanarayana Makam
* \date          07.02.2012 Created
*/
BOOL IsValidAlpaNum(char ch)
{
    int id = ch;
    if ( (id>=48&&id<=57) || (id >=65 && id<=90) || (id >=97 &&id <=122))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/**
* \brief         Reads The Function call from the current position of 
                 input file using lexer input function yyinput
* \param[in]     CString omStrFunction Function Call in the form of <FunctionCall(>
* \param[out]    CString omStrFunction provides the total function call
* \return        Void
* \authors       Venkatanarayana Makam
* \date          07.02.2012 Created
*/
void vReadFunctionCall(CString& omStrFunction)
{
	omStrFunction = yytext;
    RemoveComments(omStrFunction);
	omStrFunction.TrimLeft(" \n\t");
	omStrFunction.TrimRight(" \n\t");
    
    CString omStrTemp = omStrFunction;
    omStrTemp.TrimRight(" {\n\t");
    char chCurrent = ' '; 
    char chPrevious = ' ';
    BOOL bComment= FALSE;
    BOOL bString = FALSE;
    INT nCloseBracesReq = 1;
    INT nChar;
    while(1)
    {
        chCurrent = yyinput();
        omStrFunction += chCurrent;
        if( '"' == chCurrent )
        {
            if( bComment == FALSE )
            {
                if( '\\' != chPrevious )
                {
                    bString = !bString;
                }
            }
        }
        else if ( '*' == chCurrent )
        {
            if ( bString == FALSE )
            {
                if ( '\\' == chPrevious )
                {
                    bComment = TRUE;
                }
            }
        }
        else if ( '/' == chCurrent )
        {
            if ( bString == FALSE )
            {
                if ( '*' == chPrevious )
                {
                    bComment = FALSE;
                }
            }
        }
        else if ( '(' == chCurrent )
        {
            if ( bString == FALSE && bComment == FALSE )
            {
                nCloseBracesReq++;
            }
        }
        else if ( ')' == chCurrent )
        {
            if ( bString == FALSE && bComment == FALSE )
            {
                nCloseBracesReq--;
                if (nCloseBracesReq == 0)
                {
                    //MessageBox(0, omStrFunction, "FunCtion Call", MB_OK);
                    break;
                }
            }
        }
        chPrevious = chCurrent;
    }
	return;
}

/**
* \brief         Stores the CAPL mstimer variables in the Global variables.These
                 Variables are used while calling SetTimer CAPL function
                 input file using lexer input function yyinput
* \param[in]     None Takes input from yytext
* \return        Void
* \authors       Venkatanarayana Makam
* \date          
*/
void vStoreTimers()
{
	CString omStrTimers;
	omStrTimers = yytext;
	if( RemoveComments(omStrTimers) == TRUE)
    {
	    omStrTimers.TrimLeft(" \t\n");
		omStrTimers.TrimRight(" \t\n");
	    int nStart = omStrTimers.Find(" ");
        if(nStart < 0)
        {
            nStart = omStrTimers.Find("\t");
        }
	    CString omStrTimerType;
	    CString omStrTemp;
	    if(nStart >= 0)
	    {
		    omStrTimerType = omStrTimers.Left(nStart);
		    omStrTimerType.TrimLeft(" \t\n");
			omStrTimerType.TrimRight(" \t\n");
		    omStrTimerType.MakeLower();
    		
		    omStrTimers = omStrTimers.Right(omStrTimers.GetLength() - nStart);
		    nStart = 0;						
			//vTokenize(omStrTimers, ",;", omStrTemp, nStart);
			
			char *chTemp = new char[omStrTimers.GetLength()+1];
			if (NULL != chTemp)
			{
				strcpy(chTemp, (LPCSTR)omStrTimers);
				char* token = strtok( chTemp, ",;" );
				while( token != NULL )
				{
					omStrTemp = token;
					omStrTemp.TrimLeft(" \t\n");
					omStrTemp.TrimRight(" \t\n");
					if (omStrTimerType == "mstimer")
					{
						g_ouGlobalVariables.g_omStrMsTimers.Add(omStrTemp);
					}
					else if (omStrTimerType == "timer")
					{
						g_ouGlobalVariables.g_omStrSecTimers.Add(omStrTemp);
					}
					token = strtok( NULL, ",;" );
					//omStrTemp.Empty();
					//vTokenize(omStrTimers, ",;", omStrTemp, nStart);
				}
			}
			if (NULL != chTemp)
			{
				delete[] chTemp;
				chTemp = NULL;
			}
	    }
    }
    else
    {
		ECHO_INPUT;
    }

}

/**
* \brief         This function finds the next token in the target string
				 
* \param[in]     None Takes input from yytext
* \return        Void
* \authors       Saravanan K S
* \date          
*/
void vTokenize(CString strInput, CString strToken, CString& strOutput, int& nStart)
{	
	int nIdx = strInput.Find(strToken, nStart);
	int nCount = 0;
	strOutput.Empty();

	if (nIdx != -1)
	{
		nCount = nIdx - nStart;	// Calculate the number of characters to extract
		strOutput = strInput.Mid(nStart, nCount); //Extract the charatcers
	}
	else if( nStart < strInput.GetLength() )
	{
		strOutput = strInput.Right(strInput.GetLength() - nStart);
		nIdx = strInput.GetLength()-1;
	}
	else
	{
		strOutput.Empty();
	}
	nStart = nIdx + 1;
}

/**
* \brief         This will extract the name of the key from yytext. If key is
                 supported by BUSMASTER it will be stored in" acSptdKey " else 
                 it will be stored in " acUnSptdKey ".
* \param[in]     None Takes input from yytext
* \return        Void
* \authors       Venkatanarayana Makam
* \date          
*/
void vKeyStoreHeader()
{
    CString omStrName = yytext;
    RemoveComments(omStrName);
    
    CString omStrTemp = omStrName;
    omStrTemp.MakeLower();
    INT nIndex;
    if( (nIndex = omStrTemp.Find("on key")) >= 0 )
    {
        omStrTemp = omStrName;
        omStrTemp.Delete(nIndex, strlen("on key"));
        omStrTemp.TrimLeft(" \n\t");
        omStrTemp.TrimRight(" \n\t{");
    }

    try
    {
    char acKeyName[defSTR_MaxSizeBreadth];
    int m = 8; //length of /*@@key:
    int n = 0;
    
    if( omStrTemp[0] == '\'' )
    {
        // Single char  
        int id = omStrTemp[1];
        if( IsValidAlpaNum( omStrTemp[1] ) == FALSE)//if not alpha numeric
        {
            char cTemp[2]; 
            cTemp[0] = omStrTemp[1];
            cTemp[1] = 0;
            ouUnSptdKey.bAdd(cTemp);
        }
        else
        { //if alpha numeric
            acSptdKey[cIndex2++] = omStrTemp[1];
        }
    }
    else
    {
        n = 0;
        m = 0;
        if( omStrTemp[m] != '*')
        {
            while( omStrTemp[m] != NULL )
            {
                acKeyName[n++] = omStrTemp[m++];
            }
            acKeyName[n] = 0;
            ouUnSptdKey.bAdd(acKeyName);
            
        }
    }
    }

    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vKeyStoreHeader\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}
/**
* \brief         The Function will Process the CAPL Event "on key".The unsupported
                 keys are replaced with the user selected alternative keys
* \param[in]     None Takes input from yytext
* \return        Void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleOnkey(CString& omStrKeyVal)
{
    CString omStrText;
    CString omStrFunctionName;
    g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_KEY;         //to be used to convert "this"
	CString omTemp="";
	omTemp = defSTR_Header;
	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());
    fprintf(yyout,omTemp);
    if( omStrKeyVal[0] == '*' )
    {//for Key_All
        omStrText = defSTR_KeyAllHeader;
        fprintf(yyout,omStrText);
        fprintf(yyout,defSTR_KeyAllStart);
        fprintf(yyout, "UINT KeyValue = GetCaplKeyValue(chBMKeyValue);");
        omStrFunctionName = defSTR_KeyAllStart;
        omStrFunctionName.Replace(defSTR_FunctionDefinition, ";");
    }
    else
    {//check for unsupported kay
        if( omStrKeyVal[0] =='\'')
        {
            omStrKeyVal = omStrKeyVal.Right( omStrKeyVal.GetLength() - 1 );
            omStrKeyVal = omStrKeyVal.Left( omStrKeyVal.GetLength() - 1);
        }
        int index = ouUnSptdKey.nFind( omStrKeyVal );
        if( index == -1)
        {//means supported key
            omStrText = defSTR_KeyCharHeader;// + HandlerVal + " */\n";
            omStrText += omStrKeyVal + defSTR_HeaderFormat;
            fprintf(yyout,omStrText);
            omStrFunctionName.Format(defSTR_KeyCharStart,omStrKeyVal.GetAt(0));
            fprintf(yyout,"%s",omStrFunctionName.GetBuffer(MAX_PATH));
            omStrFunctionName.Replace(defSTR_FunctionDefinition, ";");
        }
        else
        {//means unsupported key
            omStrText = defSTR_KeyCharHeader;
            if(  acAltKey[index] == -1)
            { // if no alternate key is available
                //array is initialised with -1
                omStrText += omStrKeyVal + defSTR_HeaderFormat;
                fprintf(yyout,omStrText);
                fprintf(yyout,defSTR_KeyCharStart, omStrKeyVal.GetAt(0));

                sprintf(g_chErrorString,defSTR_Warning7,omStrKeyVal);
                g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);

                nUnCnvrtdLine = nUnCnvrtdLine + 2;//wighted 2

            }
            else
            {
                omStrText += CString( acAltKey[index])+defSTR_HeaderFormat;
                fprintf(yyout,omStrText);

                omStrFunctionName.Format(defSTR_KeyCharStartAlt, omStrKeyVal, acAltKey[index]);
                fprintf(yyout,"%s",omStrFunctionName.GetBuffer(MAX_PATH));
                omStrFunctionName.Replace(defSTR_FunctionDefinition, ";");

                sprintf(g_chErrorString,defSTR_ConvertedkeyInfo,omStrKeyVal, acAltKey[index]);
                g_ouGlobalVariables.g_ouWarningStrings.insert(g_chErrorString);
            }
        }
    }
    g_ouGlobalVariables.omStrLastEvent = omStrText;
    omStrFunctionName = defSTF_Extern + omStrFunctionName + ";";
    g_ouGlobalVariables.g_omStrFunCalls.Add(omStrFunctionName);
    //}//end of keyhandler
}

/**
* \brief         Returns The CAPL Equavalent BUSMASTER Strorage Type
* \param[in]     CString& omStrCAPLType CAPL Storage Type
* \param[out]    CString& omStrBMType CAPL Equivalent BM Storage Type
* \return        S_OK if the Suitable Type is found otherwise S_FALSE
* \authors       Venkatanarayana Makam
* \date          
*/
int GetBusmasterStorageType(CString& omStrCAPLType, CString& omStrBMType)
{
    int nRetVal = S_OK;
    if(omStrCAPLType.CompareNoCase("int") == 0)
    {
        omStrBMType = "SHORT";
    }
    else if(omStrCAPLType.CompareNoCase("word") == 0)
    {
        omStrBMType = "USHORT";
    }
    else if(omStrCAPLType.CompareNoCase("dword") == 0)
    {
        omStrBMType = "ULONG";
    }
	else if(omStrCAPLType.CompareNoCase("qword") == 0)
    {
        omStrBMType = "ULONGLONG";
    }
	else if(omStrCAPLType.CompareNoCase("int64") == 0)
    {
        omStrBMType = "LONGLONG";
    }
    else if(omStrCAPLType.CompareNoCase("long") == 0)
    {
        omStrBMType = "LONG";
    }
    else if(omStrCAPLType.CompareNoCase("byte") == 0)
    {
        omStrBMType = "UCHAR";
    }
    else if(omStrCAPLType.CompareNoCase("message") == 0)
    {
        omStrBMType = "STCAN_MSG";
    }
    else if(omStrCAPLType.CompareNoCase("message*") == 0)
    {
        omStrBMType = "STCAN_MSG";
    }
    else if(omStrCAPLType.CompareNoCase("float") == 0)
    {
        omStrBMType = "FLOAT";
    }
    else if(omStrCAPLType.CompareNoCase("DOUBLE") == 0)
    {
        omStrBMType = "FLOAT";
    }
    else if((omStrCAPLType.CompareNoCase("msTimer") == 0) ||
            omStrCAPLType.CompareNoCase("Timer") == 0)
    {
        omStrBMType = "CHAR*";
    }
    else if((omStrCAPLType.CompareNoCase("char") == 0))
    {
        omStrBMType = "CHAR";
    }
    //msTimer"|"timer
    else
    {
        omStrBMType = "";
        nRetVal = S_FALSE;
    }
    return nRetVal;
}
/**
* \brief         This Function will be called by the lexer at the start and end
                 of the "variables" section of a CAPL File This function writes 
                 BM Global Variable header comments
* \param[in]     BOOL bStart, TRUE if the variable section is started, FALSE if 
                 the section is over
* \return        int
* \authors       Venkatanarayana Makam
* \date          
*/
int onGlobalVariablesHeader(BOOL bStart)
{
    //if( TRUE == bStart)
    {
		g_bGlobalVariable = true;
		CString omTemp = defSTR_GlobalVarHeader;
		omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());
		omTemp += defSTR_GlobalVarNameSpace;
		omTemp+="{";
		fprintf(yyout,omTemp);
        g_ouGlobalVariables.omStrLastEvent = defSTR_GlobalVar;
     //   yy_push_state(VariableParse);
    }
    //else
    //{
    //    fprintf(yyout,defSTR_GlobalVarEnd);
    //    yy_pop_state();
    //}
    return S_OK;
}

/**
* \brief         Converts the CAPL variable declaration to BM Declaration type
* \param[in]     CString& omStrcaplVariable CAPL Storage type.
* \return        int
* \authors       Venkatanarayana Makam
* \date          
*/
int ConvertToBMVariableDec(CString& omStrcaplVariable, bool bGlobal)
{
    RemoveComments(omStrcaplVariable);
    omStrcaplVariable.TrimLeft(" \n\t");
    omStrcaplVariable.Replace('\t', ' ');
	omStrcaplVariable.Replace('\n', ' ');
    int nIndex = omStrcaplVariable.Find(" ", 0);
    int nRetVal = S_OK;

    if ( nIndex >=0 )
    {
        CString omStrBMType;
        CString omStrTemp = omStrcaplVariable.Left(nIndex);
        omStrTemp.TrimRight(" \n\t");
        if( omStrTemp.CompareNoCase("message") == 0)
        {
			
            MsgVariables ouMsgVariable;
            vMsgDecl(omStrcaplVariable, ouMsgVariable, bGlobal);
            if(bGlobal == true)
            {
                g_ouGlobalVariables.g_ouGlobalMsgVariables.Add(ouMsgVariable);
            }
            else
            {
                g_ouGlobalVariables.g_ouLocalMsgVariables.Add(ouMsgVariable);
            }
            return S_FALSE;
        }
        else if( ( omStrTemp.CompareNoCase("msTimer") == 0)|| ( omStrTemp.CompareNoCase("Timer") == 0) )
        {
			vHandleTimerDeclaration(omStrcaplVariable);
        }
		
        else if(GetBusmasterStorageType(omStrTemp, omStrBMType) == S_OK)
        {
            omStrcaplVariable.Delete(0, nIndex);
            omStrcaplVariable.Insert(0, omStrBMType);
        }
        else
        {
            omStrcaplVariable = "INT " + omStrcaplVariable;
        }
    }
	else
	{
		omStrcaplVariable = "INT ";
	}
    return S_OK;
}

int onCaplVaribleDecl(bool bGlobal)
{
	CString omStrValues = yytext;
	if (S_OK == ConvertToBMVariableDec(omStrValues, g_bGlobalVariable))
    {
        fprintf(yyout, "\n%s\n", omStrValues.GetBuffer(MAX_PATH));
        return S_OK;
    }
    return S_FALSE;
}
int onConstVaribleDecl(bool bGlobal)
{
	CString omStrValue = yytext;
    CString omStrTemp;
    RemoveComments(omStrValue);
	omStrValue.TrimLeft(" \t\n");
	INT nIndex = omStrValue.Find(" ");
    if(nIndex >= 0)
    {
        omStrTemp = omStrValue.Left(nIndex);
        omStrTemp.TrimLeft(" \t\n");
		omStrTemp.TrimRight(" \t\n");
        if ( omStrTemp.CompareNoCase("const") == 0 )
        {
            omStrTemp = omStrValue.Right(omStrValue.GetLength()-nIndex);
            if( S_OK == ConvertToBMVariableDec(omStrTemp, bGlobal) )
            {
                fprintf(yyout, "\nconst static %s", omStrTemp.GetBuffer(MAX_PATH));
                return S_OK;
            }
        }
    }
    return 0;
}
int onVaribleDecl(bool bGlobal )
{
	CString omStrcaplVariable = yytext;
	RemoveComments(omStrcaplVariable);
    omStrcaplVariable.TrimLeft(" \n\t");
    omStrcaplVariable.Replace('\t', ' ');
    int nIndex = omStrcaplVariable.Find(" ", 0);
    int nRetVal = S_OK;
    if ( nIndex >=0 )
    {
        CString omStrBMType;
        CString omStrTemp = omStrcaplVariable.Left(nIndex);
        omStrTemp.TrimRight(" \n\t");
        if(GetBusmasterStorageType(omStrTemp, omStrBMType) == S_OK)
        {
            omStrcaplVariable.Delete(0, nIndex);
            omStrcaplVariable.Insert(0, omStrBMType);
        }
        else
        {
            omStrcaplVariable = "INT " + omStrcaplVariable;
        }
		fprintf(yyout, "\nstatic %s", omStrcaplVariable.GetBuffer(0));
    }
    return S_OK;
}
/**
* \brief         Called by the lexer when it encounters global variable 
                 declarations
* \param[in]     None
* \return        int
* \authors       Venkatanarayana Makam
* \date          
*/
int onGlobalVariables()
{
    CString omStrValues = yytext;
	
    if( S_OK == ConvertToBMVariableDec(omStrValues, true) )
    {
        fprintf(yyout, "%s\n", omStrValues.GetBuffer(MAX_PATH));
        return S_OK;
    }
    return S_FALSE;
}
/**
* \brief         Called by the lexer when it encounters loval variable 
                 declarations
* \param[in]     None
* \return        int
* \authors       Venkatanarayana Makam
* \date          
*/
int onLocalVariables()
{
	HRESULT hResult = S_FALSE;
    CString omStrValues = yytext;
    if( S_OK == ConvertToBMVariableDec(omStrValues, false) )
    {
        INT nLen = omStrValues.GetLength();
        fprintf(yyout, "static %s\n", omStrValues.GetBuffer(MAX_PATH));

        hResult = S_OK;
    }
	return hResult;
}

/**
* \brief         Called by the lexer when it encounters a Function definition
* \param[in]     None
* \return        int
* \authors       Venkatanarayana Makam
* \date          
*/
void vFuNdef()
{
    CString omStrFun = yytext;
    RemoveComments(omStrFun);
    FunctionPrototype ouFunType;
    if ( VALID_FUNCTION == RetriveFunctionDetails(omStrFun, ouFunType, TYPE_FUNDEF))
    {
        g_ouGlobalVariables.g_ouFunctionPrototypeList.AddTail(ouFunType);
    }
    else
    {
        //ECHO_INPUT;
    }
}

/**
* \brief         Called by the lexer when it encounters a CAPL Function
* \param[in]     None
* \return        int
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleCaplSupportedFunction()
{
    CString omStrTemp = yytext;
    CString omStrFunction = omStrTemp;
    RemoveComments(omStrFunction);
    omStrTemp.TrimLeft(" \t\n(");
	omStrTemp.TrimRight(" \t\n(");
    omStrFunction.TrimRight(" \t\n(");

    
    //Handle some Specific Type of Functions
    if( omStrTemp.CompareNoCase("write") == 0 )
    {
        fprintf(yyout, "\nTrace (");
    }
    else if ( omStrTemp.CompareNoCase("canceltimer") == 0 )
    {
        fprintf(yyout, "\nStopTimer (");
    }
	else
    {
		if(omStrTemp.CompareNoCase("timenowns") == 0||omStrTemp.CompareNoCase("timenowfloat") == 0||omStrTemp.CompareNoCase("timenowint64") == 0)
		{
			omStrFunction = "timenow";
			CString strError = CString(defSTR_UnSupportedFunction)+CString(" , but converted to timenow");
		
			sprintf(g_chErrorString,strError, (LPCSTR)omStrTemp);
			g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);
		
		}
        omStrFunction.MakeLower();
        omStrFunction += defSTR_CAPLWrapperToken;
        omStrFunction += " ( ";
		if(omStrTemp.CompareNoCase("messagetimens")==0)
		{
			bTimeStampNs=true;
		}
        fprintf(yyout, "%s", omStrFunction.GetBuffer(MAX_PATH));
    }
}
/**
* \brief         Called by the lexer when it encounters a Function Call
* \param[in]     None
* \return        int
* \authors       Venkatanarayana Makam
* \date          
*/
VOID vHandleFunctionCall()
{
    CString omFunCall;
    int start;
    omFunCall = yytext ;
    RemoveComments(omFunCall);
    start = omFunCall.Find("(",0);
    CString omFunName = omFunCall.Left(start);
    omFunName.TrimLeft(" \n\t");
	omFunName.TrimRight(" \n\t");

    FunctionPrototype ouFunType;
    CString omStrNoCase = omFunName;
    omStrNoCase.MakeLower();
	if ( omFunName.CompareNoCase("SetTimer") == 0 )
	{
		vHandleSetTimerCall();
	}
	else if (omFunName.CompareNoCase("output") == 0 && g_ouGlobalVariables.m_eBus != FLEXRAY)
	{
		fprintf(yyout, "\noutput_CAPL(&");
	}
    else if ( omFunName.CompareNoCase("resetCan") == 0 )
	{
		fprintf(yyout, "\nResetController(1");
	}
     else if ( omFunName.CompareNoCase("elCount") == 0 )
	{
		fprintf(yyout, "\nsizeof(");
	}
    else if (GetUtilityFunctionPrototype(omFunName, ouFunType) == S_OK)
    {
        CStringArray omFunctionArray;
		INT_PTR nArgCount = ouFunType.m_ouArgList.GetSize();
        CString omStrBMFunction;
        if(ouFunType.m_bMsgArgPresent == TRUE )
		{
		    CString omStrFunction;
			vReadFunctionCall(omStrFunction);
            nFormBMFunctionCall(omStrFunction, omStrBMFunction);
		    fprintf(yyout, "\n %s", omStrBMFunction.GetBuffer(MAX_PATH));     
        }
        else
        {
            omStrBMFunction = defSTR_Utils; 
            omStrBMFunction += omFunName + "(";
            fprintf(yyout, "\n %s", omStrBMFunction.GetBuffer(MAX_PATH));
        }
    }
    
    
    else if(omStrNoCase == "if" || omStrNoCase == "switch" || omStrNoCase =="do"
                                        || omStrNoCase == "while"||omStrNoCase == "for"
										|| omStrNoCase == "return"|| omStrNoCase == "case" || omStrNoCase=="frframe" )
    {
        //fprintf(yyout, " %s(", omStrNoCase.GetBuffer(MAX_PATH));;
		if(omStrNoCase=="frframe")
		{
			sprintf(g_chErrorString,defSTR_UnSupportedVariableFormat, (LPCSTR)omFunName);
			g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);
		}
		ECHO_INPUT;
        return;
    }

    /*else if(omFunName == "if" || omFunName == "switch" || omFunName =="do"|| 
                omFunName == "while"|| omFunName == "for"||omFunName == "return")
    {
        ECHO_INPUT;
        return;
    }*/
    //IN Future we can support it from a external file;
    else if (isCaplSupportedFunction(omFunName) == S_OK)
    {
		ECHO_INPUT;
    }
    else
    {
        sprintf(g_chErrorString,defSTR_UnSupportedFunction, (LPCSTR)omFunName);
        g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);
		ECHO_INPUT;
    }
    return;
}

/**
* \brief         Created a Busmaster Function call by Typecasting Message 
                 Argument with STCAN_MSG
* \param[in]     CString& omStrFunction - capl funcation call
* \param[out]     CString& omStrBMFunction - BM Function call
* \return        int
* \authors       Venkatanarayana Makam
* \date          
*/
INT nFormBMFunctionCall(CString& omStrFunction, CString& omStrBMFunction)
{
    FunctionPrototype ouFunDetails;
    RetriveFunctionDetails(omStrFunction, ouFunDetails, TYPE_FUNCALL);
	FunctionPrototype ouFunType;
    GetUtilityFunctionPrototype(ouFunDetails.m_omStrFunName, ouFunType);
    omStrBMFunction = defSTR_Utils;
    omStrBMFunction += ouFunDetails.m_omStrFunName + "(";
	for(int i = 0 ; i < ouFunDetails.m_omStrFunNameArray.GetSize(); i++)
	{
		//omStrBMFunction = ouFunDetails.m_omStrFunName + " ( ";
		if(ouFunType.m_ouArgList.GetAt(i) == CAPL_MESSAGE)
		{
			CString omStrTemp;
            if( ouFunDetails.m_omStrFunNameArray.GetAt(i).CompareNoCase("this") == 0)
            {
                omStrTemp.Format(defSTR_StcanTypecase, defSTR_RxMsg);
            }
            else
            {
			    omStrTemp.Format(defSTR_StcanTypecase, ouFunDetails.m_omStrFunNameArray.GetAt(i));
            }
            omStrBMFunction += omStrTemp + ",";
		}
		else
		{
            CString omStrArg;
            CString omStrRetArg;
            if ( HandleArgument(ouFunDetails, i, omStrRetArg) == TRUE)
            {
                CString omStrRetVal;
                nFormBMFunctionCall(omStrRetArg, omStrRetVal);
                omStrBMFunction += omStrRetVal+ ",";
            }
            else
            {
                omStrBMFunction += ouFunDetails.m_omStrFunNameArray.GetAt(i) + ",";
            }
			
		}
	}
	omStrBMFunction.TrimRight(",");
	omStrBMFunction += ")";
    return 0;
}

/**
* \brief         Handles the Function call argument and if the argument 
* \param[in]     FunctionPrototype& ouFunDetails - Function Deatails
* \param[in]     INT &nArgIndex                  - Argument index in ouFunDetails
* \param[out]    CString& omStrFunction          - Function Call   
* \return        TRUE if the argument is function call
* \authors       Venkatanarayana Makam
* \date          
*/
BOOL HandleArgument(FunctionPrototype& ouFunDetails, INT &nArgIndex, CString& omStrFunction )
{
    CString omStrArg = ouFunDetails.m_omStrFunNameArray.GetAt(nArgIndex);
    if(isFunction(omStrArg) == TRUE)
    {
        FunctionPrototype ouRecFunDetails;
        int nIndex = omStrArg.Find("(", 1);
        if(nIndex >= 1)
        {
            CString omStrFunName = omStrArg.Left(nIndex);
            if( GetUtilityFunctionPrototype(omStrFunName, ouRecFunDetails) == S_OK )
            {
                INT nArgCount = ouRecFunDetails.m_ouArgList.GetSize();
                //omStrFunction += omStrFunName + "(";
                for(INT i = 0; i< nArgCount; i++)
                {
                    nArgIndex += i;
                    omStrFunction += ouFunDetails.m_omStrFunNameArray.GetAt(nArgIndex) + ",";
                }
                omStrFunction.TrimRight(",");
            }
            else
            {
                return FALSE;
            }
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/**
* \brief         Checks whether the argument is valid Function call or not
* \param[in]     CString omStrArg - Input Text
* \return        TRUE if the argument is function call
* \authors       Venkatanarayana Makam
* \date          
*/
BOOL isFunction(CString omStrArg)
{
    omStrArg.Remove(' ');
    BOOL bRetVal = FALSE;
    if(omStrArg[0] != '"')
    {
        int nIndex = omStrArg.Find("(", 1);
    
        if(nIndex >= 1)
        {
            if((IsValidAlpaNum(omStrArg[nIndex-1]) == TRUE) || (omStrArg[nIndex-1] == '_'))
            {
                bRetVal = TRUE;
            }
        }
    }
    return bRetVal;
}

/**
* \brief         Handles the SetTimer call of CAPL Function from yytext
* \param[in]     NONE
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleSetTimerCall()
{
	char ch;
	CString omStrTimerName;
	while ( (ch = yyinput()) != ',')
	{
		omStrTimerName += ch; 
	}
	omStrTimerName.TrimLeft(" \n");
	omStrTimerName.TrimRight(" \n");
	RemoveComments(omStrTimerName);
	INT_PTR nCount = g_ouGlobalVariables.g_omStrMsTimers.GetSize();
	BOOL bTimerFound = FALSE;
	for(int i =0 ; i< nCount; i++)
	{
		if ( omStrTimerName == g_ouGlobalVariables.g_omStrMsTimers.GetAt(i) )
		{
			bTimerFound = TRUE;
			break;
		}
	}
	if( bTimerFound == TRUE )
	{
		fprintf(yyout, "\n\tsettimer_CAPL(MSEC_TIMER, %s, ", (LPCSTR)omStrTimerName);
	}
	else
	{
		fprintf(yyout, "\n\tsettimer_CAPL(SEC_TIMER, %s, ", (LPCSTR)omStrTimerName);
	}
}

/**
* \brief         Provides the CAPL utility function prototype
* \param[in]     CString& omFunName - Function Name
* \param[out]    FunctionPrototype& ouFunType - Function Prototype
* \return        S_OK if the omFunName is a valid utility function
                 S_FALSE otherwise
* \authors       Venkatanarayana Makam
* \date          
*/
int GetUtilityFunctionPrototype(CString& omFunName, FunctionPrototype& ouFunType)
{
	INT_PTR nCount  = g_ouGlobalVariables.g_ouFunctionPrototypeList.GetCount();
    
    for ( INT_PTR i = 0 ; i< nCount; i++)
    {
        POSITION pos = g_ouGlobalVariables.g_ouFunctionPrototypeList.FindIndex(i);
        ouFunType = g_ouGlobalVariables.g_ouFunctionPrototypeList.GetAt(pos);
        if( ouFunType.m_omStrFunName.CompareNoCase(omFunName) == 0 )
        {
            return S_OK;
        }
    }
    return S_FALSE;
}

/**
* \brief         Checks whether the function is BM supported CAPL Name
* \param[in]     CString& omFunName - Function Name
* \return        S_OK if the omFunName is BM supported CAPL Name
                 S_FALSE otherwise
* \authors       Venkatanarayana Makam
* \date          
*/
int isCaplSupportedFunction(CString& omFunName)
{
    if(omFunName == "atol"||omFunName == "strncpy")
    {
        return S_OK;
    }
    return S_FALSE;
}

/**
* \brief         Return the string name for the storaget type
* \param[in]     caplArgType ouArgType - Argument type
* \param[out]    CString& omStrArgName - Argument name
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void GetNameFromArgType(caplArgType ouArgType, CString& omStrArgName)
{
    switch(ouArgType)
    {
        case CAPL_CHAR:
            omStrArgName = "CHAR";
            break;
        case CAPL_VOID:
            omStrArgName = "VOID";
            break;
        case CAPL_INT:
            omStrArgName = "SHORT";
            break;
        case CAPL_WORD:
            omStrArgName = "USHORT";
            break;
        case CAPL_DWORD:
            omStrArgName = "ULONG";
            break;
        case CAPL_LONG:
            omStrArgName = "LONG";
            break;
		case CAPL_QWORD:
			omStrArgName = "ULONGLONG";
			break;
		case CAPL_INT64:
			omStrArgName = "LONGLONG";
			break;
        case CAPL_FLOAT:
            omStrArgName = "FLOAT";
            break;
        case CAPL_DOUBLE:
            omStrArgName = "DOUBLE";
            break;
        case CAPL_MESSAGE:
            omStrArgName = "STCAN_MSG";
            break;
        case CAPL_BYTE:
            omStrArgName = "UCHAR";
            break;
		case CAPL_PFRPDU:
			omStrArgName = "BMPdu&";
			break;
        default:
            omStrArgName = "INT";
            break;
    }
}
/**
* \brief         Return the capl argument type from the string
* \param[in]     CString& omStrName - Argument name
* \return        caplArgType
* \authors       Venkatanarayana Makam
* \date          
*/
caplArgType GetArgTypeFromName(const CString& omStrName)
{
    caplArgType retArgType = CAPL_VOID;
    if(omStrName.CompareNoCase("CHAR") == 0)
    {
        retArgType = CAPL_CHAR;
    }
    if(omStrName.CompareNoCase("INT") == 0)
    {
        retArgType = CAPL_INT;
    }
    else if(omStrName.CompareNoCase("WORD") == 0)
    {
        retArgType = CAPL_WORD;
    }
    else if(omStrName.CompareNoCase("DWORD") == 0)
    {
        retArgType = CAPL_DWORD;
    }
    else if(omStrName.CompareNoCase("LONG") == 0)
    {
        retArgType = CAPL_LONG;
    }
	else if(omStrName.CompareNoCase("QWORD")==0)
	{
		retArgType = CAPL_QWORD;
	}
	else if(omStrName.CompareNoCase("INT64")==0)
	{
		retArgType = CAPL_INT64;
	}
    else if(omStrName.CompareNoCase("FLOAT") == 0)
    {
        retArgType = CAPL_FLOAT;
    }
    else if(omStrName.CompareNoCase("DOUBLE") == 0)
    {
        retArgType = CAPL_DOUBLE;
    }
    else if(omStrName.CompareNoCase("MESSAGE") == 0)
    {
        retArgType = CAPL_MESSAGE;
    }
    
    else if(omStrName.CompareNoCase("BYTE") == 0)
    {
        retArgType = CAPL_BYTE;
    }
    else if(omStrName.CompareNoCase("VOID") == 0)
    {
        retArgType = CAPL_VOID;
    }
	else if(omStrName.CompareNoCase("FRPDU") == 0)
	{
		retArgType = CAPL_PFRPDU;
	}
    /*else   
    {
        retArgType = ((bIsPointer == TRUE) ? CAPL_PINT : CAPL_INT);
    }*/
    return retArgType;
}

/**
* \brief         Retrive the function call or function definition details
* \param[in]     CString omStrFunction - Function call or Function Definition
* \param[in]     eCALLTYPE eType - Specifies Function call or Function Definition
* \param[out]    FunctionPrototype& ouFunType - Function Details
* \return        caplArgType
* \authors       Venkatanarayana Makam
* \date          
*/
INT RetriveFunctionDetails(CString omStrFunction, FunctionPrototype& ouFunType, eCALLTYPE eType)
{
    INT bRet = VALID_FUNCTION;
    omStrFunction.TrimLeft(" \n\t");
    omStrFunction.TrimRight(" \n\t{");
    omStrFunction.Replace("\n", "");
	RemoveComments(omStrFunction);
	omStrFunction.Replace("*","");
    //Find Return Type & Name
    INT nIndex = omStrFunction.Find("(", 0);
    if( nIndex >= 0)
    {
        CString omFunTypeName = omStrFunction.Left(nIndex); 
        omFunTypeName.TrimRight(" \n\t");
        nIndex = omFunTypeName.Find(" ", 0); 
        if( nIndex >= 0)
        {
            CString omStrFunType = omFunTypeName.Left(nIndex);
            omFunTypeName.Delete(0, omStrFunType.GetLength());
            omStrFunType.TrimLeft(" \n\t");
            omStrFunType.TrimRight(" \n\t");
            omFunTypeName.TrimLeft(" \n\t");
            omFunTypeName.TrimRight(" \n\t");
            ouFunType.m_omStrFunName = omFunTypeName;
            ouFunType.m_ouRetType = GetArgTypeFromName(omStrFunType);
        }
        else
        {
            omFunTypeName.TrimLeft(" \n\t");
            omFunTypeName.TrimRight(" \n\t");
            ouFunType.m_omStrFunName = omFunTypeName;
            ouFunType.m_ouRetType = CAPL_VOID;
        }
    
    }

    CString omStrNoCase = ouFunType.m_omStrFunName;
    omStrNoCase.MakeLower();
    if(omStrNoCase == "if" || omStrNoCase == "switch" || omStrNoCase =="do"
                                        || omStrNoCase == "while"||omStrNoCase == "for"
                                        || omStrNoCase == "return"|| omStrNoCase == "case" || omStrNoCase=="frframe" )
    {
        return CONDITIONAL_STATEMENT;
    }
    /*if(ouFunType.m_omStrFunName == "if" || ouFunType.m_omStrFunName == "switch" || ouFunType.m_omStrFunName =="do"
                                        || ouFunType.m_omStrFunName == "while"||ouFunType.m_omStrFunName == "for"
                                        || ouFunType.m_omStrFunName == "return" )
    {
        return CONDITIONAL_STATEMENT;
    }*/
    //Find Arguments
    INT nCurPos = omStrFunction.Find("(", 0);
	INT nArgCount = 0;
    nCurPos++;
    //Delete Last one )
    if( eType == TYPE_FUNCALL)
    {
        INT nCount = omStrFunction.GetLength();
        if(nCount > 0)
        {
            if(omStrFunction[nCount-1] == ')')
            {
                omStrFunction.Delete(nCount-1);
            }
        }
    }
    
	CString omStrTemp = "";
	vTokenize(omStrFunction, ",", omStrTemp, nCurPos);
    ouFunType.m_ouArgList.RemoveAll();
    while (omStrTemp != "")
    {
        if( eType == TYPE_FUNDEF)
        {
            omStrTemp.TrimLeft(" ");
            INT nIndex = omStrTemp.Find(" ");
            if( nIndex >= 0)	//FUNCTION DEFINITION
            {
                bRet = VALID_FUNCTION;
                CString omStrArgType = omStrTemp.Left(nIndex);
				
                CString omStrArgName = omStrTemp.Right(omStrTemp.GetLength() - nIndex);
                omStrArgName.TrimLeft("\n\t");      //TODO::CHECK ' '
                omStrArgName.TrimRight(")\n\t");    //TODO::CHECK ' '
                omStrArgName.Remove(' ');
                omStrArgName.TrimLeft("*");
				omStrArgName.TrimRight("*");
                caplArgType ouArgType = GetArgTypeFromName(omStrArgType);
			    
				if(ouArgType ==CAPL_MESSAGE)
			    {
				    ouFunType.m_bMsgArgPresent = TRUE;
			    }
                ouFunType.m_ouArgList.Add(ouArgType);
                ouFunType.m_omStrFunNameArray.Add(omStrArgName);
			    nArgCount++;
            }
            else
            {
                bRet = INVALID_FUNCTION;
            }
        }
		else if( eType == TYPE_FUNCALL)		//FUNCATION CALL
		{

            omStrTemp.TrimLeft(" \n\t");            //TODO::CHECK ' '
            omStrTemp.TrimRight(" \n\t");          //TODO::CHECK ' '
            if ( omStrTemp[0] == '"' && omStrTemp[omStrTemp.GetLength() - 1] != '"')
            {
                omStrTemp += ','; //missed in tokeniser
                while(nCurPos < omStrFunction.GetLength())
                {
                    char ch = omStrFunction[nCurPos];
                    if ( ch != ',' )
                    {
                        omStrTemp += ch;
                        nCurPos++;
                    }
                    else
                    {
						nCurPos++;	//to get "," position
                        break;
                    }
                }
            }
            ouFunType.m_omStrFunNameArray.Add(omStrTemp);
			caplArgType argType = CAPL_VOID;
			ouFunType.m_ouArgList.Add(argType);
			nArgCount++;
		}
		omStrTemp.Empty();
		vTokenize(omStrFunction, ",", omStrTemp, nCurPos);
    };
    return VALID_FUNCTION;
}

/**
* \brief         Removes the Line and block comments from the String
* \param[inout]  CString& omTempString - String input and output
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
BOOL RemoveComments(CString& omTempString)
{
    //Remove Block Comments
    BOOL bRet = TRUE;
    INT nStringCount = omTempString.GetLength();
    //omTempString.Trim(" \t\n");
    for(INT i=0; i < omTempString.GetLength();)
    {
        INT nIndex = omTempString.Find("/*", i);
        if(nIndex >= 0)
        {
            i = omTempString.Find("*/", nIndex);
            if(i >= 0)
            {
				CString omTempLeft = omTempString.Left(nIndex);
				CString omTempRight = omTempString.Right(omTempString.GetLength()-i-2);
				//Mid(nIndex, i-nIndex+2);
				omTempLeft.TrimRight(" \n\t");
				omTempRight.TrimRight(" \n\t");
				omTempString = omTempLeft + " " + omTempRight;
                i = 0;
            }
        }
        else
        {
            break;
        }
    }

    //Remove Line Comments
    //omTempString.Trim(" \t\n");
    nStringCount = omTempString.GetLength();
    for(INT i=0; i < omTempString.GetLength();)
    {
       // omTempString.Trim(" \t\n");
        INT nIndex = omTempString.Find("//", i);
        if(nIndex >= 0)
        {
            i = omTempString.Find("\n", nIndex);
            if(i >= 0)
            {
                omTempString.Delete(nIndex, i-nIndex+1);
                i = nIndex;
            }
            else
            {
                bRet = FALSE;
                break;
            }
        }
        else
        {
            break;
        }
    }
    return bRet;
}
/**
* \brief         Handles the CAPL "on prestart" event
* \param[in]     None  
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleOnPreStart()
{
	CString omTemp="";
	omTemp = defSTR_Header;
	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());
	fprintf(yyout, "%s", (LPCSTR)omTemp);
    g_ouGlobalVariables.omStrLastEvent = "Utils_PreStart */\n";
    fprintf(yyout,g_ouGlobalVariables.omStrLastEvent);
    fprintf(yyout,defUtils_PrestartDef);
    g_ouGlobalVariables.g_omStrFunCalls.Add("void Utils_PreStart();");
    bPreStart = TRUE;
}
/**
* \brief         Handles the CAPL "on start" event
* \param[in]     None  
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleOnStart()
{
	CString omTemp="";
	omTemp = defSTR_Header;
	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());
	fprintf(yyout, "%s", (LPCSTR)omTemp);
    g_ouGlobalVariables.omStrLastEvent = defSTR_OnBusConnectHeder;
    fprintf(yyout, "%s", defSTR_OnBusConnectHeder);
    fprintf(yyout, defSTR_OnBusConnect);
    CString omStrTemp = defSTR_OnBusConnect;
    omStrTemp.TrimRight(defSTR_FunctionDefinition);

    omStrTemp = defSTF_Extern + omStrTemp + ";";
    g_ouGlobalVariables.g_omStrFunCalls.Add(omStrTemp);
   
	if (CAN == g_ouGlobalVariables.m_eBus && g_MsgInitialize.empty() == false)
	{
		fprintf(yyout, g_MsgInitialize.c_str());
	}

    fprintf(yyout,"\n\tUtils_PreStart();");

}
/**
* \brief         Handles the CAPL "on stopMeasurement" event
* \param[in]     None  
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleOnstopMeasurement()
{
	CString omTemp="";
	omTemp = defSTR_Header;
	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());
    fprintf(yyout, "%s", (LPCSTR)omTemp);
    g_ouGlobalVariables.omStrLastEvent = defSTR_OnBusDisConnectHeder;
    fprintf(yyout, "%s", (LPCSTR)g_ouGlobalVariables.omStrLastEvent);
    fprintf(yyout, "%s", defSTR_OnBusDisConnect);

    CString omStrTemp = defSTR_OnBusDisConnect;
    omStrTemp.TrimRight(defSTR_FunctionDefinition);
    omStrTemp = defSTF_Extern + omStrTemp + ";";
    g_ouGlobalVariables.g_omStrFunCalls.Add(omStrTemp);
}
/**
* \brief         Handles the CAPL "on message" event
* \param[in]     None  
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleOnmessage(CString& omStrMessage)
{
    g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_MESSAGE; //to be used to convert "this"
	CString omTemp="";
	omTemp = defSTR_Header;
	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());
	fprintf(yyout, "%s", (LPCSTR)omTemp);
    CString omStrFunctionName;
    //Check for CAN Channel Filter
    int nChannel = 0;
    int nStart = omStrMessage.Find(".");
    if( nStart >= 0 )
    {
        CString omStrCanCh = omStrMessage.Left(nStart);
        omStrCanCh.MakeLower();
        int nCanStart = omStrCanCh.Find("can");
        if (nCanStart >= 0)
        {
            omStrCanCh.Delete(nCanStart, 3);
            if (isdigit(omStrCanCh[0]))
            {
                omStrMessage = omStrMessage.Right(omStrMessage.GetLength()-nStart-1);
                nChannel = atol(omStrCanCh.GetBuffer(MAX_PATH));
            }
        }
    }
	CString omStrBMMsgFormat, omStrMsgNameDecl, omStrBMMsgWrapperFormat, omStrBMMsgWrapperStart;
    int start;
    if(omStrMessage[0]=='*')
    {
        g_ouGlobalVariables.omStrLastEvent = defSTR_OnMsgAllHeader;
        omStrBMMsgFormat = defSTR_OnMsgAllStart;
    }
    else if( !isdigit( omStrMessage[0] ) )
    {//for message name
        start = omStrMessage.Find("::", 0);
        if( start != -1 )
        {
            omStrMessage = omStrMessage.Right( omStrMessage.GetLength() - start - 2 );
            start = omStrMessage.Find("(", 0);
            if(start != -1)
            {
                omStrMessage = omStrMessage.Left(start);
            }
            omStrMessage.TrimLeft(" ");
            omStrMessage.TrimRight(" ");

            MsgVariables ouMsgVariable;
            ouMsgVariable.m_omStrMsgType = omStrMessage;
            ouMsgVariable.m_omStrMsgName = "RxMsg";
            g_ouGlobalVariables.g_ouLocalMsgVariables.Add(ouMsgVariable);
        }
        g_ouGlobalVariables.omStrLastEvent = defSTR_MsgNameHeader;
        g_ouGlobalVariables.omStrLastEvent += omStrMessage + defSTR_HeaderFormat;
        

		if (CAN == g_ouGlobalVariables.m_eBus)
		{
			omStrMsgNameDecl.Format(defSTR_MsgNameDecl, omStrMessage, omStrMessage);
			omStrBMMsgWrapperStart.Format(defSTR_MsgNameWrapperStart, omStrMessage, omStrMessage);
			omStrBMMsgWrapperFormat.Format(defSTR_MsgNameWrapperDef, omStrMessage, omStrMessage);
			omStrBMMsgFormat.Format(defSTR_MsgNameStart, omStrMessage, omStrMessage);
		}
		else
		{
			omStrBMMsgFormat.Format(defSTR_MsgNameStart, omStrMessage, omStrMessage);
		}        
        
        //we have replaced "STCAN_MSG" with message name in this case
    } 
    else 
    {
        if( omStrMessage[ omStrMessage.GetLength() - 1] == 'x' ||
            omStrMessage[ omStrMessage.GetLength() - 1] == 'X' )
        {//avoid x for extended
            omStrMessage = omStrMessage.Left( omStrMessage.GetLength() - 1 );
        }
        start = omStrMessage.Find('-',0);

        if ( start >= 0 )
        {
			//means msgrange
			g_ouGlobalVariables.omStrLastEvent = defSTR_MsgIdRangeHeader;
			CString id;
			id = omStrMessage.Left( start );
			if( id[1] == 'x' || id[1] == 'X')
			{//hex
				id = id.Right( id.GetLength() - 2 );//avoid"0x"
			}
			else
			{//decimal
				char *stop;
				id.Format("%x",strtol( id, &stop, 10 ));
				//          id = CString( strtol(id,&stop,16) );
			}
			g_ouGlobalVariables.omStrLastEvent += id + '_';
			id = omStrMessage.Right( omStrMessage.GetLength() - start- 1);
			if( id[1] == 'x' || id[1] == 'X')
			{//hex
				id = id.Right( id.GetLength() - 2 );//avoid"0x"
			}
			else
			{//decimal
				char *stop;
				id.Format("%x",strtol( id, &stop, 10 ));
				//      id = CString( strtol(id,&stop,16) );
			}
			g_ouGlobalVariables.omStrLastEvent +=id + defSTR_HeaderFormat;
			start = g_ouGlobalVariables.omStrLastEvent.Find('_',0);
			id = g_ouGlobalVariables.omStrLastEvent.Right( g_ouGlobalVariables.omStrLastEvent.GetLength() - start - 1 );
			id = id.Left( id.GetLength() - 3 );//extract the range
			id.TrimRight();
            g_ouGlobalVariables.omStrLastEvent.Format("%s%s%s", defSTR_MsgIdRangeHeader, id, defSTR_HeaderFormat);
            omStrBMMsgFormat.Format(defSTR_MsgIdRangeStart,id);
        }
		else
		{
			CString omStrList;
            start = omStrMessage.Find(',',0);
            if ( start >=0 )
			{
				//For discreate
                start = 0;                
				CString omStrTemp = "";
				vTokenize(omStrMessage, ",", omStrTemp, start);
                while (omStrTemp != "")
                {
                    omStrTemp.TrimLeft(" ");
                    omStrList += omStrTemp + "_";				
					omStrTemp.Empty();
					vTokenize(omStrMessage, ",", omStrTemp, start);
                };
                omStrList.TrimRight("_");
                g_ouGlobalVariables.omStrLastEvent = defSTR_MsgIdRangeHeader + omStrList + defSTR_HeaderFormat;
                omStrBMMsgFormat.Format(defSTR_MsgIdListStart, omStrList);
			}
			else
			{
				//means id
				g_ouGlobalVariables.omStrLastEvent = defSTR_MsgIdUs ;
				if( omStrMessage[1] =='x' || omStrMessage[1] =='X')
				{
					omStrMessage = omStrMessage.Right( omStrMessage.GetLength() - 2);
				}
				else
				{
					char *stop;

					omStrMessage.Format("%x",strtol( omStrMessage, &stop, 10)) ;

				}
				g_ouGlobalVariables.omStrLastEvent += omStrMessage + defSTR_HeaderFormat;
                omStrBMMsgFormat.Format(defSTR_MsgIdStart,omStrMessage);
			}
		}
    }
    if (omStrBMMsgFormat.GetLength() > 0)
    {
        fprintf(yyout,g_ouGlobalVariables.omStrLastEvent);
        fprintf(yyout,omStrBMMsgFormat,omStrMessage);
		

		
		{
			if (-1 != omStrBMMsgFormat.Find(defSTR_MsgNameHeader))
			{
				omStrFunctionName = omStrBMMsgWrapperStart;
			}
			else
			{
				omStrFunctionName = omStrBMMsgFormat;
			}			
			omStrFunctionName.TrimRight(defSTR_FunctionDefinition);
			omStrFunctionName = defSTF_Extern + omStrFunctionName + ";";
			omStrFunctionName.Replace("_7016A3A052554b2dA5526D504AA2CE1A", "");

			if (CAN == g_ouGlobalVariables.m_eBus && omStrMsgNameDecl.IsEmpty() == false)
			{
				g_ouGlobalVariables.g_omStrFunCalls.Add(omStrMsgNameDecl);
				g_ouGlobalVariables.g_omStrFunWrappers.Add(omStrBMMsgWrapperFormat);
			}			
			g_ouGlobalVariables.g_omStrFunCalls.Add(omStrFunctionName);
		}        
		
        if(nChannel > 0)
        {
            fprintf(yyout, defSTR_ChannelFilter, nChannel);
        }
    }
}
/**
* \brief         Handles the CAPL Typecasting
* \param[in]     None  
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleTypeCaste()
{
    CString omStrExpression = yytext;
    CString omStrLeftOperand;
	omStrExpression.TrimLeft(" \n");
	omStrExpression.TrimRight(" \n");

    int nStart, nEnd;
    if( omStrExpression[0] == ';' || omStrExpression[0] == ',')
    {
        fprintf( yyout,"%c", omStrExpression[0] );
    }
    
    nStart = omStrExpression.Find("(",0);
    nEnd = omStrExpression.FindOneOf(")");

    omStrLeftOperand = omStrExpression.Mid(nStart+1, nEnd-nStart-1);
	omStrLeftOperand.TrimLeft(" ");
	omStrLeftOperand.TrimRight(" ");

    if(GetBusmasterStorageType(omStrLeftOperand, omStrLeftOperand) == S_OK )
    {
        fprintf(yyout,"( %s )", (LPCSTR)omStrLeftOperand);
    }
    else
    {
		ECHO_INPUT;
    }
}

/**
* \brief         Handles the CAPL Message Type casting
* \param[in]     None  
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleMsgTypeCaste()
{
    CString omStrExpression;
    CString omStrLeftOperand, omStrRightOperand;
    int nStart, nEnd;
    if( yytext[0] == ';' || yytext[0] == ',')
    {
        fprintf( yyout,"%c",yytext[0] );
    }

    omStrExpression = yytext;

    nStart = omStrExpression.Find("=",0);
    omStrLeftOperand = omStrExpression.Left( nStart );//name of variable
    omStrLeftOperand.TrimLeft();
    omStrLeftOperand.TrimRight();
    
    nStart = omStrExpression.Find(")");
    nEnd = omStrExpression.FindOneOf(",;");

    omStrRightOperand = omStrExpression.Mid(nStart+1, nEnd-nStart-1);
	omStrRightOperand.TrimLeft(" \n");
	omStrRightOperand.TrimRight(" \n");

    if(omStrRightOperand.CompareNoCase("this") == 0)
    {
        omStrRightOperand = "RxMsg";
    }
    fprintf(yyout,"\n\t%s.m_sWhichBit.m_aucData = %s.m_sWhichBit.m_aucData;",(LPCSTR)omStrLeftOperand, (LPCSTR)omStrRightOperand);
}

/**
* \brief         Handles the CAPL Utility Function call
* \param[in]     None  
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleUtilityFun()
{
    BOOL bRetVal =TRUE;
    CString omStrText = yytext;
    bRetVal = RemoveComments(omStrText);
    bRetVal = RemoveComments(omStrText);
	CString omTemp="";
	
    if(bRetVal == TRUE)
    {
        FunctionPrototype ouFunType;
        INT nFunType = RetriveFunctionDetails(omStrText, ouFunType, TYPE_FUNDEF);

        if ( VALID_FUNCTION == nFunType)
        {
            g_ouGlobalVariables.g_ouLocalMsgVariables.RemoveAll();
            
            if( g_ouGlobalVariables.omStrLastEvent.GetLength() > 0)
            {
				if(g_ouGlobalVariables.omStrLastEvent == defSTR_GlobalVar)
				{
					omTemp = defSTR_UsingGlobalVarNameSpace;
					omTemp += defSTR_GlobalVarEnd;
				}
				else
				{
					omTemp = defSTR_EndComment;
				}
				omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());
                fprintf(yyout, "\n%s%s\n", (LPCTSTR)omTemp, (LPCTSTR)g_ouGlobalVariables.omStrLastEvent);
                g_ouGlobalVariables.omStrLastEvent.Empty();
            }
            CString omStrBMFunction;
            //TODO::
            FormBMFunction(ouFunType, omStrBMFunction);     

            g_ouGlobalVariables.omStrLastEvent = "Utils_"+ ouFunType.m_omStrFunName;
            g_ouGlobalVariables.omStrLastEvent += " */";

			omTemp = defSTR_Header;
			omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());

            fprintf(yyout, "%s%s", (LPCSTR)omTemp, (LPCSTR)g_ouGlobalVariables.omStrLastEvent);
            fprintf(yyout, "\n%s", omStrBMFunction.GetBuffer(MAX_PATH));
            
            omStrBMFunction.TrimRight(defSTR_FunctionDefinition);

            omStrBMFunction = omStrBMFunction + ";";
            g_ouGlobalVariables.g_omStrFunCalls.Add(omStrBMFunction);
        }
	    else if ( CONDITIONAL_STATEMENT == nFunType)
        {
            omStrText = yytext;
            CString omStrThisText;
            if(g_ouGlobalVariables.g_oucaplEventHandleState == CAPL_EVENT_KEY)
            {
                omStrThisText = defSTR_KeyValue;
            }
            else if(g_ouGlobalVariables.g_oucaplEventHandleState == CAPL_EVENT_MESSAGE)
            {
                omStrThisText = defSTR_RxMsg;
            }
            nReplaceThisInFunction(omStrText, omStrThisText);
            fprintf(yyout, "%s", omStrText.GetBuffer(MAX_PATH));
        }
        else
	    {
			ECHO_INPUT;
	    }
    }
}

/**
* \brief         Replaces this in the string
* \param[in]     CString& omStrText - input string  
* \param[out]    CString& omStrThisText - output string  
* \return        int
* \authors       Venkatanarayana Makam
* \date          
*/
int nReplaceThisInFunction(CString& omStrText, CString& omStrThisText)
{
    RemoveComments(omStrText);
    INT nStartIndex = omStrText.Find("(");
    INT nStopIndex = -1;
    if(nStartIndex >= 0)
    {
       nStopIndex = omStrText.Find(")", nStartIndex);
    }
    if(nStopIndex >= 0)
    {
        CString omStrArg = omStrText.Mid(nStartIndex+1, nStopIndex-nStartIndex-1);
        INT nThisIndex = 0;
        INT nThisLength = 4;
        INT nCurrentIndex = 0;
        while(nCurrentIndex <= omStrArg.GetLength() )
        {
            nThisIndex = omStrArg.Find("this", nCurrentIndex);
            if(nThisIndex == 0)
            {
                if(isalpha(omStrArg[nThisIndex+nThisLength]) == FALSE)
                {
                    omStrArg.Delete(nThisIndex, nThisLength);
                    omStrArg.Insert(nThisIndex, omStrThisText);
                    nCurrentIndex  += omStrThisText.GetLength();
                }
                else
                {
                    nCurrentIndex += nThisLength;
                }
            }
            else if(nThisIndex >= 1)
            {
                if(( isalpha(omStrArg[nThisIndex-1]) == FALSE ) && (isalpha(omStrArg[nThisIndex+nThisLength]) == FALSE) )
                {
                    omStrArg.Delete(nThisIndex, nThisLength);
                    omStrArg.Insert(nThisIndex, omStrThisText);
                    nCurrentIndex  += omStrThisText.GetLength();
                }
                else
                {
                    nCurrentIndex += nThisLength;
                }
            }
            else if(nThisIndex == -1)
            {
                omStrText.Delete(nStartIndex+1, nStopIndex-nStartIndex-1);
                omStrText.Insert(nStartIndex+1, omStrArg);
                break;
            }
        }
    }
    return 0;
}


INT FormBMFunction(FunctionPrototype& ouFunProtoType,CString& omStrBMFunction)
{
    CString omStrTemp;
    GetNameFromArgType(ouFunProtoType.m_ouRetType, omStrTemp);
    omStrBMFunction = omStrTemp + " ";
    omStrBMFunction += defSTR_Utils;
    omStrBMFunction += ouFunProtoType.m_omStrFunName + "(";
    INT_PTR nCount = ouFunProtoType.m_ouArgList.GetSize();
    for(INT_PTR i=0; i<nCount; i++)
    {
        //POSITION pos = ouFunProtoType.m_ouArgList.FindIndex(i);
        MsgVariables ouMsgVariable;
        GetNameFromArgType(ouFunProtoType.m_ouArgList.GetAt(i), omStrTemp);
        if(omStrTemp == "STCAN_MSG" )
        {
            omStrTemp += "&";
            //ouMsgVariable.m_omStrMsgType = 
        }

        omStrBMFunction += omStrTemp + " ";
        omStrBMFunction += ouFunProtoType.m_omStrFunNameArray.GetAt(i) + ",";
    }
    omStrBMFunction.TrimRight(",");
    omStrBMFunction += ")\n{";
    return 0;
}

void getCaplEvent(CString& caplEvent, CString& caplEventFunction)
{
	caplEvent = "";
	caplEventFunction = "";
	
	if (g_ouGlobalVariables.omStrLastEvent.GetLength() > 0)
	{
		CString omTempEndComment = "";
		if (g_ouGlobalVariables.omStrLastEvent == defSTR_GlobalVar)
		{
			omTempEndComment = defSTR_UsingGlobalVarNameSpace;
			omTempEndComment += defSTR_GlobalVarEnd;
			g_bGlobalVariable = false;
		}
		else
		{
			omTempEndComment = defSTR_EndComment;

        }
        omTempEndComment.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());
        fprintf ( yyout, "%s%s", (LPCSTR) omTempEndComment, (LPCSTR) g_ouGlobalVariables.omStrLastEvent );
        g_ouGlobalVariables.omStrLastEvent.Empty();
    }
    g_ouGlobalVariables.g_ouLocalMsgVariables.RemoveAll();
    //g_lFunctionStartPos = ftell(yyout);

	CString omStrName = yytext;
	RemoveComments(omStrName);

	CString omStrTemp = omStrName;
	omStrTemp.MakeLower();
	INT nIndex;
	char chTemp[MAX_PATH];
	if (sscanf_s(omStrTemp, " on %[a-zA-Z] %*s", chTemp, sizeof(chTemp)) == 1 || sscanf_s(omStrTemp, " on %[*]", chTemp, sizeof(chTemp)) == 1)
	{
		caplEvent = chTemp;
		caplEventFunction = omStrName;
	}

}
/**
* \brief         Called by the lexer to Handle the CAPL Events
* \param[in]     None
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleCanCaplEvents()
{
	CString caplEvent, omStrName;
	CString omStrTemp = "";
	INT nIndex;
	getCaplEvent(caplEvent, omStrName);
	omStrTemp = omStrName;
	omStrTemp.MakeLower();
	if (caplEvent != "")
    {
		CString omTemp = "";
		omTemp = defSTR_Header;
		omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[g_ouGlobalVariables.m_eBus].c_str());
		if (caplEvent == "key")
        {
			nIndex = omStrTemp.Find("key");
            omStrTemp = omStrName;
            omStrTemp = omStrName.Right( omStrName.GetLength() - nIndex-3); 
            omStrTemp.TrimLeft(" \n\t");
            omStrTemp.TrimRight(" \n\t{");
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_KEY;
            vHandleOnkey(omStrTemp);
        }
		else if (caplEvent == "message")
        {
			nIndex = omStrTemp.Find("message");
            omStrTemp = omStrName;
            omStrTemp = omStrName.Right( omStrName.GetLength() - nIndex-7); 
            omStrTemp.TrimLeft(" \n\t");
            omStrTemp.TrimRight(" \n\t{");
            vHandleOnmessage(omStrTemp);
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_MESSAGE;
        }
		else if (caplEvent == "timer")
        {
			nIndex = omStrTemp.Find("timer");
            CString omStrFuncName;
            omStrTemp = omStrName;
            omStrTemp = omStrName.Right( omStrName.GetLength() - nIndex-5); 
            omStrTemp.TrimLeft(" \n\t");
            omStrTemp.TrimRight(" \n\t{");

            fprintf ( yyout, "\n%s%s%s%s\n", (LPCSTR) omTemp, defSTR_OnTimer, (LPCSTR) omStrTemp, " */" );

            
            omStrFuncName.Format(defSTR_TimerStart,omStrTemp);
            fprintf(yyout,"%s", omStrFuncName.GetBuffer(MAX_PATH));
            
            omStrFuncName.TrimRight(defSTR_FunctionDefinition);
            omStrFuncName = defSTF_Extern + omStrFuncName + ";";
            g_ouGlobalVariables.g_omStrFunCalls.Add(omStrFuncName);
            
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_TIMER;
            
            g_ouGlobalVariables.omStrLastEvent = defSTR_OnTimer + omStrTemp + " */";
        }
		else if (caplEvent == "prestart")
        {
            omStrTemp.TrimLeft(" \n\t");
            omStrTemp.TrimRight(" \n\t{");
            vHandleOnPreStart();
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_PRESTART;
        }
       
		else if (caplEvent == "envvar")
        {
            CString omStrText = yytext;
            omStrText.Remove('\n');
            //fprintf(yyout, "%s", defSTR_UnSupportedFunctionStart);
            fprintf ( yyout, "%s", (LPCSTR) omStrText );
            /*g_ouGlobalVariables.g_nLastParseState = SecondParse;
            BEGIN(UnsupportedParse);*/
        }
		else if (caplEvent == "prestop")
        {
            CString omStrText = yytext;
            omStrText.Remove('\n');
            //fprintf(yyout, "%s", defSTR_UnSupportedFunctionStart);
            fprintf ( yyout, "%s", (LPCSTR) omStrText );
            /*g_ouGlobalVariables.g_nLastParseState = SecondParse;
            BEGIN(UnsupportedParse);*/
        }
		else if (caplEvent == "stopmeasurement")
        {
            omStrTemp.TrimLeft(" \n\t");
            omStrTemp.TrimRight(" \n\t{");
            vHandleOnstopMeasurement();
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_STOP;
        }
		else if (caplEvent == "start")
        {
            omStrTemp.TrimLeft(" \n\t");
            omStrTemp.TrimRight(" \n\t{");
			g_bBusConnectGenerated = true;
            vHandleOnStart();
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_START;
        }
		else if (caplEvent == "busoff")
        {
            fprintf ( yyout, "\n%s%s", (LPCSTR) omTemp, defSTR_BusOffHeader );
            g_ouGlobalVariables.omStrLastEvent = defSTR_BusOffHeader;
            fprintf(yyout, "%s", defSTR_BusOffStart);
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_BUSOFF;

            CString omStrTemp = defSTR_BusOffStart;
            omStrTemp.TrimRight(defSTR_FunctionDefinition);

            omStrTemp = defSTF_Extern + omStrTemp + ";";
            g_ouGlobalVariables.g_omStrFunCalls.Add(omStrTemp);
        }
		else if (caplEvent == "erroractive")
        {
            fprintf ( yyout, "\n%s%s", (LPCSTR) omTemp, defSTR_ErrorActiveHeader );
            g_ouGlobalVariables.omStrLastEvent = defSTR_ErrorActiveHeader;
            fprintf(yyout, "%s", defSTR_ErrorActiveStart);
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_ERRORACTIVE;

            CString omStrTemp = defSTR_ErrorActiveStart;
            omStrTemp.TrimRight(defSTR_FunctionDefinition);
            
            omStrTemp = defSTF_Extern + omStrTemp + ";";
            g_ouGlobalVariables.g_omStrFunCalls.Add(omStrTemp);
        }
		else if (caplEvent == "errorpassive")
        {
            nIndex = omStrTemp.Find("errorpassive");
            fprintf ( yyout, "\n%s%s", (LPCSTR) omTemp, defSTR_ErrorPassiveHeader );
            g_ouGlobalVariables.omStrLastEvent = defSTR_ErrorPassiveHeader;
            fprintf(yyout, "%s", defSTR_ErrorPassiveStart);
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_ERRORPASSIVE;

            CString omStrTemp = defSTR_ErrorPassiveStart;
            omStrTemp.TrimRight(defSTR_FunctionDefinition);

            omStrTemp = defSTF_Extern + omStrTemp + ";";
            g_ouGlobalVariables.g_omStrFunCalls.Add(omStrTemp);

        }
		else if (caplEvent == "warninglimit")
        {
            fprintf ( yyout, "\n%s%s", (LPCSTR) omTemp, defSTR_WarningLimitHeader );
            g_ouGlobalVariables.omStrLastEvent = defSTR_WarningLimitHeader;
            fprintf(yyout, "%s", defSTR_WarningLimitStart);
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_WARNINGLIMIT;

            CString omStrTemp = defSTR_WarningLimitStart;
            omStrTemp.TrimRight(defSTR_FunctionDefinition);
            omStrTemp = defSTF_Extern + omStrTemp + ";";
            g_ouGlobalVariables.g_omStrFunCalls.Add(omStrTemp);
        }
		else if (caplEvent == "errorframe")
        {
            fprintf ( yyout, "\n%s%s", (LPCSTR) omTemp, defSTR_ErrorFrameHeader );
            g_ouGlobalVariables.omStrLastEvent = defSTR_ErrorFrameHeader;
            fprintf(yyout, "%s", defSTR_ErrorFrameStart);
            g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_ERRORFRAME;

            CString omStrTemp = defSTR_ErrorFrameStart;
            omStrTemp.TrimRight(defSTR_FunctionDefinition);
            omStrTemp = defSTF_Extern + omStrTemp + ";";
            g_ouGlobalVariables.g_omStrFunCalls.Add(omStrTemp);
        }
		else if (caplEvent == "*") //Currently mapped to on frframe.
		{
			nIndex = omStrTemp.Find("*");
			omStrTemp = omStrName;
			omStrTemp = omStrName.Right( omStrName.GetLength() - nIndex); 
            omStrTemp.TrimLeft(" \n\t");
            omStrTemp.TrimRight(" \n\t{");
			g_ouGlobalVariables.g_oucaplEventHandleState = CAPL_EVENT_MESSAGE;
			vHandleOnmessage(omStrTemp);
		}
		
		else
		{
			ECHO_INPUT;
			sprintf(g_chErrorString, defSTR_UnSupportedEventHandler, (LPCSTR)caplEvent);
			g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);
		}
    }
   
 
}

/**
* \brief         Handles the CAPL Timer Decarations
* \param[in]     CString& omStrVal - Timer Declarations
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/
void vHandleTimerDeclaration(CString& omStrVal)
{
    try
    {
    CString val, name ;
    int start;
    if( omStrVal[0] == ';')
    {
        fprintf(yyout,";");
    }
    val = omStrVal;

    start = val.Find(' ',0);

    name = val.Left(start  );
    name.MakeLower();//extract the datatype
    omStrVal.Empty();
    if( name.Find("timer") != -1 )
    {//it is timer
        val = val.Right( val.GetLength() - start - 1);
        val.TrimLeft();
        while(val.GetLength() != 0 )//till end of text
        {
            start = val.FindOneOf(",;");
            name = val.Left( start);
            name.TrimLeft();
            name.TrimRight();
            CString omStrTemp;
            omStrTemp.Format("%s %s[] = \"%s%s\";", "char", name, defSTR_OnTimer, name);
            omStrVal += omStrTemp;
            //ouTimerName.bAdd( name );//add in array
            val = val.Right( val.GetLength() - start - 1 );
        }
    }
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vHandleTimerDeclaration\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}


/**
* \brief         Called by lexer to Handle .byte, .long, .word calls on a message variable
* \param[in]     CString& omStrVal - Timer Declarations
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/                        
void vHandleByte(CString val)
{
	try
    {
    //yytext asd.long(1)
    // this.byte(2)
    // abc.word(1);
    CString name,type,index;
    int start , end ;

    //val = yytext;
    start = val.Find(".",0);

    name = val.Left(start);
    name.TrimLeft();
    name.TrimRight();

    end = val.Find("(",start);
    type = val.Mid(start+1,end-start-1);
    type.TrimLeft();
    type.TrimRight();
    type.MakeLower();//for case insensitive comparison

    start = end;

    end = val.Find(")",start);

    index = val.Mid( start + 1 , end - start - 1 );
    index.TrimLeft();
    index.TrimRight();

    if(name == defSTR_this )
    {
        //replace "this" withe "RxMsg"
        name = defSTR_RxMsg;
    }
	if (CAN == g_ouGlobalVariables.m_eBus)
	{
		if( type == defSTR_long ) 
		{
			//if long
			fprintf(yyout,defSTR_LongCnvrt, (LPCSTR)name,(LPCSTR)name,(LPCSTR)index);
		}

		if( type == defSTR_byte )
		{
			//if byte
			fprintf(yyout,defSTR_ByteCnvrt, (LPCSTR)name,(LPCSTR)name,(LPCSTR)index);
		}

		if( type == defSTR_word )
		{
			//if word
			fprintf(yyout,defSTR_WordCnvrt, (LPCSTR)name,(LPCSTR)name,(LPCSTR)index);
		}
	}
	else
	{
		if(type==defSTR_byte||type==defSTR_long||type==defSTR_word||type==defSTR_dword||type==defSTR_qword||type==defSTR_char||type==defSTR_int||type==defSTR_int64)	
		{
			fprintf(yyout,defSTR_TypeAtCnvrt,(LPCSTR)type,(LPCSTR)name,(LPCSTR)name,(LPCSTR)index);
		}
	}
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vHandleByte\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
    

}

void vHandleUnsupportedFunctionEnd()
{
	//BEGIN(g_ouGlobalVariables.g_nLastParseState);
}
void vHandleUnsupportedLine()
{
	fprintf(yyout, "\n//");
}


std::string getBusmasterKeyword()
{
	CString omStrVal;
	CString omStrMsg, omStrSignal;

	// yytext -- "Msg.Signal"

	int nIndex;
	omStrVal = yytext;

	nIndex = omStrVal.Find(".", 0);
	//omStrMsg = (omStrVal.Mid(0,nIndex).Trim().GetBuffer(MAX_PATH));//name of message
	//omStrSignal = omStrVal.Right(omStrVal.GetLength() - nIndex-1).Trim().GetBuffer(MAX_PATH);

	string omStrTempVal = omStrVal;
	omStrVal = omStrVal.Mid(0, nIndex);
	omStrVal.TrimLeft();
	omStrVal.TrimRight();
	omStrMsg = (omStrVal.GetBuffer(MAX_PATH));//name of message

	omStrVal = omStrTempVal.c_str();
	omStrVal = omStrVal.Right(omStrVal.GetLength() - nIndex - 1);
	omStrSignal = omStrVal.GetBuffer(MAX_PATH);
	if (omStrMsg == defSTR_this)
	{//replace "this" with "RxMsg".
		omStrMsg = defSTR_RxMsg;
	}

	CString omStrSignalAlias;
	CString omStrMsgType = "";
	CString omStrMsgObj = "";
	MsgVariables ouMsgVariable;
	CString omStrTempType = omStrMsg;
	omStrVal.MakeLower();
	return g_obBusmasterKeyword.GetBusmasterEquivalent(std::string(omStrVal));
}

/**
* \brief         Replece '.' with m_sWhichBit for valid message and signals
                 Also it handles the Multiplex signals
* \param[in]     None
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/           
void vHandleDot()
{
	
	//class.member
	try
    {
		CString omStrVal;
		CString omStrMsg, omStrSignal;

		// yytext -- "Msg.Signal"
   
		int nIndex;
		omStrVal = yytext;

		nIndex = omStrVal.Find(".",0);
		//omStrMsg = (omStrVal.Mid(0,nIndex).Trim().GetBuffer(MAX_PATH));//name of message
		//omStrSignal = omStrVal.Right(omStrVal.GetLength() - nIndex-1).Trim().GetBuffer(MAX_PATH);

		string omStrTempVal = omStrVal;
		omStrVal = omStrVal.Mid(0,nIndex);
		omStrVal.TrimLeft();
		omStrVal.TrimRight();
		omStrMsg = (omStrVal.GetBuffer(MAX_PATH));//name of message

		omStrVal = omStrTempVal.c_str();
		omStrVal = omStrVal.Right(omStrVal.GetLength() - nIndex-1);
		omStrSignal = omStrVal.GetBuffer(MAX_PATH);
		if( omStrMsg == defSTR_this)
		{//replace "this" with "RxMsg".
			omStrMsg = defSTR_RxMsg ;
		}
				
		CString omStrSignalAlias;
		CString omStrMsgType="";
		CString omStrMsgObj="";
		MsgVariables ouMsgVariable;
		CString omStrTempType = omStrMsg;
		omStrVal.MakeLower();
		std::string strBmEquivalent = getBusmasterKeyword();
		
		if ( strBmEquivalent.length() > 0)
		{
			if(0 == strBmEquivalent.compare("unsupported"))
			{
				CString omStrError =  omStrTempVal.c_str();
				fprintf(yyout,"%s", (LPCSTR)omStrError);
				sprintf(g_chErrorString,defSTR_UnSupportedVariableFormat, (LPCSTR)omStrError);
				g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);
				return;
			}
			omStrTempType+=".";
			if(	strBmEquivalent.compare("RX")==0)
			{
				if(omStrTempType == "RxMsg_7251A0FE3CF2460FBE53180FD7024F8F.")
				{
					omStrTempType = defSTR_RxMsg;
					omStrTempType+=".";
				}
				else
				{
					omStrTempType="";
				}
			}
			omStrVal=omStrTempType+strBmEquivalent.c_str();
			fprintf(yyout,"%s", (LPCSTR)omStrVal);
		}
		else if(bGetMessageObj(omStrMsg,ouMsgVariable) == TRUE)
		{
			omStrMsgType = ouMsgVariable.m_omStrMsgType;
			omStrMsgObj = ouMsgVariable.m_ouFlxMsgVar.m_omStrMsgObj;
			if (g_ouGlobalVariables.m_eBus == CAN)
			{
				CString strMsgType = omStrMsgType.GetBuffer(MAX_PATH);
				//TODO::Check
				g_ouGlobalVariables.g_ouDBC2DBFConverter->FindSignalAlias( strMsgType, omStrSignal, omStrSignalAlias);
				char chWarning[1024];
				if( omStrSignal != omStrSignalAlias )
				{
					sprintf(chWarning, "(Multiplexed Signal): signal <%s::%s> is Replaced with <%s::%s>", \
						(LPCSTR)strMsgType, (LPCSTR)omStrSignal, (LPCSTR)strMsgType, (LPCSTR)omStrSignalAlias);
					g_ouGlobalVariables.g_ouWarningStrings.insert(chWarning);
					fprintf(yyout,defSTR_MWBitAlias,(LPCSTR)omStrMsg, (LPCSTR)omStrSignal, (LPCSTR)omStrSignalAlias);
				}
				else
				{
					fprintf(yyout,defSTR_MWBit,(LPCSTR)omStrMsg, (LPCSTR)omStrSignal);
				}
			}
		}
		else
		{
			CString omStrError =  omStrTempVal.c_str();
			fprintf(yyout,"%s", (LPCSTR)omStrError);
			sprintf(g_chErrorString,defSTR_UnSupportedVariableFormat, (LPCSTR)omStrError);
			g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);
		}
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vHandleDot\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }

}


/**
* \brief         Retrives the Database message for a given argument name
* \param[in]     CString omStrMsgObj - Argument name
* \param[out]    CString ouMsgVariable - message variable
* \return        BOOL TRUE - if omStrMsgObj is valid argument
* \authors       Robin G.K.
* \date          30.07.2015
*/ 
bool bGetMessageObj(const CString& omStrMsgObj,MsgVariables& ouMsgVariable)
{
	INT_PTR nCount;
    bool bFound = false;
	MsgVariables ouMsg;
    //First Local;
    nCount = g_ouGlobalVariables.g_ouLocalMsgVariables.GetSize();
    for(INT_PTR i = 0; i < nCount; i++ )
    {
        ouMsg = g_ouGlobalVariables.g_ouLocalMsgVariables.GetAt(i);
        if(ouMsg.m_omStrMsgName == omStrMsgObj )
        {
			ouMsgVariable = ouMsg;
            bFound = true;
            break;
        }
    }
    //If not found Global 
    if( FALSE == bFound )
    {
        nCount = g_ouGlobalVariables.g_ouGlobalMsgVariables.GetSize();
        for(INT_PTR i = 0; i < nCount; i++ )
        {
            ouMsg = g_ouGlobalVariables.g_ouGlobalMsgVariables.GetAt(i);
            if(ouMsg.m_omStrMsgName == omStrMsgObj )
            {
				ouMsgVariable = ouMsg;
                bFound = true;
                break;
            }
        }
    }
    return bFound;
}



/**
* \brief         Removes the .Phys name
* \param[in]     None
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/     
void vRemovePhys()
{
    try
    {
        // yytext -- "*.phys"
        CString omStrParse;
        omStrParse = yytext;

        int nIndex = omStrParse.Find(".",0);
        if( nIndex != -1 )
        {
            fprintf( yyout, "%s/*%s */", (LPCSTR)omStrParse.Mid( 0, nIndex),
                                         (LPCSTR)omStrParse.Mid(nIndex));
        }
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vRemovePhys\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}

/**
* \brief         Replaces the "this" variable with appropriate variable
* \param[in]     None
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/     
void vCnvrtThis()
{
    try
    {
    if( CAPL_EVENT_MESSAGE == g_ouGlobalVariables.g_oucaplEventHandleState )
    {//if message handler
        fprintf(yyout,defSTR_RxMsg);
    }
	else if( CAPL_EVENT_KEY == g_ouGlobalVariables.g_oucaplEventHandleState )
    {//if key handler
        fprintf(yyout,defSTR_KeyValue);
    }
    if( CAPL_EVENT_BUSOFF == g_ouGlobalVariables.g_oucaplEventHandleState )
    {//if message handler
        fprintf(yyout, "%s.%s", defSTR_ErrorMsg, defSTR_ChannelVar);
    
    }
    else if( ( g_ouGlobalVariables.g_oucaplEventHandleState >= CAPL_EVENT_ERRORACTIVE ) &&   
                ( g_ouGlobalVariables.g_oucaplEventHandleState <= CAPL_EVENT_ERRORFRAME ) )
    {//if key handler
        fprintf(yyout, "%s.%s + %s.%s", defSTR_ErrorMsg, defSTR_TxErrorVar, defSTR_ErrorMsg, defSTR_RxErrorVar);
    }
    }
    catch(...)
    {
        CString cs;
        

        cs.Format(ExceptionFormat,"\"vCnvrtThis\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }

}

/**
* \brief         Replaces the "this." variable with RxMsg variable
* \param[in]     None
* \return        void
* \authors       Venkatanarayana Makam
* \date          
*/     
void vCnvrtThisData()
{
	CString val = yytext;
    if( CAPL_EVENT_MESSAGE == g_ouGlobalVariables.g_oucaplEventHandleState )
    {//if message handler
        CString strVal = yytext;
        CString strSignal;
        int start = strVal.Find("this.", 0);
        strVal = strVal.Right(strVal.GetLength() - start - 5);
        
        strSignal.Format("%s.%s", defSTR_RxMsg, strVal);
        fprintf(yyout, strSignal);
        //fprintf(yyout,"Next");
    
    }
	else if( CAPL_EVENT_BUSOFF == g_ouGlobalVariables.g_oucaplEventHandleState )
    {//if message handler
		bool bTokenFound = false;
		if(val.Find("this") != -1)
		{
			val.Replace("this", defSTR_ErrorMsg);
		}
		if(val.Find(defSTR_CAPLTxErrorCount) != -1)
		{
			bTokenFound = true;
			val.Replace(defSTR_CAPLTxErrorCount, defSTR_TxErrorVar);
		}
		if(val.Find(defSTR_CAPLRxErrorCount) != -1)
		{
			bTokenFound = true;
			val.Replace(defSTR_CAPLRxErrorCount, defSTR_RxErrorVar);
		}
		if(val.Find(defSTR_CAPLChannel) != -1)
		{
			bTokenFound = true;
			val.Replace(defSTR_CAPLChannel, defSTR_ChannelVar);
		}
		if(bTokenFound == false)
		{
			fprintf(yyout, "%s.%s", defSTR_ErrorMsg, defSTR_ChannelVar);
		}
		else
		{
			fprintf(yyout, "%s", (LPCSTR)val);
		}
        
    }
    else if( ( g_ouGlobalVariables.g_oucaplEventHandleState >= CAPL_EVENT_ERRORACTIVE ) &&   
                ( g_ouGlobalVariables.g_oucaplEventHandleState <= CAPL_EVENT_ERRORFRAME ) )
    {//if key handler
		bool bTokenFound = false;
		if(val.Find("this") != -1)
		{
			val.Replace("this", defSTR_ErrorMsg);
		}
		if(val.Find(defSTR_CAPLTxErrorCount) != -1)
		{
			bTokenFound = true;
			val.Replace(defSTR_CAPLTxErrorCount, defSTR_TxErrorVar);
		}
		if(val.Find(defSTR_CAPLRxErrorCount) != -1)
		{
			bTokenFound = true;
			val.Replace(defSTR_CAPLRxErrorCount, defSTR_RxErrorVar);
		}
		if(bTokenFound == false)
		{
			fprintf(yyout, "%s.%s + %s.%s", defSTR_ErrorMsg, defSTR_TxErrorVar, defSTR_ErrorMsg, defSTR_RxErrorVar);
		}
		else
		{
			fprintf(yyout, "%s", (LPCSTR)val);
		}
    }
	
}

///**
//* \brief         
//* \param[in]     None
//* \return        void
//* \authors       Venkatanarayana Makam
//* \date          
//*/     
//
//int nConvert(ETYPE_BUS eBus, CString omStrCAPLFile, CString omStrCFile, CString omStrLogFile, CStringArray& omStrDbcFileArray, CDBCConverterBase* ouDBC2DBFConverter, CString& omStrResult)
//{
//    g_ouGlobalVariables.m_eBus = eBus;
//	g_bBusConnectGenerated = false;
//	g_bGlobalVariable = false;
//	g_MsgInitialize.clear();
//
//	if( NULL == ouDBC2DBFConverter && m_eBus == CAN)
//    {
//        omStrResult = "Loading Dbc Converter Failed";
//        return S_FALSE;
//    }
//
//    g_ouGlobalVariables.g_ouDBC2DBFConverter = ouDBC2DBFConverter;
//    cIndex2 = 0;
//    bPreStart = FALSE;
//	
////FirstParse Starts Here::Unsupported keys anf function definitions are processed
//    CString omStrTempFile = omStrCFile + ".temp";
//    yyin = fopen(omStrCAPLFile,"r");
//    yyout = fopen (omStrTempFile,"w+");
//	if (NULL == yyin || NULL == yyout)
//	{
//		return S_FALSE;
//	}
//    
//    BEGIN( FirstParse ); //start first parse
//    yylex();
//    
//    pFileLog = fopen( omStrLogFile,"w" );
//	if (NULL != pFileLog)
//	{
//		fprintf( pFileLog,defSTR_InputFileInfo, omStrCAPLFile );//writing to log file
//		fprintf( pFileLog,defSTR_OutputFileInfo, omStrCFile );
//	}
//
//    if( ouUnSptdKey.nGetSize() > 0)
//    {//if there is any unsuppoted key then
//        CEnvVarHandlerDlg d;
//        d.DoModal();
//    }
//	CString omTemp = "";
//	
//	//SecondParse Starts Here::
//    fseek(yyin, 0, SEEK_SET);
//    BEGIN(SecondParse);
//    yylex();
//    if( g_ouGlobalVariables.omStrLastEvent.GetLength() > 0)
//    {
//		if(g_ouGlobalVariables.omStrLastEvent == defSTR_GlobalVar)
//		{
//			omTemp = defSTR_UsingGlobalVarNameSpace;
//			omTemp += defSTR_GlobalVarEnd;
//		}
//		else
//		{
//			omTemp = defSTR_EndComment;
//		}
//		omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//		fprintf(yyout, "\n%s%s\n", (LPCSTR)omTemp, (LPCSTR)g_ouGlobalVariables.omStrLastEvent);
//        g_ouGlobalVariables.omStrLastEvent.Empty();
//    }
//
//    fclose( yyin );
//    fclose( yyout );
//   
////ThirdParse Starts Here::
//	yyin = fopen(omStrTempFile,"r");
//	yyout = fopen (omStrCFile,"w+");
//	if (NULL == yyin || NULL == yyout)
//	{
//		return S_FALSE;
//	}
//
//	BEGIN(ThirdParse);
//	yylex();
//	fclose( yyin );
//	fclose( yyout );
////Fourth Phase Starts Here::
//	yyin = fopen(omStrCFile ,"r");
//	yyout = fopen (omStrTempFile,"w+");
//	if (NULL == yyin || NULL == yyout)
//	{
//		return S_FALSE;
//	}
//
//	BEGIN(FourthParse);
//	yylex();
//	fclose( yyin );
//	fclose( yyout );
//
////Fifth Phase Starts Here::
//	yyin = fopen(omStrTempFile,"r");
//	yyout = fopen (omStrCFile,"w+");
//	if (NULL == yyin || NULL == yyout)
//	{
//		return S_FALSE;
//	}
//
//	BEGIN(FifthParse);
//	yylex();	
//	
//	fclose( yyin );
//	fclose( yyout);
//    DeleteFile(omStrTempFile);
//
////Include Headers,Wrapper,Utility Function definition, Function Declarations
//	CString omFun;
//    CString omStrTemp;
//    INT_PTR nCount = g_ouGlobalVariables.g_omStrFunCalls.GetSize();
//    for(INT_PTR i=0; i < nCount; i++)
//    {
//        omStrTemp = g_ouGlobalVariables.g_omStrFunCalls.GetAt(i);
//        bAddExport(omStrTemp);
//        omFun += omStrTemp+"\n";
//    }
//	CString omWrapperFun;
//	nCount = g_ouGlobalVariables.g_omStrFunWrappers.GetSize();
//	for(INT_PTR i=0; i < nCount; i++)
//    {
//        omWrapperFun += g_ouGlobalVariables.g_omStrFunWrappers.GetAt(i)+"\n";
//    }
//
//	std::fstream filestr;
//    filestr.open (omStrCFile.GetBuffer(MAX_PATH), fstream::in | fstream::out);
//    INT nDBCCount = omStrDbcFileArray.GetSize();
//    std::string str((std::istreambuf_iterator<char>(filestr)),
//                     std::istreambuf_iterator<char>());
//    filestr.seekp(0);
//		
//	filestr<<COPYWRITE_INFORMATION;
//	filestr<<C_FILE_VERSION;
//	filestr<<BUSMASTER_VERSION;    
//	omTemp = PROTOCOL;
//
//	if(eBus==CAN)
//	{
//		omTemp.Replace(defSTR_PlaceHolderForBusname, "CAN");
//	}
//	else
//	{
//		omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//	}
//    filestr<<omTemp; 
//	
//	filestr<<endl<<endl;
//	
//	omTemp = defSTR_StartIncludeHeader;
//	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//    filestr<<omTemp;
//	filestr<<"#include <windows.h>\n";
//	omTemp = defSTR_ProtocolIncludeHeader;
//	if(eBus==CAN)
//	{
//		omTemp.Replace(defSTR_PlaceHolderForBusname, "CAN");
//	}
//	else
//	{
//		omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//	}
//    filestr<<omTemp;
//	/*if(CAN==m_eBus)
//	{
//		for(INT_PTR i= 0; i < nDBCCount; i++)
//		{
//			CString omStrDbcFile = omStrDbcFileArray.GetAt(i);
//			INT nIndex = omStrDbcFile.ReverseFind('.');
//			if(nIndex >= 0)
//			{
//				omStrDbcFile = omStrDbcFile.Left(nIndex);
//				omStrDbcFile = omStrDbcFile + "_Unions.h";
//			}
//			filestr<<"#include \""<<omStrDbcFile<<"\"\n";
//		}
//	}*/
//	omTemp = defSTR_EndIncludeHeader;
//	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//    filestr<<omTemp<<endl;//footer for include header
//
//	omTemp = defSTR_FuncProHeader;
//	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//    filestr<<omTemp;
//    filestr.write(omFun.GetBuffer(MAX_PATH), omFun.GetLength());
//    if(FALSE == bPreStart)
//    {
//        filestr<<defUtils_PrestartDec;
//    }
//	if(true == bTimeStampNs && FLEXRAY==m_eBus)
//	{
//		filestr<<def_TimeNsDec;
//	}
//
//	if (CAN == m_eBus && g_bBusConnectGenerated == false && g_MsgInitialize.empty() == false)
//	{
//		filestr << defSTR_OnBusConnectDecl;
//	}
//
//	omTemp = defSTR_FuncProFooter;
//	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//    filestr<<omTemp<<endl;
//    filestr<<"\n";
//	
//	omTemp = defSTR_FuncProWrapperHeader;
//	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//    filestr<<omTemp;
//	filestr.write(omWrapperFun.GetBuffer(MAX_PATH), omWrapperFun.GetLength());
//    omTemp = defSTR_FuncProWrapperFooter;
//	omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//    filestr<<omTemp<<endl;
//
//
//    filestr<<defSTR_RXMessageDec<<defSTR_TXMessageDec;
//	filestr<<defSSTR_TYPEDEFFOR<<defSSTR_TYPEDEFSWITCH<<defSSTR_TYPEDEFIF<<endl;
//    filestr<<str;
//
//	if(FALSE == bPreStart)
//	{
//		omTemp = defUtils_StartComment;
//		omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//        filestr<<omTemp;
//        filestr<<defUtils_PrestartDef<<endl<<'}';
//		omTemp = defUtils_EndComment;
//		omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
//        filestr<<omTemp;
//	}
//	if(true == bTimeStampNs && FLEXRAY == m_eBus)
//	{
//		filestr<<def_TimeNsDef<<endl;
//		
//	}
//
//	if (CAN == m_eBus && g_bBusConnectGenerated == false && g_MsgInitialize.empty() == false)
//	{
//		// Generating Bus Connect if Msg Handlers are initialized
//		std::string omBusConnect = "";
//
//		CString omTemp = "";
//		omTemp = defSTR_Header;
//		omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[m_eBus].c_str());
//
//		omBusConnect += omTemp + defSTR_OnBusConnectHeder;
//		omBusConnect += defSTR_OnBusConnect + g_MsgInitialize;
//
//		omTemp = defSTR_Footer;
//		omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[m_eBus].c_str());
//		omTemp +=  defSTR_OnBusConnectHeder;
//
//		omBusConnect += omTemp;
//
//		filestr << omBusConnect << endl;
//	}
//    fclose( yyin );
//    
////Result File Generation
//    vWriteLogFile();
//	if (NULL != pFileLog)
//	{
//		fclose( pFileLog );
//	}
//
////Return Value Updation
//    if( (g_ouGlobalVariables.g_ouErrorStrings.size() == 0) && g_ouGlobalVariables.g_ouWarningStrings.size() == 0)
//    {
//        omStrResult.Format("Conversion Completed Successfully");
//    }
//    else
//    {
//        omStrResult.Format("Conversion is Completed With  %d Error(s) and %d Warning(s)", g_ouGlobalVariables.g_ouErrorStrings.size(), g_ouGlobalVariables.g_ouWarningStrings.size());
//    }
//
//    /* for(int i = 0; i< g_ouGlobalVariables.g_ouGlobalMsgVariables.GetCount(); i++)
//    {
//        MsgVariables &ouVariable= g_ouGlobalVariables.g_ouGlobalMsgVariables.GetAt(i);
//        MessageBox(NULL, ouVariable.m_omStrMsgType, ouVariable.m_omStrMsgName, MB_OK);
//    }*/
//    return 0;
//
//}
void vWriteLogFile()
{
	if (NULL == pFileLog)
	{
		return;
	}

    if( (g_ouGlobalVariables.g_ouErrorStrings.size() == 0) && g_ouGlobalVariables.g_ouWarningStrings.size() == 0)
    {
        fprintf(pFileLog, "\n\n\nConversion Completed Successfully\n");
    }
    else
    {
        fprintf(pFileLog, "\n\n\nConversion is Completed With  %Iu Error(s) and %Iu Warning(s)\n", (size_t)g_ouGlobalVariables.g_ouErrorStrings.size(), (size_t)g_ouGlobalVariables.g_ouWarningStrings.size());

        if( g_ouGlobalVariables.g_ouErrorStrings.size() >= 1)
        {
            fprintf(pFileLog, "\n\tErrors Occured in Conversion:(%Iu Error(s)) \n", (size_t)g_ouGlobalVariables.g_ouErrorStrings.size());
            set<string>::iterator ErrorItr = g_ouGlobalVariables.g_ouErrorStrings.begin();
            while( ErrorItr != g_ouGlobalVariables.g_ouErrorStrings.end())
            {
                // The unique data. Use it.
                string UniqueFilePath = *ErrorItr;
                fprintf(pFileLog, "\t\t%s\n", UniqueFilePath.c_str());
                // Increment to next item in set.
                ++ErrorItr;
            }
        }
        if( g_ouGlobalVariables.g_ouWarningStrings.size() >= 1)
        {
            fprintf(pFileLog, "\n\tWarnings Occured in Conversion:(%Iu Warning(s)) \n", (size_t)g_ouGlobalVariables.g_ouWarningStrings.size());
            set<string>::iterator WarningsItr = g_ouGlobalVariables.g_ouWarningStrings.begin();
            while( WarningsItr != g_ouGlobalVariables.g_ouWarningStrings.end())
            {
                // The unique data. Use it.
                string UniqueFilePath = *WarningsItr;
                fprintf(pFileLog, "\t\t%s\n", UniqueFilePath.c_str());
                // Increment to next item in set.
                ++WarningsItr;
            }
        }
    }
}
BOOL bAddExport(CString& omFun)
{ 
	omFun.TrimLeft(" ");
	omFun.TrimRight(" ");
    INT_PTR nIndex = omFun.Find(defSTF_Extern);
    if(nIndex >= 0)
    {
        nIndex = nIndex + strlen(defSTF_Extern);
        nIndex = omFun.Find(" ", nIndex);
        if(nIndex >= 0)
        {
            omFun.Insert((INT)nIndex, defSTF_Export);
        }
    }
    return TRUE;
}
/**
* \brief         This will convert the input can file into C file.This will
                 check whether the input file is correct or not.If name of
                 output file is not there then it will  create a default
                 output file with the name of input file with extention C.
                 The log file will be created with default name of input file 
                 withe extention "txt".This function will intialize acAltkey
                 with -1.
* \param[in]     None
* \return        void
* \authors       Amit Ranjan
* \authors       Venkanarayana Makam
* \date          
*/     
void vMsgDecl(CString& omStrValue, MsgVariables& ouMsgVariable, bool bGlobal)
{
    try
    {
    CString val , ident , name ;
    bool bSpecialMsg = false;
    int nChannel = 1;
    int start  ;
    char extndd = 0;
    CMessage msg;
    int flag = 0;
    unsigned int decid = 0;
    if( omStrValue[0] == ';')
    {
        fprintf(yyout,";");
    }
    val = omStrValue;
    CString omStrTemp;
    val.TrimLeft();
    val = val.Right( val.GetLength() - 7 );
    start = val.Find(' ',0);
    val = val.Right(val.GetLength() - start  );

    val.TrimLeft();
    

    
    start = val.Find(' ',0 );
    ident = val.Left( start );//id or name of message

    val = val.Right( val.GetLength() - start );
    val.TrimLeft();
    start = val.FindOneOf(" ;=");

    name = val.Left( start );//name of the variable
    val = val.Right( val.GetLength() - start );
    
    if( ( start = ident.Find('.',0) ) != -1 )
    {//for "can1.asc" like cases
        CString strCAN = ident.Left(start);
        ident = ident.Right( ident.GetLength() - start - 1 );
        int nStart = strCAN.Find("CAN");
        if(  nStart >= 0 )
        {
            nChannel = atoi(strCAN.Right(strCAN.GetLength() - nStart - 3).GetBuffer(MAX_PATH));
        }
        if(ident[0] == '*')
        {
            int nIndex = omStrValue.Find(".*");
            if(nIndex > 0)
            {
                ident = omStrValue.Right(omStrValue.GetLength() - nIndex - 2); 
                ident.TrimLeft(" ");
				ident.TrimRight(" ");
                bSpecialMsg = true;
            }
        }
    }

    if( ident[0] == '\'')
    {
        ident.TrimLeft("' ");
		ident.TrimRight("' ");
        decid = ident[0];
        flag = 1;
        g_ouGlobalVariables.g_ouDBC2DBFConverter->FindMessage(decid, &msg);
    }
    else if( isdigit( ident[0]) )
    {//if message is declared with id then
        flag = 1;
        
        if( ident[ ident.GetLength() - 1 ] =='x'||
            ident[ ident.GetLength() - 1 ] =='X')
        {//if last element is 'x'
            extndd = 1;
            ident = ident.Left( ident.GetLength() - 1 );
        }
        int nIndex = ident.Find("0x");
        if( nIndex == -1 )
        {
            nIndex = ident.Find("0X");
        }
        if( nIndex != -1 )
        {
            nIndex +=2;
            // remove 0x
            char *stop;
            ident = ident.Right(ident.GetLength() - nIndex);
            decid = strtol(ident,&stop,16);
        }
        else
        {//if deci id
            decid = atoi(ident);
        }
        //g_ouGlobalVariables.g_ouDBC2DBFConverter->FindMessage(decid, msg);
        //Not to use database messages
        msg.m_uiMsgID = INVALID;
    }
    if(ident[0] == '*')
    {
        
    }
    else if(g_ouGlobalVariables.g_ouDBC2DBFConverter!=nullptr)
    {//message is declared with name
		g_ouGlobalVariables.g_ouDBC2DBFConverter->FindMessage(ident.GetBuffer(MAX_PATH),&msg);
    }
    
   // ouMsgVariable.m_omStrMsgType = msg.m_acName;    
    
    if( val.Find('{',0) == -1 )
    {//if message is not initialized
        if( flag == 0 )
        {
			bReplaceBMFunctionName(name);
			ident.TrimLeft(" \n\t");
			ident.TrimRight(" \n\t");
			name.TrimLeft(" \n\t");
			name.TrimRight(" \n\t");
            if(false == bSpecialMsg)
            {
				if(ident.Compare(name) == 0)
				{
					g_ouGlobalVariables.g_omStrRepeatedMsg.AddTail(ident);

					fprintf(yyout, defSTR_RepeatedMsgFormat, (LPCSTR)ident, (LPCSTR)name);

					char chData[255];
					std::string omData;
					sprintf(chData, defSTR_MSGInitialize, (LPCSTR)name, msg.m_uiMsgID, msg.m_cFrameFormat == 'S' ? 0 : 1, 
						msg.m_ucLength, 1 );
					omData = chData;
					omData += ");\n";
					//fprintf(yyout, ");");
					if (true == bGlobal)
					{
						g_MsgInitialize += omData;
					}
					else
					{
						fprintf(yyout, "%s", omData.c_str());
					}
					
					/*fprintf(yyout,defSTR_RepeatedMsgFormat, (LPCSTR)ident, (LPCSTR)name,
                    msg.m_uiMsgID,msg.m_cFrameFormat == 'S'? 0 : 1,
                    msg.m_ucLength >0? 0:1 ,msg.m_ucLength, 1 ); */

				}
				else
				{				
					fprintf(yyout, defSTR_MsgFormat, (LPCSTR)ident, (LPCSTR)name);

					char chData[255];
					std::string omData;
					sprintf(chData, defSTR_MSGInitialize, (LPCSTR)name, msg.m_uiMsgID, msg.m_cFrameFormat == 'S' ? 0 : 1,
						msg.m_ucLength, 1);
					omData = chData;
					omData += ");\n";
					if (true == bGlobal)
					{
						g_MsgInitialize += omData;
					}
					else
					{
						fprintf(yyout, "%s", omData.c_str());
					}

					/*,
                    msg.m_uiMsgID,msg.m_cFrameFormat == 'S'? 0 : 1,
                    msg.m_ucLength >0? 0:1 ,msg.m_ucLength, 1 ); */

				}
                if( msg.m_uiMsgID == INVALID )
                {//means unknown message
                    sprintf(g_chErrorString, "(Message Not Found) Message : %s not found in database.", (LPCSTR)ident);
                    g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);
                    nUnCnvrtdLine += 1;
                    // to decrease the percentage of conversion
                }
            }
            else
            {
				//bReplaceBMFunctionName(ident);
                fprintf(yyout,defSTR_ChanelSpecificMsg, (LPCSTR)ident); 
				sprintf(g_chErrorString, "Channel Specific (message CAN1.*) Message Types are not supported yet.", (LPCSTR)ident);
                g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);
            }
            ouMsgVariable.m_omStrMsgType = ident;
            ouMsgVariable.m_omStrMsgName = name;
        }

        else
        {
            flag = 0;
            if( msg.m_uiMsgID != INVALID )
            {//means id is in database
				if(name = msg.m_acName.c_str())
				{
					fprintf(yyout, defSTR_MsgFormat, msg.m_acName.c_str(), (LPCSTR)name);
					/*,
						msg.m_uiMsgID,extndd,msg.m_ucLength >0? 0:1,
						msg.m_ucLength, 1 );*/
				}
				else
				{
					fprintf(yyout, defSTR_RepeatedMsgFormat, msg.m_acName.c_str(), (LPCSTR)name);
					/*,
						msg.m_uiMsgID,extndd,msg.m_ucLength >0? 0:1,
						msg.m_ucLength, 1 );*/
					g_ouGlobalVariables.g_omStrRepeatedMsg.AddTail(msg.m_acName.c_str());
				}
                ouMsgVariable.m_omStrMsgType = msg.m_acName.c_str();
                ouMsgVariable.m_omStrMsgName = name;
            }
            else
            {//means it is an unknown message
				fprintf(yyout, defSTR_STCANMsgFormat, (LPCSTR)name);
				//, (LPCSTR)name, decid, extndd);

				char chData[255];
				std::string omData;
				sprintf(chData, defSTR_MSGInitialize, (LPCSTR)name, decid, extndd, 0, 1);
				omData = chData;
				omData += ");\n";
				if (true == bGlobal)
				{
					g_MsgInitialize += omData;
				}
				else
				{
					fprintf(yyout, "%s", omData.c_str());
				}
            }
        }
    }
    else
    {//if message is initialized
        int  end = 0 ;
        char dlc = 8;//to extract dlc

        CStringArray csa , msgbyte;
        // msgbyte.SetSize(8); //***anish
        int i;
        for ( i = 0; i< 8; i++)
        {//initialize array with '0'
            msgbyte.SetAtGrow(i,"0");
        }

        start = val.Find('{',0);
        val = val.Right( val.GetLength() - start - 1);
        start = 0;
        while((end = val.Find(",",start)) != -1 )
        {
            CString cs = val.Mid(start, end - start);
            csa.Add(cs);
            start = end + 1;
        }
        csa.Add(val.Right( val.GetLength() - start  ));//start - 1
        for(  i = 0; i < csa.GetSize(); i++)
        {
            CString cs = csa.GetAt( i );//byte and value
            CString key, value;
            cs.TrimLeft();
            cs.TrimRight();
            int pos = cs.Find("=",0);

            key = cs.Left(pos);
            value = cs.Right(cs.GetLength() - pos - 1 );
            key.TrimLeft();
            key.TrimRight();
            key.MakeLower();//for case insensitive comparision
            value.TrimLeft();

            if( ( start =value.FindOneOf("\t\n}") ) != -1 )
            {
                value = value.Left( start );
            }
            if(key == "dlc")
            {
                dlc = atoi(value);//since val is CString so atoi is used
            }


            if(cs.Find(defSTR_byte,0) != -1)
            {//to store the value of particular byte of message
                CString index;
                start = key.Find('(',4);
                end = key.Find(')',6);
                index = key.Mid( start + 1, end - 2 );
                index.TrimLeft();
                index.TrimRight();
                start = atoi( index );//for comparision

                for( int i = 0;i <= 7; i++)
                {
                    if( i == start )
                    {
                        msgbyte.SetAtGrow(i, value);
                        break;
                    }
                }
            }//end if if(byte)

            if(cs.Find(defSTR_word,0) != -1)
            {
                CString index;
                int flag = 0;
                if ( value[0]=='0'&& (value[1]=='x'||value[1]=='X') )
                {
                    value = value.Right( value.GetLength() - 2 );
                    flag = 1;
                }
                start = key.Find('(',4);
                end = key.Find(')',6);
                index = key.Mid( start + 1, end - 2 );
                index.TrimLeft();
                index.TrimRight();
                start = atoi( index );//for comparision

                if ( flag == 1 )
                {
                    CString hex = "0x";
                    msgbyte.SetAtGrow( start*2, hex + value.Left(2) );
                    msgbyte.SetAtGrow( start*2 + 1, hex + value.Right(2) );
                    flag = 0;
                }
                else
                {
                    msgbyte.SetAtGrow( start*2, value.Left(2) );
                    msgbyte.SetAtGrow( start*2 + 1, value.Right(2) );
                }
            }//end of word
            if( cs.Find(defSTR_long,0) != -1)
            {
                CString index;
                int flag = 0;
                if ( value[0]=='0'&& (value[1]=='x'||value[1]=='X') )
                {
                    value = value.Right( value.GetLength() - 2 );
                    flag = 1;
                }
                start = key.Find('(',4);
                end = key.Find(')',6);
                index = key.Mid( start + 1, end - 2 );
                index.TrimLeft();
                index.TrimRight();
                start = atoi( index );//for comparision

                if( start == 0 )
                {
                    if ( flag == 1 )
                    {//if declared as hex
                        CString hex = "0x";
                        msgbyte.SetAtGrow(0, hex + value.Left(2) );
                        msgbyte.SetAtGrow(1, hex + value.Mid(2,2) );
                        msgbyte.SetAtGrow(2, hex + value.Mid(4,2) );
                        msgbyte.SetAtGrow(3, hex + value.Right(2) );
                        flag = 0;
                    }
                    else
                    {//if declared as dec
                        msgbyte.SetAtGrow(0,value.Left(2) );
                        msgbyte.SetAtGrow(1,value.Mid(2,2) );
                        msgbyte.SetAtGrow(2,value.Mid(4,2) );
                        msgbyte.SetAtGrow(3,value.Right(2) );
                    }
                }
                else
                {//if index  = 1
                    if ( flag == 1 )
                    {//if declared as hex
                        CString hex = "0x";
                        msgbyte.SetAtGrow(4, hex + value.Left(2) );
                        msgbyte.SetAtGrow(5, hex + value.Mid(2,2) );
                        msgbyte.SetAtGrow(6, hex + value.Mid(4,2) );
                        msgbyte.SetAtGrow(7, hex + value.Right(2) );
                        flag = 0;
                    }
                    else
                    {//if declared as dec
                        msgbyte.SetAtGrow(4,value.Left(2) );
                        msgbyte.SetAtGrow(5,value.Mid(2,2) );
                        msgbyte.SetAtGrow(6,value.Mid(4,2) );
                        msgbyte.SetAtGrow(7,value.Right(2) );
                    }
                }
            }//end of long
        }
        if( flag == 0 )
        {
            if( msg.m_uiMsgID == INVALID )
            {
                sprintf(g_chErrorString, "(Message Not Found) Message : %s not found in database.", (LPCSTR)ident);
                g_ouGlobalVariables.g_ouErrorStrings.insert(g_chErrorString);
            }

			ouMsgVariable.m_omStrMsgType = ident;
            ouMsgVariable.m_omStrMsgName = name;

			fprintf(yyout, defSTR_MsgIniFormat, (LPCSTR)ident, (LPCSTR)name);

			char chData[255];
			std::string omData;
			sprintf(chData, defSTR_MSGInitializeData, (LPCSTR)name, msg.m_uiMsgID, msg.m_cFrameFormat == 'S' ? 0 : 1, dlc, 1);
			omData = chData;
			for (int m = 0; m < dlc; m++)
			{
				//sprintf(chData1, "%s", (LPCSTR)msgbyte.GetAt(m));//each byte of message
				omData += (LPCSTR)msgbyte.GetAt(m);
				if (m < dlc - 1)
				{
					omData += ",";
					//sprintf(chData1, ",");
				}
			}

			omData += ");\n";
			//fprintf(yyout, ");");
			if (true == bGlobal)
			{
				g_MsgInitialize += omData;
			}
			else
			{
				fprintf(yyout, "%s", omData.c_str());
			}
				//(LPCSTR)name, msg.m_uiMsgID, msg.m_cFrameFormat == 'S' ? 0 : 1, dlc);

			 //fprintf(yyout,defSTR_MsgIniFormat, (LPCSTR)ident, (LPCSTR)name,
				//(LPCSTR)name, msg.m_uiMsgID, msg.m_cFrameFormat == 'S' ? 0 : 1, dlc);

    //        for( int  m = 0; m < dlc; m++)
    //        {
				//fprintf(yyout,"%s", (LPCSTR)msgbyte.GetAt( m ));//each byte of message
    //            if(m < dlc - 1)
    //            {
    //                fprintf(yyout,",");
    //            }
    //        }
    //        fprintf(yyout,");");
        }
        else
        {
            flag = 0;
            if( msg.m_uiMsgID != INVALID )
            {
                fprintf(yyout,defSTR_MsgIniFormat,msg.m_acName.c_str(), (LPCSTR)name/*,
                    msg.m_uiMsgID,extndd,dlc*/ );
                for( int  m = 0; m < dlc; m++)
                {
					fprintf(yyout,"%s", (LPCSTR)msgbyte.GetAt( m ));//each byte of message
                    if(m < dlc - 1)
                    {
                        fprintf(yyout,",");
                    }
                }
                fprintf(yyout,"}};");
            }
            else
            {
				//fprintf(yyout,"\nSTCAN_MSG %s = {0x%x,%d,0,%d,1,"
    //                ,(LPCSTR)name, decid,extndd,dlc);

    //            for( int  m = 0; m < dlc; m++)
    //            {
    //                fprintf(yyout,"%s", (LPCSTR)msgbyte.GetAt(m ));//each byte of message
    //                if(m < dlc - 1)
    //                {
    //                    fprintf(yyout,",");
    //                }
    //            }
				//fprintf(yyout,"};");

				fprintf(yyout, defSTR_MsgIniFormat, "STCAN_MSG", (LPCSTR)name);

				char chData[255];
				std::string omData;
				sprintf(chData, defSTR_MSGInitializeData, (LPCSTR)name, decid, extndd, dlc, 1);
				omData = chData;
				for (int m = 0; m < dlc; m++)
				{
					//fprintf(yyout,"%s", (LPCSTR)msgbyte.GetAt(m ));//each byte of message
					omData += (LPCSTR)msgbyte.GetAt(m);
					if (m < dlc - 1)
					{
						omData += ",";
						//sprintf(chData1, ",");
					}
				}

				omData += ");\n";
				//fprintf(yyout, ");");
				if (true == bGlobal)
				{
					g_MsgInitialize += omData;
				}
				else
				{
					fprintf(yyout, "%s", omData.c_str());
				}
            }
        }
    }

    
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vMsgDecl\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}
void SearchForRepeatedMessage()
{
	CString omStrName = yytext;
	POSITION pos = g_ouGlobalVariables.g_omStrRepeatedMsg.Find(omStrName);
	if(NULL != pos)
	{
		fprintf(yyout, "BM_%s", omStrName.GetBuffer(MAX_PATH));
	}
	else if(omStrName == "SendMsg")
	{
		fprintf(yyout, "BM_%s", omStrName.GetBuffer(MAX_PATH));
	}
	else
	{
		ECHO_INPUT;
	}
}
void vUpdateArraySize()
{
	CString omStrText = yytext;
	int nIndexEnd, nIndexStart;
	nIndexEnd = omStrText.ReverseFind(']');
	if(nIndexEnd > 0 )
	{
		nIndexStart = omStrText.ReverseFind('[');
		if(nIndexStart >=0 && nIndexStart < nIndexEnd )
		{
			CString omStrSize = omStrText.Mid(nIndexStart+1, nIndexEnd - nIndexStart-1);
			omStrText.Replace(omStrSize, "MAX_PATH");
		}
	}
	fprintf(yyout, "%s", omStrText.GetBuffer(MAX_PATH));
}


void bReplaceBMFunctionName(CString omStrName)
{
	omStrName.TrimLeft(" \n\t");
	omStrName.TrimRight(" \n\t");
	if(omStrName.Find("SendMsg") >= 0)	//TODO::List Has to Update
	{
		g_ouGlobalVariables.g_omStrRepeatedMsg.AddTail("SendMsg");
	}
}

