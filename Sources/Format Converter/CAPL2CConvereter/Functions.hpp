/*******************************************************************************
  Project       :  Language Converter
  FileName      :  Functions.hpp
  Description   :  Implementation file for parsing functions
  $Log:   Y:/EMC2/Projects/CAN_Monitor_Maintenance/Archives/Source/LanguageConverter/Functions.hpv  $
   
      Rev 1.0   17 Aug 2005 13:46:12   amb2kor
   Initial Version.
  Author        :  Amit Ranjan
  Date Created  :  21.04.2004
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

#include "StdAfx.h"
#ifndef _LEXER_H
#define _LEXER_H
#define INVALID 0xffffffff
#include "lexer.h"
#endif
#include "ConstString.h"
#include "Selectdb.h"
#include "Converter.h"
#include "Message.h"
#include "Signal.h"
#include "ValueDescriptor.h"


/*******************************************************************************
 Function Name    : vKeyStoreHeader
 Input(s)         :     -
 Output           :     -
 Functionality    : This will extract the name of the key from yytext. If key is
                    supported by BUSMASTER it will be stored in" acSptdKey " else 
                    it will be stored in " acUnSptdKey ".
                    
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vKeyStoreHeader()
{
    //yytext-- /*@@key:'a':*/   or /*@@key:pageup:*/
    try
    {
    char acKeyName[defSTR_MaxSizeBreadth];
    int m = 8; //length of /*@@key:
    int n = 0;
    
    if( yytext[8] == '\'' )//if yytext is not /*@@key:*:*/
    {
        // Single char  
        if( !isalnum( yytext[9] ) )//if not alpha numeric
        {
            char cTemp[2]; 
            cTemp[0] = yytext[9];
            cTemp[1] = 0;
            ouUnSptdKey.bAdd(cTemp);
        }
        else
        { //if alpha numeric
            acSptdKey[cIndex2++] = yytext[9];
        }
    }
    else
    {
        n = 0;
        m = 8;
        if( yytext[m] != '*')
        {
            while( yytext[m] != NULL && yytext[m] != ':')
            {
                acKeyName[n++] = yytext[m++];
            }
            acKeyName[n] = 0;
            ouUnSptdKey.bAdd(acKeyName);
            
        }
        // else if it is '*' it will be considered in the second parse
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

/********end of function*******************************************************/

/*******************************************************************************
 Function Name    : vEnvVar
 Input(s)         :     -
 Output           :     -
 Functionality    : This will extract the name of the Environment Handler from 
                    yytext and store it into " auUnSptdKey ".
                    
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vEnvVar()   
{
    //yytext--/*@@envVar:.*:*/
    try
    {
    CString val ;
    int start = 0 ;
    val = yytext ;
    start = val.Find(':',0);
    val = val.Right( val.GetLength() - start - 1 );// to avoid "/*@@envVar:"
    start = val.Find(':',0);
    val = val.Left( start  );
    val.TrimLeft();
    val.TrimRight();
    ouUnSptdKey.bAdd( val );
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vEnvVar\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
    
}

/********************end of function*******************************************/

/*******************************************************************************
 Function Name    : vCaplFunc
 Input(s)         :     -
 Output           :     -
 Functionality    : This will extract the name of the capl function from 
                    yytext and store it into " ouFuncName ".
                    
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vCaplFunc()
{
    //yytext -- /*@@caplFunc:abc(para1,...):*/
    try
    {
    int start ;
    CString val ;
    val = yytext;
    start = val.Find(':',0);
    val = val.Right( val.GetLength() - start - 1 );// to avoid "/*@@caplFunc:"
    
    start = val.Find('(',0);
    val = val.Left( start );//name of function
    val.TrimLeft();
    val.TrimRight();
    ouFuncName.bAdd (val);
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vCaplFunc\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }

}

/**********************end of function*****************************************/

/*******************************************************************************
 Function Name    : vKeyStoreCase
 Input(s)         :     -
 Output           :     -
 Functionality    : This will extract the name of the key from yytext.If the
                    length is more than one then key will be compared with the 
                    menbers of UnSptdKeyList.If it will match then key will be
                    stored in " acUnSptdKey ".Before storing it is checked that 
                    whether the key is already existing in the acUnSptdKey array
                    or not.If it is not there then only the key will be stored.
                    
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 27.04.2004
 Modifications    :
*******************************************************************************/
void vKeyStoreCase()
{
    //yytext -- case .* :
    try
    {
    CString keyname;
    int start ;

    static  char* UnSptdKeyList[] ={    "pageup","pagedown","end","home","f1",
        "f2","f3","f4","f5","f6","f7","f8","f9","f10","f11","f12","ctrlpageup",
        "ctrlpagedown","ctrlhome","ctrlend","ctrlf1","ctrlf2","ctrlf3","ctrlf4",
        "ctrlf5","ctrlf6","ctrlf7","ctrlf8","ctrlf9","ctrlf10","ctrlf11",
        "ctrlf12","CursorDown","CursorUp","CursorRight","CursorLeft",
        "ctrlCursorDown","ctrlCursorUp","ctrlCursorLeft","ctrlCursorRight"};

    keyname = yytext;

    start = keyname.FindOneOf("cC");
    keyname = keyname.Right( keyname.GetLength() - start );

    start = keyname.Find(' ',0);
    keyname = keyname.Right( keyname.GetLength() - start - 1 );

    keyname.TrimLeft();
    keyname = keyname.Left( keyname.GetLength() - 1 );
    keyname.TrimRight();

    if( keyname[0] =='\'' )
    {
        keyname = keyname.Mid(1,keyname.GetLength() - 1);
    }

   
    if( keyname.GetLength() > 1 ) //if length of key is more than one
    {
        for (int i = 0;i < 40; i++)
        {
            if( _stricmp(UnSptdKeyList[i],keyname) == 0 )
            {
                ouUnSptdKey.bAdd(keyname);//add in the array
                i = 40 ;//to terminate the loop
            }
        }
    }
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vKeyStoreCase\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
    
}   

/**************************end of function*************************************/

/*******************************************************************************
 Function Name    : vHeader
 Input(s)         :     -
 Output           :     -
 Functionality    : This will take care of header and footer.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/ 
void vHeader()
{
    try
    {
    CString val;
    CString HandlerType, HandlerVal;
    val = yytext;
    
    // yytext -- "/*@@var:*/" /*@@key:'a':*/
    if( val == "/*@@var:*/" )
    {
        // yytext -- /*@@var:*/
        fprintf(yyout,defSTR_StartIncludeHeader);//header for include header
        fprintf(yyout,"#include<windows.h>\n");
        fprintf(yyout,"#include<struct.h>\n");
        fprintf(yyout,defSTR_EndIncludeHeader);//footer for include header
        fprintf(yyout,defSTR_GlobalVarHeader);//header for global variables
        
        fprintf(yyout,defSTR_TypedefByte);
        fprintf(yyout,defSTR_TypedefWord);
        fprintf(yyout,defSTR_DefRx);
        fprintf(yyout,defSTR_ForTrace, defSTR_TraceBufferName);// for Trace

        nSourceLine = nSourceLine + 6;//2 lines have been increased by other func
        cFlagHeader = 1; //flag for DelForHeader state
        
    }
    else
    {//if not vaiable declaration part then
        
        int start;
        val = val.Left( val.GetLength() - 2);//remove "*/"
        val = val.Right( val.GetLength() - 4);//remove"/*@@"

        start = val.Find(':',0);
        HandlerType = val.Left( start  );
        
        val = val.Right( val.GetLength() - start - 1);
        HandlerVal = val.Left( val.GetLength() - 1 );
        
        fprintf(yyout,defSTR_Header);
        
        if( HandlerType == defSTR_timer )
        {//timer handler
            Footer = defSTR_OnTimer ;
            Footer += HandlerVal + defSTR_TimerFormat;
            
            fprintf(yyout,Footer);
            fprintf(yyout,defSTR_TimerStart,HandlerVal);
        }//end of timer
    
        if( HandlerType == defSTR_startstart )
        {
            Footer = defSTR_DllLoadHeader;
            fprintf(yyout,Footer);
            fprintf(yyout,defSTR_DllLoadStart);

            if( fprestart == 1 )
            {
                fprintf(yyout,"\nUtils_PreStart();");
                nSourceLine += 1;//we are adding one more line
            }
        }//end of dll load

        if( HandlerType == "preStart" )
        {
            if( fstart == 0 )
            {//if dll load is not in the program
                Footer = defSTR_DllLoadHeader;
                fprintf(yyout,Footer);
                fprintf(yyout,defSTR_DllLoadStart);
            }
            else
            {//if dll load is there then replace it with utility function
                Footer = "Utils_PreStart */\n";
                fprintf(yyout,Footer);
                fprintf(yyout,"void Utils_PreStart()\n{");
                
            }

        }//end of prestart

        if(  HandlerType == "stop" ) 
        {
            Footer = defSTR_DllUnLoadHeader;
            fprintf(yyout,Footer);
            fprintf(yyout,defSTR_DllUnLoadStart);
        }//end of dll unload

        if( HandlerType == defSTR_busoff )
        {
            Footer = defSTR_BusOffHeader;
            fprintf(yyout,Footer);
            fprintf(yyout,defSTR_BusOffStart);
        }//end of busoff

        if( HandlerType == defSTR_caplfunc  )
        {
            Footer = defSTR_Utils;
            int start;
            start = HandlerVal.Find('(',0);
            if( start == -1 )
            {
                MessageBox(0,"Header of caplFunc Handler is wrong","Warning",MB_OK);
                exit( 0 );
            }
            HandlerVal = HandlerVal.Left( start );
            Footer += HandlerVal + defSTR_HeaderFormat;
            fprintf(yyout,Footer);
        }//end of capl function

        if( HandlerType == defSTR_errorframe  )
        {
            Footer = defSTR_ErrorFrameHeader;
            fprintf(yyout,Footer);
            fprintf(yyout,defSTR_ErrorFrameStart);
        }//end of errorframe

        if( HandlerType == defSTR_erroractive )
        {
            Footer = defSTR_ErrorActiveHeader;
            fprintf(yyout,Footer);
            fprintf(yyout,defSTR_ErrorActiveStart);
        }//end of erroractive

        if( HandlerType == defSTR_errorpassive  )
        {
            Footer = defSTR_ErrorPassiveHeader;
            fprintf(yyout,Footer);
            fprintf(yyout,defSTR_ErrorPassiveStart);
        }//end of errorpassive

        if( HandlerType == defSTR_warninglimit  )
        {
            Footer = defSTR_WarningLimitHeader;
            fprintf(yyout,Footer);
            fprintf(yyout,defSTR_WarningLimitStart);
        }//end of warninglimit

        if(HandlerType == defSTR_key|| HandlerType == defSTR_envvar)
        {//for key handler and environment handler
            cFlagHeader = 3;//to be used to convert "this"
            if(HandlerType == defSTR_envvar )
            {//log entry for environment handler
                fprintf(logfile,defSTR_EnvVarLogEntry,nSourceLine );
            }
            if( yytext[8] == '*' )
            {//for Key_All
                Footer = defSTR_KeyAllHeader;
                fprintf(yyout,Footer);
                fprintf(yyout,defSTR_KeyAllStart);
            }
            else
            {//check for unsupported kay
                if( HandlerVal[0] =='\'')
                {
                    HandlerVal = HandlerVal.Right( HandlerVal.GetLength() - 1 );
                    HandlerVal = HandlerVal.Left( HandlerVal.GetLength() - 1);
                }
                int index = ouUnSptdKey.nFind( HandlerVal );
                if( index == -1)
                {//means supported key
                    Footer = defSTR_KeyCharHeader;// + HandlerVal + " */\n";
                    Footer += HandlerVal + defSTR_HeaderFormat;
                    fprintf(yyout,Footer);
                    fprintf(yyout,defSTR_KeyCharStart,HandlerVal);
                }
                else
                {//means unsupported key
                    Footer = defSTR_KeyCharHeader;
                    if(  acAltKey[index] == -1)
                    { // if no alternate key is available
                        //array is initialised with -1
                        Footer += HandlerVal + defSTR_HeaderFormat;
                        fprintf(yyout,Footer);
                        fprintf(yyout,defSTR_KeyCharStart,HandlerVal);
                        fprintf(logfile,defSTR_Warning7,HandlerVal,nSourceLine);
                        nUnCnvrtdLine = nUnCnvrtdLine + 2;//wighted 2

                    }
                    else
                    {
                        Footer += CString( acAltKey[index])+defSTR_HeaderFormat;
                        fprintf(yyout,Footer);
                        fprintf(yyout,defSTR_KeyCharStart,
                            CString( acAltKey[index] ));
                        fprintf(logfile,defSTR_ConvertedkeyInfo,HandlerVal,
                            nSourceLine,acAltKey[index]);
                    }
                }
            }
        }//end of keyhandler
    
        if( HandlerType == defSTR_msg )
        {  //for message handler
            cFlagHeader = 4;//to be used to convert "this"
            if(yytext[8]=='*')
            {
                Footer = defSTR_OnMsgAllHeader;
                fprintf(yyout,Footer);
                fprintf(yyout,defSTR_OnMsgAllStart);
            }
            else if( !isdigit( HandlerVal[0] ) )
            {//for message name
                start = HandlerVal.FindOneOf("' ':");
                if( start != -1 )
                {
                    HandlerVal = HandlerVal.Left( start );
                }
                Footer = defSTR_MsgNameHeader;
                Footer += HandlerVal + defSTR_HeaderFormat;
                fprintf(yyout,Footer);
                fprintf(yyout,defSTR_MsgNameStart,HandlerVal,HandlerVal);
                //we have replaced "STCAN_MSG" with message name in this case
            }
            else 
            {
                if( HandlerVal[ HandlerVal.GetLength() - 1] == 'x' ||
                     HandlerVal[ HandlerVal.GetLength() - 1] == 'X' )
                {//avoid x for extended
                    HandlerVal = HandlerVal.Left( HandlerVal.GetLength() - 1 );
                }
                start = HandlerVal.Find('-',0);

                if ( start == -1 )
                {//means id
                    Footer = defSTR_MsgIdUs ;
                    if( HandlerVal[1] =='x' || HandlerVal[1] =='X')
                    {
                        HandlerVal = HandlerVal.Right( HandlerVal.GetLength() - 2);
                    }
                    else
                    {
                        char *stop;
                        
                        HandlerVal.Format("%x",strtol( HandlerVal, &stop, 10)) ;
                        
                    }

                    Footer += HandlerVal + defSTR_HeaderFormat;
                    fprintf(yyout,Footer);
                    fprintf(yyout,defSTR_MsgIdStart,HandlerVal);
                }
                else
                {//means msgrange
                    Footer = defSTR_MsgIdRangeHeader;
                    CString id;
                    id = HandlerVal.Left( start );
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
                    Footer += id + '_';
                    id = HandlerVal.Right( HandlerVal.GetLength() - start- 1);
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
                    Footer +=id + defSTR_HeaderFormat;
                    fprintf( yyout,Footer);
                    
                    start = Footer.Find('_',0);
                    id = Footer.Right( Footer.GetLength() - start - 1 );
                    id = id.Left( id.GetLength() - 3 );//extract the range
                    
                    fprintf(yyout,defSTR_MsgIdRangeStart,id);
                }
            }
        }//end of msg handler
        }
        nSourceLine = nSourceLine + 2;//since going in different state
        if( HandlerType == defSTR_caplfunc  )
        {
            yy_push_state( CaplFunc);
        }//end of if
        else
        {
            yy_push_state(Delete);
        }//end of else
    }//end of try block
    
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vHeader\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}   


