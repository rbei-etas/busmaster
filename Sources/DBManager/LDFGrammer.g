grammar LDFGrammer;

options
{
    language = Cpp;
}
tokens {

}


@parser::includes
{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string>
  #include <algorithm>
  #include "LDFGrammerLexer.hpp"
  #include "LDFParserHelper.h"
  #include "defines.h"
} 

@lexer::namespace { User }
@parser::namespace{ User }

@lexer::context
{
static int nCommentChannel;
}

@lexer::members
{
int LDFGrammerLexer::nCommentChannel = 0;
}
@parser::context
{
CLDFParserHelper m_ouLDFParserHelper; 
void displayRecognitionError(ANTLR_UINT8** pAntlr, Parser::ExceptionBaseType* pBaseTypes)
{
  ParsingResults ouParsingResult;
    ouParsingResult.m_ouErrorType = eError;
    ouParsingResult.m_strErrorDesc = pBaseTypes->get_message();
    //ouParsingResult.m_strErrorDesc = pBaseTypes->get_ruleName();
    //ouParsingResult.m_strErrorDesc = pBaseTypes->get_expecting();
    ouParsingResult.m_unErrorCode = 1;
    ouParsingResult.m_unLineNum = pBaseTypes->get_line();
    char chError[1024] ="";
    char** pchTokens = (char**)pAntlr;
    if ( *pchTokens != nullptr )
    {
    int nTokenExpecting = pBaseTypes->get_expecting();
    if ( nTokenExpecting > 0 )
    {
      std::string strTokenExp =  pchTokens[pBaseTypes->get_token()->get_type()];
      sprintf_s(chError, "mismatched input at line \%d at position \%d while Expecting \"\%s\" Token", pBaseTypes->get_line()
        , pBaseTypes->get_charPositionInLine(), strTokenExp.c_str());
            
    }
    else
    {
      sprintf_s(chError, "Unexpected Token Found at line \%d", pBaseTypes->get_line()
                          , pBaseTypes->get_charPositionInLine());
    }
    ouParsingResult.m_strErrorDesc = chError;;
    }
  m_ouLDFParserHelper.AddParsingResult(ouParsingResult);
}
ERRORCODE GetParsingResults(std::list<ParsingResults>& ouErrorResult, std::list<ParsingResults>& ouWarningResult)
{
  ouErrorResult = m_ouLDFParserHelper.m_ouErrors;
  ouWarningResult = m_ouLDFParserHelper.m_ouWarnings;
  return EC_SUCCESS;
}
}
@lexer::traits 
{
  class LDFGrammerLexer; 
  class LDFGrammerParser; 
 
  template<class ImplTraits>
  class UserTraits : public antlr3::CustomTraitsBase<ImplTraits>
  {
  public:
      //for using the token stream which deleted the tokens, once it is reduced to a rule
    //but it leaves the start and stop tokens. So they can be accessed as usual
    static const bool TOKENS_ACCESSED_FROM_OWNING_RULE = true;
  };

  typedef antlr3::Traits< LDFGrammerLexer, LDFGrammerParser, UserTraits > LDFGrammerLexerTraits;
  typedef LDFGrammerLexerTraits LDFGrammerParserTraits;

  /* If you don't want the override it is like this.
     class TLexer;
     class TParser;
     typedef antlr3::Traits< TLexer, TParser > TLexerTraits;
     typedef TLexerTraits TParserTraits;
   */
}


//Grammers For LDF File


ldf_file [LDFCluster *pCluster]
@init{m_ouLDFParserHelper.SetCluster(pCluster, (char**)this->get_psrstate()->get_tokenNames());m_ouLDFParserHelper.m_ouErrors.clear(); m_ouLDFParserHelper.m_ouWarnings.clear();
    LDFGrammerLexer::nCommentChannel = 0;}
:
   (comment+)?             // Completed
   lin_description_tag        // Completed
   lin_protocol_def         // Completed
   channel_name_def         // Completed
   node_def             // Completed
   diag_addr_def          // Completed
   node_composition_def       // Completed
   signal_def           // Completed
   diag_signal_def?         // Completed
   dynamic_frame_def?       // Completed
   frame_def            // Completed
   sporadic_frame_def?       // Completed
   event_triggered_frame_def?    // Completed
   diag_frame_def?         // Completed
   node_attributes_def?        // Completed
   schedule_table_def       // Completed
   signal_groups_def        // Completed
   signal_encoding_type_def     // Completed
   signal_representation_def    // Completed
  ;

  
lin_description_tag:
  Ldf_desc_file ';' 
  {
  m_ouLDFParserHelper.LinDescrptionTagFound(); 
  LDFGrammerLexer::nCommentChannel = 99;    //Hidden
  }
  ;

Ldf_desc_file:
  'LIN_description_file' 
  {
    nCommentChannel = 99;
  }
