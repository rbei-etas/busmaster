#pragma once

#include "DataTypes/Typecode_FBX_entities.h"
#include "DataTypes/FEALData.h"

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE __declspec(dllexport)
#else
#define USAGEMODE __declspec(dllimport)
#endif

const int FCLASS_SUCCESS = 0;
const int FCLASS_FAILURE = -1;
const int VERSION_NOT_SUPPORTED = 1;
const int NO_LICENSE_FOR_USAGE  = 2;

#define VERSION_NONE ""
#define VERSION_1 "1" //.2.0a"
#define VERSION_2 "2" //.0.1"
#define VERSION_2_0_1 "2.0.1"
#define VERSION_3_0 "3.0"
#define VERSION_3_0_0 "3.0.0"
#define VERSION_3_1 "3.1"

#ifdef __cplusplus
}
#endif