/**************************end of function*************************************/

/*******************************************************************************
 Function Name    : vDelete
 Input(s)         :     -
 Output           :     -
 Functionality    : On incounter of '{' the function will push the flow into 
                    "VarDecl" state or it will come out of the previous state. 
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vDelete()
{
    // yytext '{'
    try
    {
    if(cFlagHeader == 1)
    {//if coming from /*@@var*/ then go to VarDecl state
        yy_push_state(VarDecl);
        cFlagHeader = 0;
    }
    else
    {//go to state2.
        yy_pop_state();
    }
    }//end of try block
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vDelete\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }

}

/******************end of function*********************************************/

/*******************************************************************************
 Function Name    : vEndBody
 Input(s)         :     -
 Output           :     -
 Functionality    : On encounter of '}' the function will check the value of 
                   "counter".If it is not 0 then it will simply echo otherwise
                   it will go in "Footer"state.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 03.05.2004
 Modifications    :
*******************************************************************************/

void vEndBody()
{
    try
    {   
    if(yytext[0]=='{')
    {
        ECHO;
        counter++;
    }
    else
    {
        counter--;
        if(counter != 0)
        {
            ECHO;
        }
        else
        {
            counter = 1;
            yy_push_state(FooterState);
        }
    }
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vEndBody\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}



/*******************************************************************************
 Function Name    : vTimerFunc
 Input(s)         :     -
 Output           :     -
 Functionality    : This will check whether the variable is of type  " timer " 
                    or " mstimer ".If it "timer" then it will store the name of
                    variables in "ouTimerName".
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vTimerFunc()
{
    try
    {
    CString val, name ;
    int start;
    if( yytext[0] == ';')
    {
        fprintf(yyout,";");
    }
    val = yytext;

    start = val.Find(' ',0);

    name = val.Left(start  );
    name.MakeLower();//extract the datatype
    if( name.Find("mstimer") == -1 )
    {//it is timer
        val = val.Right( val.GetLength() - start - 1);
        val.TrimLeft();
        while(val.GetLength() != 0 )//till end of text
        {
            start = val.FindOneOf(",;");
            name = val.Left( start);
            name.TrimLeft();
            name.TrimRight();
            ouTimerName.bAdd( name );//add in array
            val = val.Right( val.GetLength() - start - 1 );
        }
    }
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vTimerFunc\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}

/*****************************end of function**********************************/



/*******************************************************************************
 Function Name    : vCnvrtByte
 Input(s)         :     -
 Output           :     -
 Functionality    :This will replace " byte " , " word " or "long ".
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/                                  
void vCnvrtByte()
{
    try
    {
    //yytext asd.long(1)
    // this.byte(2)
    // abc.word(1);
    CString val,name,type,index;
    int start , end ;

    val = yytext;
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

    if( type == defSTR_long ) 
    {
        //if long
        fprintf(yyout,defSTR_LongCnvrt,name,index);
    }

    if( type == defSTR_byte )
    {
        //if byte
        fprintf(yyout,defSTR_ByteCnvrt,name,index);
    }

    if( type == defSTR_word )
    {
        //if word
        fprintf(yyout,defSTR_WordCnvrt,name,index);
    }
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vCnvrtByte\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
    

}

/***************end of function************************************************/

/*******************************************************************************
 Function Name    : vPutKeyCase
 Input(s)         :     -
 Output           :     -
 Functionality    : This will extract the name of the key from yytext.Then it 
                    will match the key with the elements of acUnSptdKey array.
                    If it will match with any element it will replace the key
                    with the element stored at the exact position in acAltKey 
                    array .If the key is not in the arrya then it will simply
                    ECHO the yytext.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/ 
void vPutKeyCase()
{
    try
    {
    //yytext case 'a':
    CString keyname;
    int start ;
    if( yytext[0] == ';' || yytext[0] == ',')
    {
        fprintf(yyout,"%c",yytext[0]);
    }

    keyname = yytext;

    start = keyname.FindOneOf("cC");
    keyname = keyname.Right( keyname.GetLength() - start );

    start = keyname.Find(' ',0);
    keyname = keyname.Right( keyname.GetLength() - start - 1 );

    keyname.TrimLeft();
    keyname = keyname.Left( keyname.GetLength() - 1 );
    keyname.TrimRight();//extract the key

    if( keyname[0] =='\'' )
    {
        keyname = keyname.Mid(1,keyname.GetLength() - 1);
    }
   

    if( keyname.GetLength() > 1 )

    {
        int nPos = ouUnSptdKey.nFind(keyname);
        if( nPos != -1 ) 
        {  //if it is in acUnSptdKey then
            if( acAltKey[ nPos ] != -1)
            {
                fprintf(yyout,"case '%c' :",acAltKey[nPos]);
                fprintf(logfile,defSTR_ConvertedkeyInfo,keyname,nSourceLine,
                    acAltKey[nPos]); 
            }
            else
            {//if alternate key is not availabe
                ECHO;
                fprintf(logfile,defSTR_UnSptdKeyNotChanged,nSourceLine);
                nUnCnvrtdLine = float( nUnCnvrtdLine + 0.5 );//weighted 0.5
            }
        }
        else
        {//if it is not found in "ouUnSptdKey"
            ECHO;
        }
    }
    else
    {
        //if key length is one
        ECHO;
    }
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vPutKeyCase\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}

/***********end of function -- vPutKeyCase ************************************/

/*******************************************************************************
 Function Name    : vWrite
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replace write with sprintf.This will also take
                    care of "writetolog" function.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vWrite()
{
    try
    {
    //yytext "write(" or "writeToLog("
    cFlag = 1 ;//to be used for termination of write
    cFlagH = 0;  //to count the no. of white space
    cFlagLog = 1;//to differentiate "write"and "writetolog" function
    CString val;
    val = yytext;
    val.MakeLower();//for case insensitive comparison
    if( val.Find("writetolog",0) == -1)
    {
        cFlagLog = 0;//used in writeterminate function to differntiate between 
    }  // "write" and "writeToLog" function.
    while( !isalpha(yytext[cFlagH] ) )
    {
        cFlagH++;
    }
    for ( int n = 0; n < cFlagH; n++)
    {
        fprintf(yyout,"%c",' ');
    }
    fprintf(yyout,"sprintf( %s,",defSTR_TraceBufferName);  
    yy_push_state(Write); 
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vWrite\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}

/***********end of function -- vWrite *****************************************/

/*******************************************************************************
 Function Name    : vMsgID
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replace " id " of message with " m_unMsgID" and 
                    "dlc" with "m_ucDLC".
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
void vMsgID()
{
    try
    {
    CString val , name , type;
    int start , end;
    val = yytext;
    end=val.GetLength();
    start = val.Find(".",0);
    name = val.Mid(0,start);

    type = val.Mid(start+1,val.GetLength()-2);
    type.MakeLower();
    type.TrimRight();
    if(name == defSTR_this)
    {
        name = defSTR_RxMsg;
    }

    if(type == "id")//if id
    {
        name = name + defSTR_MuId ;
    }

    else if (type == "dlc")// id dlc
    {
        name = name + defSTR_MuDlc ;
    }

    if(type == "dir")
    {//replace "abc.dir" with "Rx".( Rx ~ 1)
        fprintf(yyout,"Rx");
    }

    if( type.CompareNoCase("can") == 0)
    {
        fprintf(yyout,"%s./*CAN */", name);
    }
    else
    {
        fprintf(yyout,"%s",name);
    }


    fprintf(yyout,"%c",yytext[yyleng-1]);//to write the last char
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vMsgID\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}

