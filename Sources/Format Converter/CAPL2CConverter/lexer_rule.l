
WS   [ \t\n]
WSP  [ \t]
ANY  [a-zA-Z0-9_,." \n\t+{}[\]():>`~!@#$%^&/\*=-]
UNSUP_LIST ("setBtr"|"setOcr"|"outport"|"inport"|"getCardType"|"getChipType"|"CallAllOnEnvVar"|"setStartdelay"|"getStartdelay"|"setDrift"|"getDrift"|"setJitter"|"getJitterMin"|"getJitterMax"|"getValue"|"putValue"|"keypressed"|"setPortBits"|"EnableControl")
UNSUP_LIST_FILE_API ("fileClose"|"fileGetBinaryBlock"|"fileGetString"|"fileGetStringSZ"|"fileName"|"filePutString"|"fileReadArray"|"fileReadFloat"|"fileReadInt"|"fileReadString"|"fileRewind"|"fileWriteBinaryBlock"|"fileWriteFloat"|"fileWriteInt"|"fileWriteString"|"FlexRayRcvFrame"|"FrSendFrame"|"FrSendMsg"|"FrSetSendFrame"|"FrSetSendMsg"|"setFilePath"|"setWritePath"|"openFileRead"|"openFileWrite"|"getProFileArray"|"getProFileInt"|"getProFileFloat"|"getProFileString"|"writeProFileInt"|"writeProFileFloat"|"writeProFileString")
ALNUM   [a-zA-Z0-9_]
NOALNUM  [^a-zA-Z0-9_]
HEADER ("key"|"msg"|"timer"|"envVar"|"caplFunc"|"startStart"|"busOff"|"errorFrameErrorFrame"|"errorActive"|"errorPassive"|"warningLimit"|"prestart"|"stop")
%{
#include "functions.h"
%}	
%x  FirstParse
%s  SecondParse
%x  VarDecl
%x  Delete
%x  Write
%x  Comment
%x  NoChange
%x  CaplFunc
%x  DelCommentML
%x  NoChangeFirstParse
%x  FooterState
%option stack
%option never-interactive 

%%
<FirstParse>"\""                          yy_push_state(NoChangeFirstParse);
<NoChangeFirstParse>"\\\""                
<NoChangeFirstParse>"\""                  yy_pop_state();
<FirstParse>[ \t;,]"case"{WS}+.*":"       vKeyStoreCase();
<FirstParse>"/*@@key:".*":*/"             vKeyStoreHeader();
<FirstParse>"/*@@envVar:".*":*/"          vEnvVar();
<FirstParse>"/*@@caplFunc:".*":*/"        vCaplFunc();
<FirstParse>"on"{WS}+"message"
<FirstParse>"message"{WS}+                counter = 2;
<FirstParse>"/*@@preStart:PreStart:*/"    fprestart = 1;
<FirstParse>"/*@@startStart:Start:*/"     fstart = 1;
<SecondParse>("/*@@"{HEADER}":".*":*/")|("/*@@var:*/")    vHeader();  
<VarDecl,SecondParse>[ \t;,]*{ALNUM}+{WS}*"="{WS}*"("{WS}*"message"{WS}+"*"{WS}*")"{WS}*"this"{WS}*";"   vMsgCpy();
<VarDecl,SecondParse>[ \t;]*"message"{WS}+{ANY}+";"       vMsgDecl();
<Delete>"{"                               vDelete(); 
<Delete>"//".*
<Delete>"/*"							  yy_push_state(DelCommentML);
<DelCommentML>"*/"							  yy_pop_state();
<CaplFunc>[a-zA-Z0-9_,." \n\t+}[\]():>`~!@#$%^&/\*=-]+"{"   vUtilFunc();
<SecondParse,VarDecl>"{"|"}"              vEndBody();                          
<VarDecl>[ \t\n;]*("timer"|"msTimer"){WS}+{ANY}+";"       vTimerFunc();  
<SecondParse,Write>{ALNUM}+"."("byte"|"long"|"word"){WS}*"("[a-zA-Z_0-9+-]+")"  vCnvrtByte();  
<SecondParse>[ \t;,]*"case"{WS}+.*":"        vPutKeyCase(); 
<SecondParse>{WSP}*("write"|"writeToLog"){WS}*"("{WS}*          vWrite();  
<SecondParse,Write>{ALNUM}+"."("id"|"dlc"|"dir"|"can"){NOALNUM}       vMsgID(); 
<SecondParse,Write>({UNSUP_LIST}|{UNSUP_LIST_FILE_API}){WS}*"("[a-zA-Z0-9_,." \n+=-]*")"{WS}*(";"|","|"==")*         vUnSptdFunc();
<SecondParse,Write>[0-9]+"."[0-9]+                ECHO;
<SecondParse,Write>{ALNUM}+"."{WS}*"phys"         vRemovePhys();
<SecondParse,Write>{ALNUM}{WS}*+"."                    vWriteDot();
<SecondParse,Write>({ALNUM}+"this"{ALNUM}+)|({ALNUM}+"this")|("this"{ALNUM}+)    ECHO;  
<SecondParse,Write>"this"   vCnvrtThis();  
<Write>";"|","|"("|")"                   vWriteTerminator(); 
<Write,SecondParse>"\""                  ECHO ,   yy_push_state(NoChange);
<NoChange>"\\\""                         ECHO;
<NoChange>"\""                           ECHO;   yy_pop_state();
<SecondParse>"elCount"{WS}*"("{WS}*[a-zA-Z0-9_[\] \t\n]+{WS}*")"   velCount(); 
<SecondParse>{WSP}*("setTimer"){WS}*"("{ANY}*")"{WS}*     vSetTimer();   
<SecondParse>"output"{WS}*"("{WS}*"this"{WS}*")"{WS}*  fprintf(yyout,"SendMsg(RxMsg)");
<SecondParse>"output"{WS}*"("{ANY}*")"{WS}*                     vOutput();
<SecondParse>("cancelTimer"|"canceltimer"){WS}*"("{ANY}*")"{WS}*  vCancelTimer();
<SecondParse>"canOnline"{WS}*"(".*")"{WS}*                vOnLine();
<SecondParse>"canOffline"{WS}*"(".*")"{WS}*                vOffLine();
<SecondParse>"trigger"{WS}*"("{WS}*")"{WS}*                   vTrigger();
<SecondParse>"resetCan"{WS}*"("{WS}*")"{WS}*                  vResetCan();
<SecondParse>"stop"{WS}*"("{WS}*")"{WS}*                      vDisconnect();
<SecondParse>{ALNUM}+{WS}*"("                       vFuncCall();  
<SecondParse,VarDecl,Write>"/*"   ECHO,   yy_push_state(Comment);
<SecondParse,NoChange,Write,VarDecl,Comment>\n             vAddLine();
<FooterState>"/*@@end*/"                vFooter();	
<Delete,FooterState,DelCommentML>\n 
<Comment>"*/"                                             ECHO,  yy_pop_state(); 
<NoChange,Write,Comment>.                   ECHO;
<Delete,FooterState,DelCommentML>. 
<SecondParse,VarDecl,Write>"//".*     ECHO;
%%
int yywrap()
{
return 1;
}