;
lin_protocol_def:
  lin_protocol_version_def
  lin_language_version_def
  lin_speed_def
  {
  m_ouLDFParserHelper.AddProtocolInfo($lin_protocol_version_def.dProtocolVer, $lin_language_version_def.dLanguageVer, $lin_speed_def.dSpeed);
  }
  ;  

lin_protocol_version_def 
returns [double dProtocolVer]
@init{dProtocolVer = 0.0;}
:
  'LIN_protocol_version' '=' version ';'
  {
    dProtocolVer = $version.dVersion;
  }  
;
  
lin_language_version_def
returns [double dLanguageVer]
@init{dLanguageVer = 0.0;}
:
  'LIN_language_version' '=' version ';'
  { 
  dLanguageVer = $version.dVersion;
  }
;
         
lin_speed_def
returns [double dSpeed]
@init{dSpeed = 0.0;}
:
  'LIN_speed' '=' lin_speed 'kbps' ';'
  {
  dSpeed = $lin_speed.dSpeed;
  }
;        

channel_name_def:
    | /*optional*/
    'Channel_name' '=' StringLiteral ';'
  {
	std::string strChannelName = $StringLiteral.text;
	m_ouLDFParserHelper.Trim(strChannelName, '"');
    m_ouLDFParserHelper.AddChannelName(strChannelName);
  
  } 
  ;

version
returns [double dVersion]
@init {dVersion = 0.0;}
:
   StringLiteral 
   {
    std::string strProtocolVer = $StringLiteral.text;
    m_ouLDFParserHelper.Trim(strProtocolVer, '"');
    dVersion = strtod(strProtocolVer.c_str(), nullptr);
   }
   | 
   Float 
   {
    dVersion = strtod($Float.text.c_str(), nullptr);
   }
; 

lin_speed returns [double dSpeed]
: integerOrFloat
{
  dSpeed = $integerOrFloat.dFloat;
}
;

integerOrFloat
returns [double dFloat]
@init {dFloat = 0.0;}
:
  integer
  {
    dFloat = $integer.nInteger; 
  } 
  | Float
  {
    dFloat = strtod($Float.text.c_str(), nullptr);
  }
;


node_def:
  'Nodes' '{' master_def slaves_def '}';

master_def:
  'Master' ':' Identifier ',' time_base_def ',' jitter_def ';'
  {
  m_ouLDFParserHelper.AddMaster($Identifier.text, $time_base_def.dTimeBase, $jitter_def.dJitterBase);
  } 
;

time_base_def returns [double dTimeBase]
: integerOrFloat 'ms'
{
  dTimeBase = $integerOrFloat.dFloat;
}
;

jitter_def returns [double dJitterBase]
: integerOrFloat 'ms'
{
  dJitterBase = $integerOrFloat.dFloat;
}
;
 
slaves_def:
  'Slaves' ':' nodeList ';'  
  {
  m_ouLDFParserHelper.AddSlaves($nodeList.strNodeList);
  }
  ;
  
identifierList returns [std::list<std::string> ouStringList]
@init {ouStringList.clear();}
:
   strTemp = Identifier{ouStringList.push_back($strTemp.text);}  (',' strTemp2 = Identifier {ouStringList.push_back($strTemp2.text);} )*;
 

diag_addr_def:
  | /*optional*/
  'Diagnostic_addresses' '{' node_address_list '}' ;
 
node_address_list:
  (
  Identifier ':' integer ';'
  {
  m_ouLDFParserHelper.AddNodeDiagAddress($Identifier.text, $integer.nInteger);
  }
  )+
; 

node_composition_def:
  | /*optional*/
  'composite' '{' configuration+ '}' ;

configuration 
@init{std::list<ldfNodeComposition> ouNodeConfigurationList;}
:
  'configuration' Identifier '{' (composite_node{ouNodeConfigurationList.push_back($composite_node.ouNodeConfiguration);})* '}'
{
  m_ouLDFParserHelper.AddNodeConfiguration($Identifier.text, ouNodeConfigurationList);
};
  
composite_node returns [ldfNodeComposition ouNodeConfiguration] :
  Identifier '{' nodeList '}'
  {
   ouNodeConfiguration.m_strEcuName = $Identifier.text;
   ouNodeConfiguration.m_strLogicalNodeList = $nodeList.strNodeList;;
  }
;   

signal_def:
  'Signals' '{' signal* '}';  