/***********end of function -- vMsgID  ****************************************/

/*******************************************************************************
 Function Name    : vWriteDot
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replece '.' with m_sWhichBit
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vWriteDot()
{
    try
    {
    // yytext -- "abc."
    CString val;
    int a;
    val = yytext;

    a = val.Find(".",0);
    val = val.Mid(0,a);//name of message
    val.TrimRight();

    if( val == defSTR_this )
    {//replace "this" with "RxMsg".
        val = defSTR_RxMsg ;
    }

    fprintf(yyout,defSTR_MWBit,val);
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vWriteDot\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }

}

/***********end of function -- vWriteDot **************************************/
/*******************************************************************************
  Function Name  : vRemovePhys
  Input(s)       : -
  Output         : -
  Functionality  : 
  Member of      : CFunctions
  Author(s)      : Raja N
  Date Created   : 7.1.2005
  Modifications  : 
*******************************************************************************/
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
            fprintf( yyout, "%s/*%s */", omStrParse.Mid( 0, nIndex),
                                         omStrParse.Mid(nIndex));
        }
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vWriteDot\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}

/*******************************************************************************
 Function Name    : vCnvrtThis
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replace "this" keyword .
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vCnvrtThis()
{
    try
    {
    if( cFlagHeader == 4 )
    {//if message handler
        fprintf(yyout,defSTR_RxMsg);
    
    }
    if( cFlagHeader == 3 )
    {//if key handler
        fprintf(yyout,defSTR_KeyValue);
    
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

/***********end of function  vCnvrtThis ***************************************/

