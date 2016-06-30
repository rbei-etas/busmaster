%{

#include <stdio.h>
#include "Definitions.h"

int nBus = BUS_CAN;
int nProtocal = NONE;
int FindProtoCol(char* file)
{
yyin = fopen(file, "r");
yylex();
return 0;
}
%}
PROTOCOLTYPE ("\"ProtocolType\"")
BUSTYPE ("\"BusType\"")
WS			 [ \t\n]
TOKEN_BA	 ("BA_")
TOKEN_BA_DEF ("BA_DEF_DEF_")
TOKEN_CAN	 ("\"CAN\"")
TOKEN_J1939  ("\"J1939\"")
TOKEN_J1939PGN	("\"J1939PGN\"")
NUMBER			[0-9]
%%
{TOKEN_BA}{WS}+{BUSTYPE}{WS}+{TOKEN_CAN}					nBus = BUS_CAN;
{TOKEN_BA}{WS}+{PROTOCOLTYPE}{WS}+{TOKEN_J1939}				nProtocal = PROTOCAL_J1939;
{TOKEN_BA_DEF}{WS}+{PROTOCOLTYPE}{WS}+{TOKEN_J1939}			nProtocal = PROTOCAL_J1939;
{TOKEN_BA}{WS}+{TOKEN_J1939PGN}{WS}+{NUMBER}{WS}*;				nProtocal = PROTOCAL_J1939;
.							;/* ignore Anything else */;
\n						;/* ignore Anything else */;
%%
int yywrap()
{
fclose(yyin);
return 1;
}