signal:
  Identifier ':' size ',' init_value ',' published_by subscribed_by  ';'
  {
  ldfsignalDefinition ouldfsignalDefinition;

  ouldfsignalDefinition.m_strName = $Identifier.text;
  ouldfsignalDefinition.m_nSize = $size.nSize;
  ouldfsignalDefinition.m_strPublisher = $published_by.strPublisher;
  ouldfsignalDefinition.m_unInitValue = $init_value.ouSignalValue.m_un64Value;
  ouldfsignalDefinition.m_ouValueType = $init_value.ouSignalValue.m_bScalar ? eScalar : eByteArray;
  ouldfsignalDefinition.m_strSuscriberList = $subscribed_by.m_strSubscriberList;
  ouldfsignalDefinition.m_ouSignalType = eSignalNormal;

  m_ouLDFParserHelper.AddSignal(ouldfsignalDefinition);
  };

 
subscribed_by returns [std::list<std::string> m_strSubscriberList]
:
  (',' nodeList { m_strSubscriberList = $nodeList.strNodeList; })?
;

init_value returns [signalInitialValue ouSignalValue]
@init {ouSignalValue.m_bScalar = false;
		ouSignalValue.m_un64Value = 0;}
:
scalar_value
{
  ouSignalValue.m_bScalar = true;
  ouSignalValue.m_un64Value = $scalar_value.nScalarValue;
}
|vector_values
{
  ouSignalValue.m_un64Value = 0;
  ouSignalValue.m_bScalar = false;
  for ( auto itr : $vector_values.m_ouVlaueList)
  {
    ouSignalValue.m_un64Value = ouSignalValue.m_un64Value*256 + itr; 
  }

}
;


vector_values returns [std::list<unsigned int> m_ouVlaueList]
:
  '{' nTemp1 = integer { m_ouVlaueList.push_back(nTemp1);}  (',' nTemp2 = integer {m_ouVlaueList.push_back(nTemp2);} )* '}';


diag_signal_def:
  'Diagnostic_signals' '{' diag_signal* '}'  ;

diag_signal:
  Identifier ':' size ',' scalar_value ';'
  {
  ldfsignalDefinition ouldfsignalDefinition;

  ouldfsignalDefinition.m_strName = $Identifier.text;
  ouldfsignalDefinition.m_nSize = $size.nSize;
  ouldfsignalDefinition.m_strPublisher.clear();
  ouldfsignalDefinition.m_unInitValue = 0;
  ouldfsignalDefinition.m_strSuscriberList.clear();
  ouldfsignalDefinition.m_ouSignalType = eSignalDiag;
  ouldfsignalDefinition.m_ouValueType = eScalar;
  m_ouLDFParserHelper.AddSignal(ouldfsignalDefinition);
  }
  ;

dynamic_frame_def:
  'dynamic_frames' vector_values
{
  m_ouLDFParserHelper.AddDynamicFrames($vector_values.m_ouVlaueList);
};

frame_def:
  'Frames' '{' frame* '}'  ;
  
frame
@init {ldfFrameDefinition ouldfFrameDefinition; ouldfFrameDefinition.m_unSize = 8 ;}
:
  frame_Name ':' frame_Id ',' published_by (',' size { ouldfFrameDefinition.m_unSize = $size.nSize; } )? '{' frame_signal_pos_list '}'
  {
  
  std::map<std::string, int> m_mapSignalPos; 
  
  ouldfFrameDefinition.m_strName = $frame_Name.strFrameName;
  ouldfFrameDefinition.m_unId = $frame_Id.nFrameId;
  ouldfFrameDefinition.m_strPublisher = $published_by.strPublisher;
  
  ouldfFrameDefinition.m_mapSignalPos = $frame_signal_pos_list.m_MapSignalPos;
    m_ouLDFParserHelper.AddMessage(ouldfFrameDefinition);
  }
;

frame_signal_pos_list returns [std::map<std::string, int> m_MapSignalPos]
:
(signal_name ',' offset ';' {m_MapSignalPos[$signal_name.strSignalName] = static_cast<unsigned int>($offset.dOffset); }  )*
;

   
sporadic_frame_def:
   'Sporadic_frames' '{' sporadic_frame* '}' ;

sporadic_frame:
   sporadic_frame_name ':' nodeList ';'
   {
    ldfSporadicFrameDef ouldfSporadicFrameDef;
    ouldfSporadicFrameDef.m_strName = $sporadic_frame_name.strSporadicFrameName;
    ouldfSporadicFrameDef.m_FrameList = $nodeList.strNodeList;
    m_ouLDFParserHelper.AddMessage(ouldfSporadicFrameDef);
   }
   ;


event_triggered_frame_def:
  'Event_triggered_frames' '{' event_triggered_frame* '}' ;

event_triggered_frame
@init{ldfEventTriggerdFrame ouldfEventTriggerdFrame;}:
  event_trig_frm_name ':' ((collision_resolving_schedule_table)? ',' {ouldfEventTriggerdFrame.m_strScheduleTable = $collision_resolving_schedule_table.strTableName;})? frame_Id ( ',' frame_list {ouldfEventTriggerdFrame.m_strAssociatedFrames = $frame_list.strFrameList;} )? ';' 
  {
  
  ouldfEventTriggerdFrame.m_strName = $event_trig_frm_name.strEventFrameName;
  ouldfEventTriggerdFrame.m_unId = $frame_Id.nFrameId;
  
  m_ouLDFParserHelper.AddMessage(ouldfEventTriggerdFrame);
  };   