/*******************************************************************************
 Function Name    : vWriteTerminator
 Input(s)         :     -
 Output           :     -
 Functionality    : This will terminate write() function
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 23.04.2004
 Modifications    :
*******************************************************************************/
void vWriteTerminator()
{
    try
    {
    int n = 0;
    // yytext -- ';' / ',', /'(' ')'
    
    if(yytext[0] == '(')
    {
        cFlag++;
        
        ECHO;
    }
    else if(yytext[0]==')')
    {  //if ')'
        cFlag--;
    
        ECHO;
    }
    if(  yytext[0] == ';' || ( yytext[0] == ',' && cFlag == 0 ) )
    {
        // cFlag == 0 to check parantheses
        fprintf(yyout,",\n");
        for(n = 0;n < cFlagH; n++ )
        { // value of cFlagH is coming from vWrite function
            fprintf(yyout,"%c",' ');//for blank space
        }
        if ( cFlagLog == 0 )
        {
            fprintf(yyout,defSTR_Trace,defSTR_TraceBufferName,");");
        }
        else
        {
            fprintf(yyout,defSTR_WriteToLog,defSTR_TraceBufferName,");");
        }
        nSourceLine++;  //to increase the no. of lines by 1
        yy_pop_state();
    }
    else
    {
        if(yytext[0] == ',')
            ECHO;
    }
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vWriteTerminator\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
    
}

/*********end of function -- vWriteTerminator *********************************/


/*******************************************************************************
 Function Name    : vSetTimer
 Input(s)         :     -
 Output           :     -
 Functionality    : This will extract the name of vaiable and elapse time from
                    the function and match the name with the elements of
                    "ouTimerName".If it will match with any element of that
                    array it means it is of type "timer" and then it will 
                    append "*1000" in the value of elapse time.And finally it
                    will call "StartTimer" function.
                    
 Member of        :     -
 Friend of        :     -

 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vSetTimer()
{
    try
    {
    //yytext--SetTimer( Timer_Variable_Name, Time_delay )
    CString text,name , val;
    int start , end;
    char wsp , index = 0;

    while( !isalpha(yytext [index] ) )
    {//to count no. of white spaces
        index++;
    }
    text = yytext;
    start = text.Find("(",7);
    end = text.Find(",",8);
    name = text.Mid(start+1,end-start-1);//name of timer

    start = end;
    end = text.Find(")",start);

    val = text.Mid(start+1, end -start -1);//elapse time
    
    name.TrimLeft();
    name.TrimRight();

    val.TrimLeft();
    val.TrimRight();

    for( wsp = 0 ; wsp <= index ; wsp ++)
    {//printf white space
        fprintf(yyout,"%c",' ');
    }
    
    fprintf(yyout,defSTR_SetTimerVal,name);

    if(ouTimerName.nFind(name) != -1)
    {//if it is "Timer" variable then
        val = val + defSTR_1000;
    }
        
    fprintf(yyout,"%s),\n",val);

    for( wsp = 0 ; wsp <= index ; wsp ++)
    {//printf white space
        fprintf(yyout,"%c",' ');
    }
    fprintf(yyout,defSTR_StartTimer,name);
    nSourceLine++;//increase no. of lines by 0ne
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vSetTimer\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}

/*********end of function -- vSetTimer ****************************************/

/*******************************************************************************
 Function Name    : vOutput
 Input(s)         :     -
 Output           :     -
 Functionality    : This will extract the parameter from yytext and replce 
                    "output" with "SendMsg".
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vOutput()
{
    try
    {
    // yytext -- output(asd)
    CString val ;
    int start;
    val = yytext ;
    start = val.Find("(",0);
    val = val.Right( val.GetLength() - start - 1);

    start = val.Find(")",0);

    val = val.Left( start );
    val.TrimLeft();
    val.TrimRight();

    fprintf(yyout,defSTR_SendMessage ,val );
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vOutput\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
    
    
}

/*******end of function -- vOutput ********************************************/

/*******************************************************************************
 Function Name    : vCancelTimer
 Input(s)         :     -
 Output           :     -
 Functionality    :This will replce "canceltimer" with "StopTimer".
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vCancelTimer()
{
    try
    {
    // yytext -- canceltimer(asd)
    CString val  ;
    int start ;
    val = yytext ;

    start = val.Find("(",10); //10--length of canceltimer

    val = val.Mid( start+1, val.GetLength() - start - 2);//extract name of timer
    val.TrimLeft();
    val.TrimRight();

    fprintf(yyout,defSTR_StopTimer , val);
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vCancelTimer\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
    
}

/**************end of function -- vCancelTimer ********************************/

/*******************************************************************************
 Function Name    : vOnLine
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replace "canOnline" with "GoOnline".
                    
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
void vOnLine()
{
    fprintf(yyout,defSTR_GoOnline);              
}
/*******************************************************************************
 Function Name    : vOffLine
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replace "canOffline" with "GoOffline".
                   
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
void vOffLine()
{
     fprintf(yyout,defSTR_GoOffline);             
}
/*******************************************************************************
 Function Name    : vTrigger
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replace "triger" with "EnableLogging".
                   
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
void vTrigger()
{
    fprintf(yyout,defSTR_EnableLogging);         
}
/*******************************************************************************
 Function Name    : vResetCan
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replace "resetCan" with "ResetController".
                  
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
void vResetCan()
{
    fprintf(yyout,defSTR_ResetController);                
}
/*******************************************************************************
 Function Name    : vDisconnect
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replace "stop" with "Disconnect".
                   
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
void vDisconnect()
{
    fprintf(yyout,defSTR_Disconnect);   
}

/***********end of function****************************************************/




/*******************************************************************************
 Function Name    : vUnSptdFunc
 Input(s)         :     -
 Output           :     -
 Functionality    : This will make unsupported functions as comment and write
                    name of the function and line no. in the log file.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
void vUnSptdFunc()
{
    fprintf(yyout,"/*");
    ECHO; 
    fprintf(yyout,"*/");
    fprintf(logfile,defSTR_UnSptdFuncLogInfo,yytext,nSourceLine);

    nUnCnvrtdLine = nUnCnvrtdLine + 1 ; //weighted 1
}

