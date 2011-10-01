#ifndef __ocicfg_container_h__
#define __ocicfg_container_h__

#include <ocig.h>

typedef void* OCI_ConfigHandle;
typedef void* OCIG_ParameterHandle;
/**
 status or filter information for configuration parameter. This structure will return status information in
 functions with write access to the configuration container and will be used for filter purposes on functions
 using read access.
 */
typedef enum OCI_ConfigUsage
{
   /** One or more parameter will be used by the controller and has a valid value. */
   validUsedParameter = 1,
   /** One or more parameter is required by the controller and has no valid value. Generation of an
       image of the container contents for the OCIG_OpenController() function will fail. */
   invalidUsedParameter = 2,
   /** One or more parameter has a valid value, but will not be used. The parameter are a member of an array 
       and the index is out of the range, that is limited by a length_is() parameter. Modification of the
       parameter defining the array length may activate the parameter. */
   validUnusedParameter = 4,
   /** One or more parameter has no valid value and will not be used. The parameter are a member of an array 
       and the index is out of the range, that is limited by a length_is() parameter. Modification of the
       parameter defining the array length may activate the parameter. */
   invalidUnusedParameter = 8,
   /** One or more parameter has a vaild value, but will be ignored by the underlaying driver layer. */
   validIgnoredParameter = 16,
   /** One or more parameter has no vaild value and will be ignored by the underlaying driver layer. */
   invalidIgnoredParameter = 32
} OCI_ConfigUsage;

/**
 
 */
typedef struct OCI_ConfigParameter
{
   char            name[OCIG_DICTIONARY_IDENT_LENGTH];
   char            value[OCIG_DICTIONARY_VALUE_LENGTH];
   OCI_ConfigUsage usage;

} OCI_ConfigParameter;

/**
 File format of ConfigContainer.

 The ConfigContainer uses a C/C++ like representation of the used parameter. It uses a format, that is simple
 to understand, parse and generate. 
 
 A formal description in EBNF is:

 <ConfigContainerFile> ::= {<line>}
 <line>                ::= [<parameterName> '=' <value> ';'][<comment>] '\n'
 <parameterName>       ::= <simpleName> { '.' <simpleName> }
 <simpleName>          ::= <nondigit> {<alphanum>} [<array>] }
 <value>               ::= <integer> | <unsigned> | <float> |<string> | '<invalid>'
 <comment>             ::= '//' {<char>}
 <integer>             ::= [ '+' | '-' ] <digit> {<digit>}
 <unsigned>            ::= [ '+' ] <digit> {<digit>}
 <float>               ::= <integer> [ '.' {digit} ] [ ('e' | 'E') <integer> ]
 <string>              ::= '"' {<char>} '"'
 <array>               ::= '[' <unsigned> ']'
 <char>                ::= <alphanum> | <space> | 
                             '+' | '-' | '*' | '/' | '.' | '#' | '~' | '=' |
                             '>' | '<' | '(' | ')' | '[' | ']' | '{' | '}' | '|'
 <alphanum>            ::= <nondigit>|<digit>
 <nondigit>            ::= '_'|<alphachar>
 <alphachar>           ::= 'a' | 'b' | 'c' | .. | 'z' | 'A' | 'B' | 'C' | .. | 'Z'
 <digit>               ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
 <space>               ::= ' ' | '\t'

 Examples:

 //
 // This is a line consisting of a comment.
 // The following line is 'empty'

 // The following line is a simple assignment
 name1 = 1;
 name2 = 2;                      //A simple assignment (integer or unsigned) with comment
 name.part1 = "Hello world";     //A structured name (string)
 name.part2[4].item = 123;       //Another structured name
 name.part2[3].item=17;          //white space between the elements are ignored
 name.part2[0].item = <invalid>  //an element with an <invalid> value
 */

/**
 Create a ConfigurationContainer for a specific controller. This call reads the data structure required by the
 controller and generates a container with the parameter and - when available - default values. Some parameter
 may have no valid default values and must be set before the container can be used to open a controller.
 */
OCI_ErrorCode OCIG_CreateConfigContainer (OCI_ControllerHandle controller, OCI_ConfigHandle* cfg);

/**
 Destroy the Configuration container and release all required resources.
 */
OCI_ErrorCode OCIG_DestroyConfigContainer (const OCI_ConfigHandle cfg);

/**
 Set some parameter of a configuration container. Parameter, that are not used by the controller will be added to 
 the container, but ignored. The OCI_ConfigUsage will deliver information about the usage of the added parameter.
 */
OCI_ErrorCode OCIG_SetConfigParameter(const OCI_ConfigHandle cfg, 
                                      const char* name, const char* value, OCI_ConfigUsage* usage);

/**
 Write all parameter of a configuration container into a file. The file will include all parameter required by
 the controller and all parameter set by OCIG_ReadConfigParameterFromFile() and OCIG_SetConfigParameter(), even
 when they are ignored by the controller. This mechanism increases the portability of the generated configuration.
 Some future OCI-Version may require additional or different parameter. When the future version has valid defaults
 for the new parameter an old file can be used. On the other hand a new file can be used by an old implementation, 
 by ignoring the new values. The OCI_ConfigUsage will deliver information about the written file.
 */
OCI_ErrorCode OCIG_WriteConfigContainerToFile (const OCI_ConfigHandle cfg, const char* filename, OCI_ConfigUsage* usage);

/**
 Read some parameter or a complete configuration container from a file. Parameter, that are not used by the 
 controller will be added to the container, but ignored. Parameter in the container, that are not in the file are
 not accessed and will keep the current value and state. The OCI_ConfigUsage will deliver information about the 
 read parameter.
 */
OCI_ErrorCode OCIG_ReadConfigParameterFromFile(const OCI_ConfigHandle cfg, const char* filename, OCI_ConfigUsage* usage);

/**
 Get the number of parameter in the configuration container. The result can be filtered to limit it to 
 any combination of valid used, valid unused, valid ignored, invalid used, invalid unused and invalid ignored
 parameter.
 */
OCI_ErrorCode OCIG_GetParameterCount(const OCI_ConfigHandle cfg, const OCI_ConfigUsage filter, uint32* count);

/**
 Get the index-th parameter of the configuration container, that passes a filter condition and return a handle
 for further access to the parameter.
*/
OCI_ErrorCode OCIG_GetParameter(const OCI_ConfigHandle cfg, const OCI_ConfigUsage filter, const uint32 index,
                                OCIG_ParameterHandle* param);
/**
 Get a named parameter in the configuration container and return a handle for further access to the parameter.
*/
OCI_ErrorCode OCIG_FindConfigParameter(const OCI_ConfigHandle config, const char* name, 
                                       OCIG_ParameterHandle* param);


OCI_ErrorCode OCIG_ParameterGetName(const OCIG_ParameterHandle param, char* destination, int size);
OCI_ErrorCode OCIG_ParameterSetValue(const OCIG_ParameterHandle param, const char* value);
OCI_ErrorCode OCIG_InitController(const OCI_ControllerHandle controller, const OCI_ConfigHandle config,
                                  const void* properties, const int propertySize);

#endif //__ocicfg_container_h__