diag_frame_def:
 'Diagnostic_frames' '{' masterReqFrame slaveReqFrame '}';
 
masterReqFrame:
 'MasterReq' ':' frame_Id '{' frame_signal_pos_list '}'
 {
  ldfDiagFrame ouldfDiagFrame;
  ouldfDiagFrame.m_strName = "MasterReq";
  ouldfDiagFrame.m_unId = $frame_Id.nFrameId;
  ouldfDiagFrame.m_strAssociatedSignals = $frame_signal_pos_list.m_MapSignalPos;
  m_ouLDFParserHelper.AddMessage(ouldfDiagFrame);
 };

slaveReqFrame:
  'SlaveResp' ':'  frame_Id '{' frame_signal_pos_list '}' 
  {
  ldfDiagFrame ouldfDiagFrame;
  ouldfDiagFrame.m_strName = "SlaveResp";
  ouldfDiagFrame.m_unId = $frame_Id.nFrameId;
  ouldfDiagFrame.m_strAssociatedSignals = $frame_signal_pos_list.m_MapSignalPos;
  m_ouLDFParserHelper.AddMessage(ouldfDiagFrame);
 };  



node_attributes_def:

  'Node_attributes' '{' node_attributes+ '}';
node_attributes returns [ldfNodeProperties ouNodeProperties]
:
  Identifier '{' 
  'LIN_protocol' '=' version';'
  {
    ouNodeProperties.m_dProtocolVer = $version.dVersion;
  }
  
  'configured_NAD' '=' id1 =  diag_address';'
  {
  ouNodeProperties.m_unInitialNAD = $id1.nInteger;	//In case if the initail NAD is
													//not defined initai nad is same as config nad
  ouNodeProperties.m_unConfigNAD = $id1.nInteger;
  }

  (
    'initial_NAD' '=' id2 = diag_address';'
    {
    ouNodeProperties.m_unInitialNAD = $id2.nInteger;
    }
  )?

  (
    'product_id' '=' supplier_id ',' function_id (',' variant { ouNodeProperties.m_unVariant = $variant.nInteger; })? ';'
    {
    ouNodeProperties.m_unSupplierId = $supplier_id.nInteger;
    ouNodeProperties.m_unFunctionId = $function_id.nInteger;
    }
  )?

  (
    'response_error' '=' signal_name ';'
    {
    ouNodeProperties.m_strRespErrSigName = $signal_name.strSignalName;
    }
  )?

  (
  'fault_state_signals' '=' signal_name_list ';'
  {
    ouNodeProperties.m_strFaultSignalList = $signal_name_list.strSignalList;
  }
  )?


  (
  'P2_min' '='  dVal1 = real_or_integer 'ms' ';'
  {
    ouNodeProperties.m_dP2Min =  $dVal1.dDouble;
  }
  )?
  
  (
  'ST_min' '=' dVal2 = real_or_integer 'ms' ';'
  {
    ouNodeProperties.m_dSTMin = $dVal2.dDouble;
  }
  )?

   (
  'N_As_timeout' '=' dVal3 = real_or_integer 'ms' ';'
  {
    ouNodeProperties.m_dN_As_timeout = $dVal3.dDouble;
  }
  )?

   (
  'N_Cr_timeout' '=' dVal4 = real_or_integer 'ms' ';'
  {
    ouNodeProperties.m_dN_Cr_timeout = $dVal4.dDouble;
  }
  )?
  configframe_def
  {
  ouNodeProperties.FrameToIdMap = $configframe_def.FrameToIdMap;
  }
  '}'
  {
  ouNodeProperties.m_strName = $Identifier.text;
   m_ouLDFParserHelper.AddNodeAttributes(ouNodeProperties);
  }
;


configframe_def returns [std::list<FrameIdPos> FrameToIdMap]
@init { unsigned int unMessageId = 0x0; unsigned int unIndex = 0; }
:
| //optional in 1.x
'configurable_frames' 
'{'
(
frame_Name ('=' message_id { unMessageId = $message_id.nInteger;})? ';'
{
  FrameIdPos ouFrameIdPos;
  ouFrameIdPos.unId = unMessageId;
  ouFrameIdPos.unIndex = unIndex++;
  ouFrameIdPos.m_strFrameName = $frame_Name.strFrameName;
  FrameToIdMap.push_back(ouFrameIdPos);
}
)*
 
'}'
;


schedule_table_def:
'Schedule_tables' '{'
  (scheduletable{ m_ouLDFParserHelper.AddScheduleTable($scheduletable.ouScheduleTable); })*