/**************end of function -- vUnsptdFunc *********************************/



/*******************************************************************************
 Function Name    : vFuncCall
 Input(s)         :     -
 Output           :     -
 Functionality    : This will extract the name of function and check with
                    acFuncName arry.If it will match with any entry it will
                    prefix "Utils_".
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vFuncCall()
{
    try
    {
    // yytext asd(
    CString val ;
    int start;
    val = yytext ;
    start = val.Find("(",0);
    val = val.Left(start);
    val.TrimRight();
    

    if( ouFuncName.nFind( val ) != -1)
    {
        fprintf(yyout,defSTR_Utils);
    }

    val = val + "(";
    fprintf(yyout,"%s",val);
    }
    catch(...)
    {
        CString cs;
        
        cs.Format(ExceptionFormat,"\"vFuncCall\"",__FILE__,__LINE__);
        MessageBox(0,cs,"Warning",MB_OK);
        exit(0);
    }
}

/*********end of function -- vFuncCall ****************************************/

/*******************************************************************************
 Function Name    : vAddLine
 Input(s)         :     -
 Output           :     -
 Functionality    : It will increse the counter on arrival of '\n'.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
void vAddLine()
{
    // yytext '\n'
     nSourceLine++;
     ECHO;   
}

/********end of function -- vAddLine ******************************************/

/*******************************************************************************
 Function Name    : vFooter
 Input(s)         :     -
 Output           :     -
 Functionality    : This will take care of footer.                    
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void vFooter()
{
    if( yy_top_state() == VarDecl )//VarDecl State
    {
        fprintf(yyout,defSTR_FooterVar);
        fprintf(yyout,defSTR_FuncProHeader);
        fprintf(yyout,defSTR_FuncProFooter);
        nSourceLine = nSourceLine + 2;
        yy_pop_state();
        yy_pop_state();

    }
    else
    {
        fprintf(yyout,defSTR_Footer);
        fprintf(yyout,"%s",Footer);
        nSourceLine += 1; //since footer contains "\n"
    }

    yy_pop_state();
}   

/*******end of function vFooter ***********************************************/

/*******************************************************************************
 Function Name    : nConvert
 Input(s)         :     -
 Output           :     -
 Functionality    : This is the function that takes care of conversion.It takes
                    three parameters ( input file name , output file name and
                    log file name) and does conversion.If user has assign 
                    database then it cinverts it and if he wants to save it,
                    the function saves the converted database in appropriate
                    output file.This function return the percentage of CANoe
                    file converted into BUSMASTER.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/

int nConvert( CString srs,CString dest ,CString dest1)
{
    

    yyin = fopen(srs,"r");
    BEGIN( FirstParse ); //start first parse
    yylex();

    yyout = fopen (dest,"w");
    logfile = fopen( dest1,"w" );
    fprintf( logfile,defSTR_InputFileInfo,srs );//writing to log file
    fprintf( logfile,defSTR_OutputFileInfo,dest );

    if( ouUnSptdKey.nGetSize() > 0)
    {//if there is any unsuppoted key then
        List d;
        d.DoModal();
    }

    if( cFlagH != 1 && counter == 2 && canoedb.IsEmpty() == TRUE)
    {//if program has any message variable 
        CSelectdb  dialog;
        dialog.DoModal();
    }
    
    convert.Convert(canoedb);//convert database
    if (cFlagLog == 2 )
    {//if user wants to save it
        convert.WriteToOutputFile(busmasterdb);
    }

    cFlagLog = 1;
    counter = 1;
    //cFlagH = 0;

    

    fseek( yyin,0,SEEK_SET );
    BEGIN( SecondParse );//begin second parse
    yylex();

    nSourceLine = nSourceLine - 1;
    float per = 0;//to calculate percentage conversion
    per = float((nSourceLine - nUnCnvrtdLine )*100/ nSourceLine);

    fprintf( logfile,defSTR_CnvrInfo,per );

    fclose( yyin );
    fclose( yyout );
    fclose( logfile );

    nSourceLine = 0, nUnCnvrtdLine = 0;
    cIndex2 = 0,cFlag = 0;
    cFlagH = 0, cFlagHeader = 0;
    counter = 1;
    fprestart = 0;
    fstart = 0;
    return( int( per ));
 
}

/*******************************************************************************
 Function Name    : OnBrowseBUSMASTERDb
 Input(s)         :     -
 Output           :     -
 Functionality    : This will pop-up a comman dialog box to select the input 
                    BUSMASTER database file.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 15.05.2004
 Modifications    :
*******************************************************************************/    

