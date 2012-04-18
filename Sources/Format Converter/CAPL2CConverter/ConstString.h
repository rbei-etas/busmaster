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
 * \file      ConstString.h
 * \brief     Interface file for standard definitions
 * \authors   Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for standard definitions
 */

#define defSTR_StartIncludeHeader "/* Start BUSMASTER include header */\n"
#define defSTR_EndIncludeHeader "/* End BUSMASTER include header */\n"

#define defSTR_GlobalVarHeader "/* Start BUSMASTER global variable */\n"
#define defSTR_TypedefByte " typedef unsigned char byte ;\n"
#define defSTR_TypedefWord " typedef long word ;\n"
#define defSTR_DefRx " #define Rx  1\n"
#define defSTR_ForTrace "char %s[1000];  /*using for Trace*/"
#define defSTR_TraceBufferName " GlobalTrace "
#define defSTR_FooterVar "/* End BUSMASTER global variable */\n"
#define defSTR_HeaderFormat " */\n"


#define defSTR_FuncProHeader "/* Start BUSMASTER Function Prototype  */\n"
#define defSTR_FuncProFooter "/* End BUSMASTER Function Prototype  */"


#define defSTR_SetTimerVal "SetTimerVal(\"OnTimer_%s_100\","
#define defSTR_1000 "*1000"
#define defSTR_StartTimer "StartTimer(\"OnTimer_%s_100\",0)"
#define defSTR_TimerFormat "_100 */\n"
#define defSTR_OnTimer "OnTimer_"
#define defSTR_TimerStart "void OnTimer_%s_100()\n{"


#define defSTR_Header "/* Start BUSMASTER generated function - "
#define defSTR_Footer "}/* End BUSMASTER generated function - "


#define defSTR_MsgIniFormat "%s  %s = { 0x%x,%d,0,%d,{"
#define defSTR_STCANMsgFormat "STCAN_MSG %s ={ 0x%x,%d,0,8,{0,0,0,0,0,0,0,0}};"
#define defSTR_MsgFormat "%s  %s = { 0x%x,%d,%c,%d };"
#define defSTR_MsgAll "OnMsg_All */"
#define defSTR_OnMsgAllHeader "OnMsg_All */\n"
#define defSTR_OnMsgAllStart "void OnMsg_All(STCAN_MSG RxMsg)\n{"
#define defSTR_MsgIdRangeHeader "OnMsgIDRange_"
#define defSTR_MsgIdRangeStart "void OnMsgIDRange_%s(STCAN_MSG RxMsg)\n{"
#define defSTR_MsgIdUs "OnMsgID_"
#define defSTR_MsgIdUsStart "void OnMsgID_"
#define defSTR_StcanMsg "(STCAN_MSG Rxmsg)\n{"
#define defSTR_MsgNameHeader "OnMsgName_"
#define defSTR_MsgNameStart "void OnMsgName_%s(%s RxMsg )\n{"
#define defSTR_MsgIdStart "void OnMsgID_%s(STCAN_MSG RxMsg)\n{"

#define defSTR_KeyAllHeader "OnKey_All */\n"
#define defSTR_KeyAllStart "void OnKey_All(unsigned char KeyValue)\n{"
#define defSTR_KeyCharHeader "OnKey_"
#define defSTR_KeyCharStart "void OnKey_%s(unsigned char KeyValue)\n{"


#define defSTR_DllLoadHeader "OnDLL_Load */\n"
#define defSTR_DllLoadStart "void OnDLL_Load()\n{"


#define defSTR_DllUnLoadHeader "OnDLL_Unload */\n"
#define defSTR_DllUnLoadStart "void OnDLL_Unload()\n{"


#define defSTR_BusOffHeader "OnError_Bus_Off */\n"
#define defSTR_BusOffStart "void OnError_Bus_Off()\n{"

#define defSTR_ErrorFrameHeader "OnError_Frame */\n"
#define defSTR_ErrorFrameStart "void OnError_Frame()\n{"


#define defSTR_ErrorActiveHeader "OnError_Active */\n"
#define defSTR_ErrorActiveStart "void OnError_Active()\n{"