'}'
;

scheduletable returns [ldfScheduleTable ouScheduleTable]:
Identifier 
'{'
    (
  command 'delay' frame_time 'ms' ';'
  {
    $command.ouScheduleCommand.m_dDelay = $frame_time.nDouble;
    ouScheduleTable.m_ouCommandList.push_back($command.ouScheduleCommand);
  }
  )*
'}'
{
  ouScheduleTable.m_strTableName = $Identifier.text;
}
;

command returns [ldfScheduleTableCommand ouScheduleCommand]:
 frame_Name { ouScheduleCommand.m_strCommandName = $frame_Name.strFrameName; ouScheduleCommand.m_eDiagType = eLIN_NORMAL_FRAME_ID; }
|'MasterReq' { ouScheduleCommand.m_strCommandName = "MasterReq"; ouScheduleCommand.m_eDiagType = eLIN_MASTER_FRAME_ID; }
|'SlaveResp' { ouScheduleCommand.m_strCommandName = "SlaveResp"; ouScheduleCommand.m_eDiagType = eLIN_SLAVE_FRAME_ID; }
| assignNAD  { ouScheduleCommand = $assignNAD.ouScheduleCommand;}
| conditionalChangeNAD { ouScheduleCommand = $conditionalChangeNAD.ouScheduleCommand;}
| dataDump { ouScheduleCommand = $dataDump.ouScheduleCommand;}
| saveConfiguration { ouScheduleCommand = $saveConfiguration.ouScheduleCommand;}
| assignFrameIdRange { ouScheduleCommand = $assignFrameIdRange.ouScheduleCommand;}
| freeFormat { ouScheduleCommand = $freeFormat.ouScheduleCommand;}
| assignFrameId { ouScheduleCommand = $assignFrameId.ouScheduleCommand;}
| unAssignFrameId { ouScheduleCommand = $unAssignFrameId.ouScheduleCommand;}                              //2.0
;

unAssignFrameId returns [ldfScheduleTableCommand ouScheduleCommand]
: 'unAssignFrameId' '{' node_name ',' frame_Name '}'
{
  ouScheduleCommand.m_eDiagType = eLIN_SID_UNASSIGN_FRAME_ID;
  ouScheduleCommand.m_strCommandName = "unAssignFrameId";
  ouScheduleCommand.m_strNodeName = $node_name.strNodeName;
  ouScheduleCommand.m_strFrameName = $frame_Name.strFrameName;
}
;
assignNAD returns [ldfScheduleTableCommand ouScheduleCommand]            
:   'AssignNAD' '{' node_name '}' 
{
  ouScheduleCommand.m_strCommandName = "AssignNAD";
  ouScheduleCommand.m_eDiagType = eLIN_SID_ASSIGN_NAD_ID;
  ouScheduleCommand.m_strNodeName = $node_name.strNodeName;
}
|
'AssignNAD' '{' old_NAD ',' new_NAD ',' supplier_id ',' function_id '}'
{
  ouScheduleCommand.m_strCommandName = "AssignNAD";
  ouScheduleCommand.m_eDiagType = eLIN_SID_ASSIGN_NAD_ID;
  ouScheduleCommand.m_chDataBytes[0] = $old_NAD.nInteger;
  ouScheduleCommand.m_chDataBytes[1] = $new_NAD.nInteger;
  ouScheduleCommand.m_chDataBytes[2] = $supplier_id.nInteger;
  ouScheduleCommand.m_chDataBytes[3] = $function_id.nInteger;
  ouScheduleCommand.m_chDataBytes[4] = 0;
  ouScheduleCommand.m_chDataBytes[5] = 0;
  ouScheduleCommand.m_chDataBytes[6] = 0;
  ouScheduleCommand.m_chDataBytes[7] = 0;
}
;
conditionalChangeNAD  returns [ldfScheduleTableCommand ouScheduleCommand]
:   'ConditionalChangeNAD' '{' nAD ',' id ',' byte ',' mask ',' inv ',' new_NAD '}' 
{
  ouScheduleCommand.m_strCommandName = "ConditionalChangeNAD";
  
  ouScheduleCommand.m_eDiagType = eLIN_SID_COND_CHANGE_NAD;
  ouScheduleCommand.m_chDataBytes[0] = $nAD.nInteger;
  ouScheduleCommand.m_chDataBytes[1] = $id.nInteger;
  ouScheduleCommand.m_chDataBytes[2] = $byte.nInteger;
  ouScheduleCommand.m_chDataBytes[3] = $mask.nInteger;
  ouScheduleCommand.m_chDataBytes[4] = $inv.nInteger;
  ouScheduleCommand.m_chDataBytes[5] = $new_NAD.nInteger;
  ouScheduleCommand.m_chDataBytes[6] = 0;
  ouScheduleCommand.m_chDataBytes[7] = 0;
}
;
dataDump returns [ldfScheduleTableCommand ouScheduleCommand]
: 'DataDump' '{' node_name',' d1 ',' d2 ',' d3 ',' d4 ',' d5 '}'
{
  ouScheduleCommand.m_strCommandName = "DataDump";
  
  ouScheduleCommand.m_eDiagType = eLIN_SID_DATA_DUMP;
  ouScheduleCommand.m_strNodeName = $node_name.strNodeName;
  ouScheduleCommand.m_chDataBytes[0] = $d1.nInteger;
  ouScheduleCommand.m_chDataBytes[1] = $d2.nInteger;
  ouScheduleCommand.m_chDataBytes[2] = $d3.nInteger;
  ouScheduleCommand.m_chDataBytes[3] = $d4.nInteger;
  ouScheduleCommand.m_chDataBytes[4] = $d5.nInteger;
  ouScheduleCommand.m_chDataBytes[5] = 0;
  ouScheduleCommand.m_chDataBytes[6] = 0;
  ouScheduleCommand.m_chDataBytes[7] = 0;
};