void CSelectdb::OnBrowseBUSMASTERDb() 
{
    CString omStrPath;
    int nIndex;

    CFileDialog cfd(TRUE,"dbf",NULL,OFN_OVERWRITEPROMPT|
        OFN_PATHMUSTEXIST,"BUSMASTER Database Files(*.dbf)|*.dbf||",this);
    cfd.m_ofn.lpstrTitle = "Select BUSMASTER Database File";
    
    GetDlgItemText( IDC_EDIT_ADBUSMASTER,omStrPath );
    omStrPath.TrimLeft();
    omStrPath.TrimRight();

    if(omStrPath.IsEmpty())
    {
        omStrPath = AfxGetApp()->GetProfileString("Files","Database File","");
    }
    if( (nIndex = omStrPath.ReverseFind('\\') )!= -1)
    {
        omStrPath = omStrPath.Left(nIndex);
    }

    cfd.m_ofn.lpstrInitialDir = omStrPath;
    
    if(cfd.DoModal()==IDOK)
    {
        omStrPath = cfd.GetPathName();
        SetDlgItemText(IDC_EDIT_ADBUSMASTER, omStrPath );
    }
    
    
}

/*******************************************************************************
 Function Name    : OnBrowseCANoeDb
 Input(s)         :     -
 Output           :     -
 Functionality    : This will pop-up a comman dialog box to select the  
                    CANoe database file.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 15.05.2004
 Modifications    :
*******************************************************************************/    

void CSelectdb::OnBrowseCANoeDb() 
{
    CString omStrPath;
    int nIndex;

    CFileDialog cfd(TRUE,"dbf",NULL,OFN_OVERWRITEPROMPT|
        OFN_PATHMUSTEXIST,"CANoe Database Files(*.dbc)|*.dbc||",this);
    cfd.m_ofn.lpstrTitle = "Select CANoe Database File";
    
    GetDlgItemText( IDC_EDIT_ADCANOE,omStrPath );
    omStrPath.TrimLeft();
    omStrPath.TrimRight();

    if(omStrPath.IsEmpty())
    {
        omStrPath = AfxGetApp()->GetProfileString("Files","Database File","");
    }
    if( (nIndex = omStrPath.ReverseFind('\\') )!= -1)
    {
        omStrPath = omStrPath.Left(nIndex);
    }

    cfd.m_ofn.lpstrInitialDir = omStrPath;
    
    if(cfd.DoModal()==IDOK)
    {
        omStrPath = cfd.GetPathName();
        SetDlgItemText(IDC_EDIT_ADCANOE, omStrPath );
    }
    
}

/*******************************************************************************
 Function Name    : OnChkbOption
 Input(s)         :     -
 Output           :     -
 Functionality    : This will enable or diable the controls according to the
                    state of checkbox button.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 15.05.2004
 Modifications    :
*******************************************************************************/
void CSelectdb::OnChkbOption() 
{
    UpdateData();
    if( m_option )
    {//if checked
        cFlagLog = 2;
        GetDlgItem( IDC_EDIT_ADBUSMASTER )->EnableWindow(TRUE);
        GetDlgItem( IDC_CBTN_BUSMASTERDB )->EnableWindow(TRUE);
    }
    else
    {//if unchecked
        cFlagLog = 1;
        SetDlgItemText(IDC_EDIT_ADBUSMASTER, "");
        GetDlgItem( IDC_EDIT_ADBUSMASTER )->EnableWindow(FALSE);
        GetDlgItem( IDC_CBTN_BUSMASTERDB )->EnableWindow(FALSE);
        
    }
    
}

/*******************************************************************************
 Function Name    : OnOK
 Input(s)         :     -
 Output           :     -
 Functionality    : This will verify the correctness of CANoe and BUSMASTER databse
                    files.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 15.05.2004
 Modifications    :
*******************************************************************************/

void CSelectdb::OnOK() 
{
//  "canoedb" and "busmasterdb" are global CString objects
    int flag = 1;
    CString extn;
    GetDlgItemText( IDC_EDIT_ADCANOE,canoedb );

    canoedb.TrimRight();
    extn = canoedb.Right(4);
    extn.MakeLower();
        
    if( extn != ".dbc")
    {
        MessageBox(defSTR_Warning8,MB_OK);
        SetDlgItemText( IDC_EDIT_ADCANOE,"" );
        GetDlgItem( IDC_EDIT_ADCANOE)->SetFocus();
        flag = 0;//to avoid conversion
    }
    else
    {
        CFile fdatabase;
        if ( fdatabase.Open( canoedb,CFile::modeRead ) == NULL )
        {
            //if file doesn't exist or in write mode
            MessageBox( defSTR_Warning2,MB_ICONWARNING|MB_OK );
            SetDlgItemText( IDC_EDIT_ADCANOE,"" );
            GetDlgItem( IDC_EDIT_ADCANOE)->SetFocus();
            flag = 0;//to avoid conversion
        }
        else
        {
            fdatabase.Close();
        }
    }
        
    if( cFlagLog == 2)
    {
        GetDlgItemText( IDC_EDIT_ADBUSMASTER,busmasterdb );
        busmasterdb.TrimRight();
        extn = busmasterdb.Right(4);
        extn.MakeLower();
            
        if( extn != ".dbf")
        {
            MessageBox(defSTR_Warning9,MB_OK);
            SetDlgItemText( IDC_EDIT_ADBUSMASTER,"" );
            GetDlgItem( IDC_EDIT_ADBUSMASTER)->SetFocus();
            flag = 0;//to avoid conversion
        }
        else
        {
            CFile fdatabase;
            if ( fdatabase.Open( busmasterdb,CFile::modeCreate|CFile::modeWrite )
                == NULL )
            {
                //if file doesn't exist or in write mode
                MessageBox( defSTR_Warning3,MB_ICONWARNING|MB_OK );
                SetDlgItemText( IDC_EDIT_ADBUSMASTER,"" );
                GetDlgItem( IDC_EDIT_ADBUSMASTER)->SetFocus();
                flag = 0;//to avoid conversion
            }
            else
            {
                fdatabase.Close();
            }
        }
    }
    if ( flag == 1 )
    {//if both are correct then
        CDialog::OnOK();
    }
}