#define defSTR_ErrorPassiveHeader "OnError_Passive */\n"
#define defSTR_ErrorPassiveStart "void OnError_Passive()\n{"


#define defSTR_WarningLimitHeader "OnError_Warning_Limit */\n"
#define defSTR_WarningLimitStart "void OnError_Warning_Limit()\n{"

#define defSTR_GoOnline "GoOnline()"
#define defSTR_GoOffline "GoOffline()"
#define defSTR_EnableLogging "EnableLogging()"
#define defSTR_ResetController "ResetController()"
#define defSTR_Disconnect "Disconnect()"

#define defSTR_Utils "Utils_"
#define defSTR_AucData "m_aucData["
#define defSTR_FormSpcfr "( %s )\n{"
#define defSTR_StopTimer "StopTimer(\"OnTimer_%s_100\")"
#define defSTR_WriteToLog "WriteToLogFile(%s%s"
#define defSTR_RxMsg "RxMsg"
#define defSTR_KeyValue "KeyValue"
#define defSTR_Trace "Trace(%s%s "
#define defSTR_MuId ".m_unMsgID"
#define defSTR_MuDlc ".m_ucDLC"
#define defSTR_MWBit "%s.m_sWhichBit."
#define defSTR_LogName "log.txt"
#define defSTR_SendMessage "SendMsg( %s )"


#define defSTR_UnSptdFuncLogInfo "Unsupported Function : %s @ Line No. %d\n"
#define defSTR_Warning1 "Invalid Output File","Warning"
#define defSTR_Warning2 "This file does not exist","Warning"
#define defSTR_Warning3 "Output file is in read mode","Warning"
#define defSTR_InputFileInfo "Input File : %s\n"
#define defSTR_OutputFileInfo "Output File : %s\n"
#define defSTR_CnvrInfo "%s%f\n","Percentage of File Converted ="
#define defSTR_Warning4 "Log file is in read mode","Warning"
#define defSTR_Warning5 "Invalid input file","Warning"
#define defSTR_Warning6 "Default combination will be used for rest of the keys","Warning"
#define defSTR_Warning7 "Unsupported Key:%s @ Line No. %d has not been converted\n"
#define defSTR_Warning8 "Wrong CANoe Database File","Warning"
#define defSTR_Warning9 "Wrong BUSMASTER Database File","Warning"
#define defSTR_EnvVarLogEntry "Environment handler : @ Line No. %d has been replaced with Key Handler\n"
#define defSTR_UnSptdKeyNotChanged "Unsupported Key : @ Line No. %d has not been changed.\n"
#define defSTR_ConvertedkeyInfo "Unsupported Key :'%s' @ %d has been replaced with '%c'\n"

#define defSTR_MaxSizeBreadth 80
#define defSTR_MaxSizeLength 255
#define defINT_SizeofacAltKeyArray  62


#define defSTR_key "key"
#define defSTR_envvar "envVar"
#define defSTR_timer "timer"
#define defSTR_startstart "startStart"
#define defSTR_busoff "busOff"
#define defSTR_caplfunc "caplFunc"
#define defSTR_errorframe "errorFrameErrorFrame"
#define defSTR_erroractive "errorActive"
#define defSTR_errorpassive "errorPassive"
#define defSTR_warninglimit "warningLimit"
#define defSTR_msg "msg"
#define defSTR_this "this"
#define defSTR_byte "byte"
#define defSTR_long "long"
#define defSTR_word "word"
#define defSTR_LongCnvrt "%s.m_sWhichBit.m_aulData[%s]"
#define defSTR_ByteCnvrt  "%s.m_sWhichBit.m_aucData[%s]"
#define defSTR_WordCnvrt "%s.m_sWhichBit.m_auwData[%s]"

#define defSTR_START_BRAC "["
#define defSTR_END_BRAC "]"
#define defSTR_DB_VER       defSTR_START_BRAC"DATABASE_VERSION"defSTR_END_BRAC
#define defSTR_VER_NO       "1.1"