saveConfiguration     returns [ldfScheduleTableCommand ouScheduleCommand]
:   'SaveConfiguration' '{' node_name '}'
{
  ouScheduleCommand.m_strCommandName = "SaveConfiguration";

  ouScheduleCommand.m_eDiagType = eLIN_SID_SAVE_CONFIG;
  ouScheduleCommand.m_strNodeName = $node_name.strNodeName;
  memset(ouScheduleCommand.m_chDataBytes, 0, sizeof(ouScheduleCommand.m_chDataBytes));
};

assignFrameIdRange    returns [ldfScheduleTableCommand ouScheduleCommand]
@init { memset(ouScheduleCommand.m_chDataBytes, 0xFF, sizeof(ouScheduleCommand.m_chDataBytes));}
:   'AssignFrameIdRange'  '{' node_name ',' frame_index (',' PID1 = frame_PID {ouScheduleCommand.m_chDataBytes[1] = $PID1.nInteger;} ',' PID2 = frame_PID {ouScheduleCommand.m_chDataBytes[2] = $PID2.nInteger;} ',' 
                                                             PID3 = frame_PID {ouScheduleCommand.m_chDataBytes[3] = $PID3.nInteger;} ',' PID4 = frame_PID {ouScheduleCommand.m_chDataBytes[4] = $PID4.nInteger;})? '}'
{
  ouScheduleCommand.m_strCommandName = "AssignFrameIdRange";

  ouScheduleCommand.m_eDiagType = eLIN_SID_ASSIGN_FRAME_RANGE;
  ouScheduleCommand.m_strNodeName = $node_name.strNodeName;
  ouScheduleCommand.m_chDataBytes[0] = $frame_index.nInteger;
  ouScheduleCommand.m_chDataBytes[5] = 0;
  ouScheduleCommand.m_chDataBytes[6] = 0;
  ouScheduleCommand.m_chDataBytes[7] = 0;
}
;

freeFormat returns [ldfScheduleTableCommand ouScheduleCommand]           
:   'FreeFormat' '{' d1 ',' d2 ',' d3 ',' d4 ',' d5 ',' d6 ',' d7 ',' d8 '}'
{
  ouScheduleCommand.m_strCommandName = "FreeFormat";

  ouScheduleCommand.m_eDiagType = eLIN_SID_FREEFORMAT;
  ouScheduleCommand.m_chDataBytes[0] = $d1.nInteger;
  ouScheduleCommand.m_chDataBytes[1] = $d2.nInteger;
  ouScheduleCommand.m_chDataBytes[2] = $d3.nInteger;
  ouScheduleCommand.m_chDataBytes[3] = $d4.nInteger;
  ouScheduleCommand.m_chDataBytes[4] = $d5.nInteger;
  ouScheduleCommand.m_chDataBytes[5] = $d6.nInteger;
  ouScheduleCommand.m_chDataBytes[6] = $d7.nInteger;
  ouScheduleCommand.m_chDataBytes[7] = $d8.nInteger;

  if ( ouScheduleCommand.m_chDataBytes[2] == defLIN_SID_READ_BY_IDENTIFIER )
  {
  ouScheduleCommand.m_eDiagType = eLIN_SID_READ_BY_IDENTIFIER;
  }

}; 

assignFrameId  returns [ldfScheduleTableCommand ouScheduleCommand]        
:   'AssignFrameId' '{' node_name ',' frame_Name '}'
{
  ouScheduleCommand.m_strCommandName = "AssignFrameId";

  ouScheduleCommand.m_eDiagType = eLIN_SID_ASSIGN_FRAME_ID;
  ouScheduleCommand.m_strNodeName = $node_name.strNodeName;
  ouScheduleCommand.m_strFrameName = $frame_Name.strFrameName;
  memset(ouScheduleCommand.m_chDataBytes, 0, sizeof(ouScheduleCommand.m_chDataBytes));
};