/*******************************************************************************
 Function Name    : OnConvert
 Input(s)         :     -
 Output           :     -
 Functionality    : This will convert the input can file into C file.This will
                    check whether the input file is correct or not.If name of
                    output file is not there then it will  create a default
                    output file with the name of input file with extention C.
                    The log file will be created with default name of input file 
                    withe extention "txt".This function will intialize acAltkey
                    with -1.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 21.04.2004
 Modifications    :
*******************************************************************************/
void CCAPL2CPropertyPage::OnConvert() 
{
    CString dest , srs ;//to store the destination file and source file
    CFile fsrs, fdest, flog; //to check files
    CString extn;//to store the extension

    int flag = 1;
    
    for( m = 0;m < 254; m++)
    {  //initialize array with -1.
        acAltKey[m]= -1;
    }
    
    GetDlgItem(IDC_CBTN_LOG)->EnableWindow(FALSE);
    GetDlgItemText( IDC_EDIT_INPUT,srs );//srs will have input file path
    GetDlgItemText( IDC_EDIT_OUTPUT,dest );//dest will have output file path

    srs.TrimRight();
    extn = srs.Right(4);
    extn.MakeLower();

    if( extn != ".can" )
    { //if file doesn't exist or in write mode
        MessageBox( defSTR_Warning5,MB_ICONWARNING|MB_OK );
        SetDlgItemText( IDC_EDIT_INPUT,"" );
        GetDlgItem( IDC_EDIT_INPUT)->SetFocus();
        flag = 0;//to avoid conversion
    }
    
    else if( fsrs.Open( srs,CFile::modeRead ) == NULL )
    { //if file doesn't exist or in write mode
        MessageBox( defSTR_Warning2,MB_ICONWARNING|MB_OK );
        SetDlgItemText( IDC_EDIT_INPUT,"" );
        GetDlgItem( IDC_EDIT_INPUT)->SetFocus();
        flag = 0;//to avoid conversion
    }
    else
    {//close fsrs file
        fsrs.Close();
    }
    
    if(dest == "")
    {  //for default output file name
        dest = srs.Left( srs.GetLength() - 4 );
        dest += ".c";
    }
    else
    {
        dest.TrimRight();
        extn = dest.Right(2);
        extn.MakeLower();
        
        if ( extn != ".c")
        {
            MessageBox(defSTR_Warning1,MB_ICONWARNING|MB_OK );
            SetDlgItemText( IDC_EDIT_OUTPUT,"" );
            GetDlgItem( IDC_EDIT_OUTPUT)->SetFocus();
            flag = 0;//to avoid conversion
        }
    }
    
    if(fdest.Open(dest,CFile::modeCreate|CFile::modeWrite)==NULL)
    {
        //if output file is in read mode
        MessageBox( defSTR_Warning3,MB_ICONWARNING|MB_OK );
        SetDlgItemText( IDC_EDIT_OUTPUT,"" );
        GetDlgItem( IDC_EDIT_OUTPUT)->SetFocus();
        flag = 0;//to avoid conversion
    }
    else
    {//close fdest file
        fdest.Close();
        
    }
    
    if ( cFlagH == 1)
    {
        GetDlgItemText( IDC_EDIT_CANOEDB,canoedb );

        canoedb.TrimRight();
        extn = canoedb.Right(4);
        extn.MakeLower();
        
        if( extn != ".dbc")
        {
            MessageBox(defSTR_Warning8,MB_OK);
            SetDlgItemText( IDC_EDIT_CANOEDB,"" );
            GetDlgItem( IDC_EDIT_CANOEDB)->SetFocus();
            flag = 0;//to avoid conversion
        }
        else
        {
            CFile fdatabase;
            if ( fdatabase.Open( canoedb,CFile::modeRead ) == NULL )
            {
                //if file doesn't exist or in write mode
                MessageBox( defSTR_Warning2,MB_ICONWARNING|MB_OK );
                SetDlgItemText( IDC_EDIT_CANOEDB,"" );
                GetDlgItem( IDC_EDIT_CANOEDB)->SetFocus();
                flag = 0;//to avoid conversion
            }
            else
            {
                fdatabase.Close();
            }
        }
        
        if( cFlagLog == 2)
        {
            GetDlgItemText( IDC_EDIT_BUSMASTERDB,busmasterdb );
            busmasterdb.TrimRight();
            extn = busmasterdb.Right(4);
            extn.MakeLower();
            
            if( extn != ".dbf")
            {
                MessageBox(defSTR_Warning9,MB_OK);
                SetDlgItemText( IDC_EDIT_BUSMASTERDB,"" );
                GetDlgItem( IDC_EDIT_BUSMASTERDB)->SetFocus();
                flag = 0;//to avoid conversion
            }
            else
            {
                CFile fdatabase;
                if ( fdatabase.Open( busmasterdb,CFile::modeCreate|CFile::modeWrite ) == NULL )
                {
                    //if file doesn't exist or in write mode
                    MessageBox( defSTR_Warning3,MB_ICONWARNING|MB_OK );
                    SetDlgItemText( IDC_EDIT_BUSMASTERDB,"" );
                    GetDlgItem( IDC_EDIT_BUSMASTERDB)->SetFocus();
                    flag = 0;//to avoid conversion
                }
                else
                {
                    fdatabase.Close();
                }
            }
        }
    }
    
    dest1 = dest;
    
    dest1 = dest.Left( dest.GetLength() -  2 );
    dest1 +=  defSTR_LogName;//default output file name
    if(flog.Open(dest1,CFile::modeCreate|CFile::modeWrite)== NULL)
    {
        MessageBox( defSTR_Warning4,MB_ICONWARNING|MB_OK );
        flag = 0;//to avoid conversion
    }
    else
    {
        flog.Close();
    }

    if( flag == 1)
    {//means all the files are correct
        float value;
            
        value = float( nConvert(srs , dest ,dest1) );
        // Clear database information   
        convert.m_listMessages.RemoveAll();

        CWnd * pPercent = GetDlgItem(IDC_STAT_RESULT2);
        if( pPercent)
        {//to display the result
            CString omStrPercent;
            omStrPercent.Format("%.2f",value);
            pPercent->SetWindowText(omStrPercent+ " % ");
            SaveSettings();
        }
        ouUnSptdKey.vClearArray();
        GetDlgItem( IDC_CBTN_LOG )->EnableWindow(TRUE);
    }
}

void CSelectdb::OnChangeEditAdcanoe() 
{
    //this function will be called every time when the user will make any
    //change in the name if input file.If the edit box is blank,"ok" button
    //will be disabled.
    CString omStrPath;
    GetDlgItemText( IDC_EDIT_ADCANOE,omStrPath );

    if( omStrPath.GetLength() > 0 )
    {
        GetDlgItem( IDOK )->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem( IDOK )->EnableWindow(FALSE);
    }
    
}

void CSelectdb::OnCancel() 
{
    fprintf(logfile,"You have not assigned the Databaase.\n");
    CDialog::OnCancel();
}

