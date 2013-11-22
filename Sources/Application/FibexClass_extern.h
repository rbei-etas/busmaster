#if !defined FIBEXCLASS_EXTERN_H__INCLUDED_
#define FIBEXCLASS_EXTERN_H__INCLUDED_

#include "DataTypes/Typecode_FBX_entities.h"
#include "DataTypes/FEALData.h"


#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif


    const int FCLASS_SUCCESS = 0;
    const int FCLASS_FAILURE = -1;
    const int VERSION_NOT_SUPPORTED = 1;
    const int NO_LICENSE_FOR_USAGE  = 2;

#define VERSION_NONE    _T("")
#define VERSION_1   _T("1")//.2.0a")
#define VERSION_2   _T("2")//.0.1")
#define VERSION_3_0 _T("3.0")
#define VERSION_3_1 _T("3.1")
#define VERSION_3_0_0   _T("3.0.0")
#define VERSION_2_0_1   _T("2.0.1")

    /*  Exported function list */
    /*USAGEMODE HRESULT GetPARSER_FIBEX(void** ppvInterface);*/

#ifdef __cplusplus
}
#endif

#endif // FIBEXCLASS_EXTERN_H__INCLUDED_