signal_groups_def:
 | //Optional
 'Signal_groups' 
 '{' 
    signal_group* 
 '}';

signal_group
@init {std::map<std::string, unsigned int> mapSignalOffset;}
:
signal_group_name ':' group_size 
'{'
    (
  signal_name ',' group_offset ';'
  {
  mapSignalOffset[$signal_name.strSignalName] = $group_offset.nInteger;
  }
  )*
'}'
{
  m_ouLDFParserHelper.AddSignalGroup($signal_group_name.text, $group_size.nInteger, mapSignalOffset);
}
;


signal_encoding_type_def:
 |     //Optional
 'Signal_encoding_types' 
 '{'
      encoding_types*
 '}';

encoding_types returns [LINCompuMethods ouCompuMethod]:
signal_encoding_type_name 
'{'
  (coding_type
  {
    ouCompuMethod.m_bBCD = $coding_type.ouCompuMethod.m_bBCD;
    ouCompuMethod.m_bASCII = $coding_type.ouCompuMethod.m_bASCII;
    for ( auto itr : $coding_type.ouCompuMethod.m_ouPhysicalValueList )
  {
    ouCompuMethod.m_ouPhysicalValueList.push_back(itr); 
  }
  for ( auto itr : $coding_type.ouCompuMethod.m_ouLogicalValueList )
  {
    ouCompuMethod.m_ouLogicalValueList[itr.first] = itr.second; 
  }
  })*
'}'
{
m_ouLDFParserHelper.AddSignalCompuMethod($signal_encoding_type_name.text, ouCompuMethod);
}
;

coding_type returns [ LINCompuMethods ouCompuMethod ]:
  logical_value
  {
   m_ouLDFParserHelper.Trim($logical_value.ouLogicalValue.m_strTextInfo, '"');
  ouCompuMethod.m_ouLogicalValueList[$logical_value.ouLogicalValue.m_unVale] =  $logical_value.ouLogicalValue.m_strTextInfo;
  }
  | 
  physical_range
  {
  ouCompuMethod.m_ouPhysicalValueList.push_back($physical_range.ouPhycalValue);
  } 
  | 
  bcd_value
  {
  ouCompuMethod.m_bBCD = true;
  } 
  | 
  ascii_value
  {
  ouCompuMethod.m_bASCII = true;
  }
;

logical_value returns [ldfLogicalValue ouLogicalValue]
: 'logical_value' ',' signal_value (',' text_info)? ';'
{
  ouLogicalValue.m_unVale = $signal_value.nInteger;
  ouLogicalValue.m_strTextInfo = $text_info.text;
};


physical_range  returns [PhysicalValue ouPhycalValue]
: 'physical_value' ','  min_value ',' max_value ',' scale ',' offset (',' text_info)?';'
{
  ouPhycalValue.m_unMin = $min_value.nInteger;
  ouPhycalValue.m_unMax = $max_value.nInteger;
  ouPhycalValue.m_dFactor = $scale.dScale;
  ouPhycalValue.m_dOffset = $offset.dOffset;
  std::string strUnit =  $text_info.text;
  m_ouLDFParserHelper.Trim(strUnit, '"');
  ouPhycalValue.m_strTextInfo = strUnit;

};
bcd_value : 'bcd_value' ';';

ascii_value     : 'ascii_value' ';';


signal_representation_def:
| //optional
'Signal_representation' '{'
 (
 signal_encoding_type_name ':' signal_name_list ';'
 {
  m_ouLDFParserHelper.MapCompuMethodToSignals($signal_encoding_type_name.text, $signal_name_list.strSignalList);
 }
 )* 
'}'
;

//Alias rules

min_value returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

max_value returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

scale returns [double dScale]
@init { $dScale = 0;}              
: real_or_integer
{
  dScale = $real_or_integer.dDouble;
};

offset returns [double dOffset]
@init { $dOffset = 0;}              
: real_or_integer
{
  dOffset = $real_or_integer.dDouble;
};


text_info           : StringLiteral;



signal_group_name   : Identifier;
signal_value returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

frame_index returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;


old_NAD returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

new_NAD returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

inv returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

mask returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

byte returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

id returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

nAD returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;




d1 returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

d2 returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

d3 returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

d4 returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

d5 returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

d6 returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

d7 returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

d8 returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;
group_size returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

group_offset returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;


frame_PID returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

node_name returns [std::string strNodeName]
@init {$strNodeName = ""; }
: 
Identifier
{
  strNodeName = $Identifier.text;
};