/*******************************************************************************
 Function Name    : vMsgDecl
 Input(s)         :     -
 Output           :     -
 Functionality    : This will convert the input can file into C file.This will
                    check whether the input file is correct or not.If name of
                    output file is not there then it will  create a default
                    output file with the name of input file with extention C.
                    The log file will be created with default name of input file 
                    withe extention "txt".This function will intialize acAltkey
                    with -1.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 15.05.2004
 Modifications    :
*******************************************************************************/
void vMsgDecl()
{
    try
    {
    CString val , ident , name ;
    int start  ;
    char extndd = 0;
    CMessage msg;
    int flag = 0;
    unsigned int decid;
    if( yytext[0] == ';')
    {
        fprintf(yyout,";");
    }
    val = yytext;
    val.TrimLeft();
    val = val.Right( val.GetLength() - 5 );
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
        ident = ident.Right( ident.GetLength() - start - 1 );
    }

    if( isdigit( ident[0]) )
    {//if message is declared with id then
        flag = 1;
        
        if( ident[ ident.GetLength() - 1 ] =='x'||
            ident[ ident.GetLength() - 1 ] =='X')
        {//if last element is 'x'
            extndd = 1;
            ident = ident.Left( ident.GetLength() - 1 );
        }
        /*if(ident.GetLength() > 2)
        {
            if(ident[1] =='X' || ident[1] == 'x' )
            {//if hex id
                char *stop;
                ident = ident.Right(ident.GetLength() - 2);
                decid = strtol(ident,&stop,16);
            }
        }
        else
        {//if deci id
            decid = atoi(ident);
        }*/
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
        msg = convert.ouFindMessage(decid);
    }
    else
    {//message is declared with name
        msg = convert.ouFindMessage(ident);
    }

    if( val.Find('{',0) == -1 )
    {//if message is not initialized
        if( flag == 0 )
        {
            if( msg.m_uiMsgID == INVALID )
            {//means unknown message
                fprintf(logfile,"Message : %s not found in database.\n",ident);
                nUnCnvrtdLine += 1;
                // to decrease the percentage of conversion
            }
            fprintf(yyout,defSTR_MsgFormat,ident,name,
                msg.m_uiMsgID,msg.m_cFrameFormat == 'S'? 0 : 1,
                msg.m_cDataFormat,msg.m_ucLength ); 
        }
        
        else
        {
            flag = 0;
            if( msg.m_uiMsgID != INVALID )
            {//means id is in database
                fprintf(yyout,defSTR_MsgFormat,msg.m_acName,name,
                msg.m_uiMsgID,extndd,msg.m_cDataFormat,
                msg.m_ucLength );
            }
            else
            {//means it is an unknown message
                fprintf(yyout,defSTR_STCANMsgFormat,name,decid,extndd);
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
            msgbyte.SetAtGrow(i,'0');
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
                fprintf(logfile,"Message : %s not found in database.\n",ident);
                nUnCnvrtdLine += 1;
                // to decrease the percentage of conversion
            }
            fprintf(yyout,defSTR_MsgIniFormat,ident,name,
                msg.m_uiMsgID,msg.m_cFrameFormat == 'S'? 0 : 1,dlc );   
        
            for( int  m = 0; m < dlc; m++)
            {
                fprintf(yyout,"%s",msgbyte.GetAt( m ));//each byte of message
                if(m < dlc - 1)
                {
                    fprintf(yyout,",");
                }
            }
            fprintf(yyout,"}};");
        }
        else
        {
            flag = 0;
            if( msg.m_uiMsgID != INVALID )
            {
                fprintf(yyout,defSTR_MsgIniFormat,msg.m_acName,name,
                msg.m_uiMsgID,extndd,dlc );
                for( int  m = 0; m < dlc; m++)
                {
                    fprintf(yyout,"%s",msgbyte.GetAt( m ));//each byte of message
                    if(m < dlc - 1)
                    {
                        fprintf(yyout,",");
                    }
                }
                fprintf(yyout,"}};");
            }
            else
            {
                fprintf(yyout,"STCAN_MSG %s ={ 0x%x,%d,0,%d,{"
                    ,name,decid,extndd,dlc);

                for( int  m = 0; m < dlc; m++)
                {
                    fprintf(yyout,"%s",msgbyte.GetAt(m ));//each byte of message
                    if(m < dlc - 1)
                    {
                        fprintf(yyout,",");
                    }
                }
                fprintf(yyout,"}};");
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

/*******************************************************************************
 Function Name    : vMsgCpy
 Input(s)         :     -
 Output           :     -
 Functionality    : This will extract the name of the message vaiable from 
                    yytext and use "memcpy" function to copy the containts
                    from "rxMsg".
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 15.05.2004
 Modifications    :
*******************************************************************************/
void vMsgCpy()
{
    // yytext  abc = (message *)this;
    CString val;
    int start;
    if( yytext[0] == ';' || yytext[0] == ',')
    {
        fprintf( yyout,"%c",yytext[0] );
    }

    val = yytext;
    start = val.Find("=",0);
    val = val.Left( start );//name of variable
    val.TrimLeft();
    val.TrimRight();
    fprintf(yyout,"memcpy( &%s, &RxMsg , sizeof( RxMsg ) );",val);
}

/*******************************************************************************
 Function Name    : velCount
 Input(s)         :     -
 Output           :     -
 Functionality    : This will replace "elCount" function.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 15.05.2004
 Modifications    :
*******************************************************************************/
void velCount()
{
    //yytect elCount(ar)
    //yytext elCount(ar[][])
    CString val;
    int start ;
    val = yytext;
    start = val.Find('(',0);

    val = val.Right( val.GetLength() - start - 1 );
    val = val.Left( val.GetLength() - 1 );//name of array
//  start = val.FindOneOf("[)");
//  val = val.Left( start );//name of array
    val.TrimLeft();
    val.TrimRight();

    fprintf(yyout,"( sizeof(%s)/sizeof(%s[0]) )",val,val);
}

        
void CSelectdb::OnClose() 
{
    OnCancel();
    CDialog::OnClose();
}

/*******************************************************************************
 Function Name    : vUtilFunc
 Input(s)         :     -
 Output           :     -
 Functionality    : This function will extract the return type and parameter of
                    of a caplFunction.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 05.06.2004
 Modifications    :
*******************************************************************************/
void vUtilFunc()
{
    CString val, parameter;
    int start,end;

    val = yytext;
    val.TrimLeft();

    start = val.Find("(",0);
    end = val.Find(")",start);

    parameter = val.Mid( start+1 ,end - start - 1);
    parameter.TrimLeft();
    parameter.TrimRight();//parameter of function

    val = val.Left( start - 1 );
    
    val.MakeReverse();
    val.TrimLeft();

    start = val.Find(" ",0);
    val = val.Right( val.GetLength() - start );
    
    val.TrimLeft();

    val.Find("",0);

    val = val.Left( start );
    val.TrimLeft();
    val.MakeReverse();//return type

    if( val.IsEmpty() )
    {//in case of void function
        fprintf(yyout,"void ");
    }
    else
    {
        fprintf(yyout,"%s ",val);//return type
    }

    val = Footer;
    //extract name from footer
    val = val.Left( val.GetLength() - 3);
    fprintf(yyout,"%s(",val);

    fprintf(yyout,"%s)\n{",parameter);
    
    yy_pop_state();
}