real_or_integer returns [double dDouble]
@init { $dDouble = 0; }     
: 
integer
{
  dDouble = $integer.nInteger;
} | 
Float
{
  dDouble = strtod($Float.text.c_str(), nullptr);
};


size returns [int nSize]
@init{$nSize = 0;}         
: integer
{
  nSize = $integer.nInteger;
}
;

published_by returns [std::string strPublisher]
@init {$strPublisher = ""; }
: 
Identifier
{
  $strPublisher = $Identifier.text;
};

frame_Name returns [std::string strFrameName]
@init {$strFrameName = ""; }
: 
Identifier
{
  $strFrameName = $Identifier.text;
};

scalar_value returns [int nScalarValue]
@init{$nScalarValue = 0;}         
: integer
{
  nScalarValue = $integer.nInteger;
}
;

frame_Id returns [int nFrameId]
@init{$nFrameId = 0;}         
: integer
{
  nFrameId = $integer.nInteger;
}
;

sporadic_frame_name returns [std::string strSporadicFrameName]
@init {$strSporadicFrameName = ""; }
: 
Identifier
{
  $strSporadicFrameName = $Identifier.text;
};


event_trig_frm_name returns [std::string strEventFrameName]
@init {$strEventFrameName = ""; }
: 
Identifier
{
  $strEventFrameName = $Identifier.text;
};

message_id returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

signal_name returns [std::string strSignalName]
@init {$strSignalName = ""; }
: 
Identifier
{
  $strSignalName = $Identifier.text;
};



nodeList returns [std::list<std::string> strNodeList]
: 
identifierList{ strNodeList = $identifierList.ouStringList; }
;

frame_list returns [std::list<std::string> strFrameList]
: 
identifierList{ strFrameList = $identifierList.ouStringList; }
;

diag_address returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

supplier_id returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

function_id returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;

variant returns [int nInteger]
@init{$nInteger = 0;}         
: integer
{
  nInteger = $integer.nInteger;
}
;


signal_name_list returns [std::list<std::string> strSignalList]
: 
identifierList{ strSignalList = $identifierList.ouStringList; }
;

frame_time returns [double nDouble]
@init { $nDouble = 0;}              
: real_or_integer
{
  nDouble = ($real_or_integer.dDouble);
};



collision_resolving_schedule_table returns [std::string strTableName]
@init {$strTableName = ""; }
: 
Identifier
{
  $strTableName = $Identifier.text;
};

signal_encoding_type_name           : Identifier;


//Tokens Required for LDF File Parsing

integer returns [int nInteger]
@init {$nInteger = 0;}
: 
DEC
{
  nInteger = strtoul($DEC.text.c_str(), nullptr, 10);
}
| 
HEX
{
  nInteger = strtoul($HEX.text.c_str(), nullptr, 16);
}
;

Float: 
   ('+' | '-')?FractionalConstant ExponentPart? FloatingSuffix?;       


        
StringLiteral :
  '"' (~('"'|'\n'|'\r'))* '"';

Identifier :
  Letter (Letter | Digit | '_')*;

Whitespace :
  (' ' | '\t' | '\f' | '\n' | '\r' )+ {$channel=nCommentChannel;};

comment:
  Comment
{
m_ouLDFParserHelper.AddComment($Comment.text);
} 
|
Whitespace
{
m_ouLDFParserHelper.AddComment($Whitespace.text);
}
  ; 
  
Comment returns [std::string strText]:
SingleLineComment
{
  strText = $SingleLineComment.text;
} 
|
MultiLineComment 
{
  strText = $MultiLineComment.text;
}
;
fragment
SingleLineComment :
  '//' (~('\n'|'\r'))* ('\n'|'\r'('\n')?)? {$channel=nCommentChannel;};
fragment
MultiLineComment :
   '/*' ( options {greedy=false;} : . )* '*/' {$channel=nCommentChannel;};


fragment
FloatingSuffix
: 'f' | 'l' | 'F' | 'L'
;
        
fragment
ExponentPart
: 'e' Sign? DigitSequence
| 'E' Sign? DigitSequence
;
fragment
Sign
: '+' | '-'
;

fragment
FractionalConstant
: DigitSequence? '.' DigitSequence
| DigitSequence '.'
;

fragment
DigitSequence
: Digit+
;

DEC : Decimal;
HEX : Hex;
fragment
Decimal : 
  ('+' | '-')?(Digit)+;

fragment
Hex : 
  ('+' | '-')?HexadecimalPrefix (HexDigit)+;
  
fragment HexadecimalPrefix    : '0' ('x'|'X');     
fragment Digit                : '0'..'9';
fragment HexDigit             : 'a'..'f' | 'A'..'F' | Digit;
fragment Exponent             : ('e' | 'E') ('+' | '-')? (Digit)+;
fragment Letter               : 'a'..'z' | 'A'..'Z';
