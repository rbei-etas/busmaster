/*****************************************************************************
 HMS Technology Center Ravensburg GmbH
******************************************************************************

 File    : VCISDK.H
 Summary : VCI software development kit.

 Date    : 2005-07-27
 Author  : Hartmut Heim

 Compiler: MSVC

******************************************************************************
 all rights reserved
*****************************************************************************/

#ifndef _VCISDK_H_
#define _VCISDK_H_

#include <windows.h>

#ifdef WIN32_LEAN_AND_MEAN
#include <objbase.h>
#endif //WIN32_LEAN_AND_MEAN

#include <vcierr.h>
#include <vciver.h>
#include <vcitype.h>

/*##########################################################################*/
/*##                                                                      ##*/
/*##     common declarations                                              ##*/
/*##                                                                      ##*/
/*##########################################################################*/

/*****************************************************************************
 * calling conventions
 ****************************************************************************/

#ifdef VCIAPI
#undef VCIAPI
#endif
#define VCIAPI __stdcall


/*##########################################################################*/
/*##                                                                      ##*/
/*##     First-In/First-Out (FIFO) specific declarations                  ##*/
/*##                                                                      ##*/
/*##########################################################################*/


/*****************************************************************************
 * Interface:
 *  IVciFifo
 *
 * Description:
 *  Common First-In/First-Out object interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE IVciFifo
DECLARE_INTERFACE_(IVciFifo, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ IVciFifo methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetCapacity )(THIS_ OUT PUINT16 pwCapacity)              PURE;
  STDMETHOD(GetEntrySize)(THIS_ OUT PUINT16 pwSize)                  PURE;
  STDMETHOD(GetFreeCount)(THIS_ OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(GetFillCount)(THIS_ OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(GetFillLevel)(THIS_ OUT PUINT16 pwLevel)                 PURE;
};
#undef INTERFACE

typedef IVciFifo* PVCIFIFO;

/*****************************************************************************
 * Interface ID of IVciFifo {25A79E68-912C-41BF-8CC7-A709672FF102}
 ****************************************************************************/
DEFINE_GUID(IID_IVciFifo,
       0x25A79E68,0x912C,0x41BF,0x8C,0xC7,0xA7,0x09,0x67,0x2F,0xF1,0x02);


/*****************************************************************************
 * Interface:
 *  IVciFifo2
 *
 * Description:
 *  This interface extend the FIFO interface by a reset function.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE IVciFifo2
DECLARE_INTERFACE_(IVciFifo2, IVciFifo)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ IVciFifo methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetCapacity )(THIS_ OUT PUINT16 pwCapacity)              PURE;
  STDMETHOD(GetEntrySize)(THIS_ OUT PUINT16 pwSize)                  PURE;
  STDMETHOD(GetFreeCount)(THIS_ OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(GetFillCount)(THIS_ OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(GetFillLevel)(THIS_ OUT PUINT16 pwLevel)                 PURE;
  //~~~ IVciFifo2 methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(Reset)(THIS)                                             PURE;
};
#undef INTERFACE

typedef IVciFifo2* PVCIFIFO2;

/*****************************************************************************
 * Interface ID of IVciFifo2 {66E3AA3A-6BF6-47A0-AB18-EFC801091E61}
 ****************************************************************************/
DEFINE_GUID(IID_IVciFifo2,
       0x66E3AA3A,0x6BF6,0x47A0,0xAB,0x18,0xEF,0xC8,0x01,0x09,0x1E,0x61);


/*****************************************************************************
 * Interface:
 *  IFifoReader
 *
 * Description:
 *  Interface for receive FIFO objects (device to host).
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE IFifoReader
DECLARE_INTERFACE_(IFifoReader, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ IFifoReader methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(Lock        )(THIS)                                      PURE;
  STDMETHOD(Unlock      )(THIS)                                      PURE;
  STDMETHOD(AssignEvent )(THIS_ IN HANDLE hEvent)                    PURE;
  STDMETHOD(SetThreshold)(THIS_ IN UINT16 wThreshold)                PURE;
  STDMETHOD(GetThreshold)(THIS_ OUT PUINT16 pwThreshold)             PURE;
  STDMETHOD(GetCapacity )(THIS_ OUT PUINT16 pwCapacity)              PURE;
  STDMETHOD(GetEntrySize)(THIS_ OUT PUINT16 pwSize)                  PURE;
  STDMETHOD(GetFillCount)(THIS_ OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(GetFreeCount)(THIS_ OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(GetDataEntry)(THIS_ OUT PVOID pvData)                    PURE;
  STDMETHOD(AcquireRead )(THIS_ OUT PVOID*  ppvData,
                                OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(ReleaseRead )(THIS_ IN UINT16 wCount)                    PURE;
};
#undef INTERFACE

typedef IFifoReader* PFIFOREADER;

/*****************************************************************************
 * Interface ID of IFifoReader {19286603-A9CD-433a-A495-72E5C9D97A97}
 ****************************************************************************/
DEFINE_GUID(IID_IFifoReader,
       0x19286603,0xA9CD,0x433A,0xA4,0x95,0x72,0xE5,0xC9,0xD9,0x7A,0x97);


/*****************************************************************************
 * Interface:
 *  IFifoWriter
 *
 * Description:
 *  Interface for transmit FIFO objects (host to device).
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE IFifoWriter
DECLARE_INTERFACE_(IFifoWriter, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ IFifoWriter methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(Lock        )(THIS)                                      PURE;
  STDMETHOD(Unlock      )(THIS)                                      PURE;
  STDMETHOD(AssignEvent )(THIS_ IN HANDLE hEvent)                    PURE;
  STDMETHOD(SetThreshold)(THIS_ IN UINT16 wThreshold)                PURE;
  STDMETHOD(GetThreshold)(THIS_ OUT PUINT16 pwThreshold)             PURE;
  STDMETHOD(GetCapacity )(THIS_ OUT PUINT16 pwCapacity)              PURE;
  STDMETHOD(GetEntrySize)(THIS_ OUT PUINT16 pwSize)                  PURE;
  STDMETHOD(GetFillCount)(THIS_ OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(GetFreeCount)(THIS_ OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(PutDataEntry)(THIS_ IN PVOID pvData)                     PURE;
  STDMETHOD(AcquireWrite)(THIS_ OUT PVOID*  ppvData,
                                OUT PUINT16 pwCount)                 PURE;
  STDMETHOD(ReleaseWrite)(THIS_ IN UINT16 wCount)                    PURE;
};
#undef INTERFACE

typedef IFifoWriter* PFIFOWRITER;

/*****************************************************************************
 * Interface ID of IFifoWriter {E1CC08CE-99C0-483B-8E5F-4C43358E1CE2}
 ****************************************************************************/
DEFINE_GUID(IID_IFifoWriter,
       0xE1CC08CE,0x99C0,0x483B,0x8E,0x5F,0x4C,0x43,0x35,0x8E,0x1C,0xE2);



/*##########################################################################*/
/*##                                                                      ##*/
/*##     device specific declarations                                     ##*/
/*##                                                                      ##*/
/*##########################################################################*/


/*****************************************************************************
 * Interface:
 *  IVciDevice
 *
 * Description:
 *  VCI device interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE  IVciDevice
DECLARE_INTERFACE_(IVciDevice, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ IVciDevice methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetDeviceInfo)(THIS_ OUT PVCIDEVICEINFO pInfo)           PURE;
  STDMETHOD(GetDeviceCaps)(THIS_ OUT PVCIDEVICECAPS pCaps)           PURE;
  STDMETHOD(OpenComponent)(THIS_ IN REFCLSID rcid,
                                 IN REFIID   riid, OUT PVOID* ppv)   PURE;
};
#undef INTERFACE

typedef IVciDevice* PVCIDEVICE;

/*****************************************************************************
 * Interface ID of IVciDevice {46DFCF2F-113B-4D01-A803-BD5B4D32C8F1}
 ****************************************************************************/
DEFINE_GUID(IID_IVciDevice,
       0x46DFCF2F,0x113B,0x4D01,0xA8,0x03,0xBD,0x5B,0x4D,0x32,0xC8,0xF1);


/*****************************************************************************
 * Interface:
 *  IDevStatus
 *
 * Description:
 *  VCI device status interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE  IDevStatus
DECLARE_INTERFACE_(IDevStatus, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ IDevStatus methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetDeviceInfo )(THIS_ OUT PVCIDEVICEINFO pInfo)          PURE;
  STDMETHOD(GetDeviceCaps )(THIS_ OUT PVCIDEVICECAPS pCaps)          PURE;
  STDMETHOD(GetLicenseInfo)(THIS_ OUT PVCILICINFO pInfo)             PURE;
  STDMETHOD(GetRuntimeInfo)(THIS_ OUT PVCIDEVRTINFO pInfo)           PURE;
};
#undef INTERFACE

typedef IDevStatus* PDEVSTATUS;

/*****************************************************************************
 * Interface ID of IDevStatus {B2485033-9FF6-407C-9C2F-2692FB68319C}
 ****************************************************************************/
DEFINE_GUID(IID_IDevStatus,
       0xB2485033,0x9FF6,0x407C,0x9C,0x2F,0x26,0x92,0xFB,0x68,0x31,0x9C);


/*****************************************************************************
 * Interface:
 *  IVciEnumDevice
 *
 * Description:
 *  VCI device enumerator interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE IVciEnumDevice
DECLARE_INTERFACE_(IVciEnumDevice, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ IVciEnumDevice methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(Next       )(THIS_ IN  UINT32         dwNumElem,
                               OUT PVCIDEVICEINFO paDevInfo,
                               OUT PUINT32        pdwFetched)        PURE;
  STDMETHOD(Skip       )(THIS_ IN UINT32 dwNumElem)                  PURE;
  STDMETHOD(Reset      )(THIS)                                       PURE;
  STDMETHOD(AssignEvent)(THIS_ IN HANDLE hEvent)                     PURE;
};
#undef INTERFACE

typedef IVciEnumDevice* PVCIENUMDEVICE;

/*****************************************************************************
 * Interface ID of IVciEnumDevice {1AD18613-CC67-4AC8-83FA-6AD24BDE629A}
 ****************************************************************************/
DEFINE_GUID(IID_IVciEnumDevice,
       0x1AD18613,0xCC67,0x4AC8,0x83,0xFA,0x6A,0xD2,0x4B,0xDE,0x62,0x9A);


/*****************************************************************************
 * Interface:
 *  IVciDeviceManager
 *
 * Description:
 *  VCI device manager interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE IVciDeviceManager
DECLARE_INTERFACE_(IVciDeviceManager, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ IVciDeviceManager methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(EnumDevices)(THIS_ OUT PVCIENUMDEVICE* ppEnum)           PURE;
  STDMETHOD(OpenDevice )(THIS_ IN  REFVCIID    rVciidDev,
                               OUT PVCIDEVICE* ppDevice)             PURE;

};
#undef INTERFACE

typedef IVciDeviceManager* PVCIDEVICEMANAGER;

/*****************************************************************************
 * Interface ID of IVciDeviceManager {7724B0A9-E6DB-46B0-A5D0-468CC4191732}
 ****************************************************************************/
DEFINE_GUID(IID_IVciDeviceManager,
       0x7724B0A9,0xE6DB,0x46B0,0xA5,0xD0,0x46,0x8C,0xC4,0x19,0x17,0x32);



/*##########################################################################*/
/*##                                                                      ##*/
/*##   Bus Access Layer (BAL) specific declarations                       ##*/
/*##                                                                      ##*/
/*##########################################################################*/

#include <baltype.h>
#include <cantype.h>
#include <lintype.h>

/*****************************************************************************
 * Component ID for VCI BAL objects {3CB880F4-2791-40BD-B852-09349D1A7B37}
 ****************************************************************************/
DEFINE_GUID(CLSID_VCIBAL,
       0x3CB880F4,0x2791,0x40BD,0xB8,0x52,0x09,0x34,0x9D,0x1A,0x7B,0x37);

/*****************************************************************************
 * Interface:
 *  IBalObject
 *
 * Description:
 *  Bus Access Layer object.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE  IBalObject
DECLARE_INTERFACE_(IBalObject, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ IBalObject methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetFeatures)(THIS_ OUT PBALFEATURES pFeatures)           PURE;
  STDMETHOD(OpenSocket )(THIS_ IN UINT32 dwBusNo,
                               IN REFIID riid, OUT PVOID* ppv)       PURE;
};
#undef INTERFACE

typedef IBalObject* PBALOBJECT;

/*****************************************************************************
 * Interface ID of IBalObject {A2B958AA-9188-4CDB-8307-25D9F1F843E9}
 ****************************************************************************/
DEFINE_GUID(IID_IBalObject,
       0xA2B958AA,0x9188,0x4CDB,0x83,0x07,0x25,0xD9,0xF1,0xF8,0x43,0xE9);


/*****************************************************************************
 * Interface:
 *  ICanControl
 *
 * Description:
 *  CAN control interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE ICanControl
DECLARE_INTERFACE_(ICanControl, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanControl methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(DetectBaud   )(THIS_ IN     UINT16        wTimeoutMs,
                                 IN OUT PCANBTRTABLE  pBtrTable)     PURE;
  STDMETHOD(InitLine     )(THIS_ IN PCANINITLINE pInit)              PURE;
  STDMETHOD(ResetLine    )(THIS)                                     PURE;
  STDMETHOD(StartLine    )(THIS)                                     PURE;
  STDMETHOD(StopLine     )(THIS)                                     PURE;
  STDMETHOD(GetLineStatus)(THIS_ OUT PCANLINESTATUS pStatus)         PURE;
  STDMETHOD(SetAccFilter )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask )                  PURE;
  STDMETHOD(AddFilterIds )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask)                   PURE;
  STDMETHOD(RemFilterIds )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask)                   PURE;
};
#undef INTERFACE
#define INTERFACE ICanControl2
DECLARE_INTERFACE_(ICanControl2, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanControl2 methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(DetectBaud   )(THIS_ IN     UINT8        bOpMode,
                                 IN     UINT8        bExMode,
                                 IN     UINT16       wTimeoutMs,
                                 IN OUT PCANBTPTABLE pBtpTable)      PURE;
  STDMETHOD(InitLine     )(THIS_ IN PCANINITLINE2 pInit)             PURE;
  STDMETHOD(ResetLine    )(THIS)                                     PURE;
  STDMETHOD(StartLine    )(THIS)                                     PURE;
  STDMETHOD(StopLine     )(THIS)                                     PURE;
  STDMETHOD(GetLineStatus)(THIS_ OUT PCANLINESTATUS2 pStatus)        PURE;
  STDMETHOD(GetFilterMode)(THIS_ IN  UINT8  bSelect,
                                 OUT PUINT8 pbMode)                  PURE;
  STDMETHOD(SetFilterMode)(THIS_ IN  UINT8  bSelect,
                                 IN  UINT8  bMode,
                                 OUT PUINT8 pbPrev)                  PURE;
  STDMETHOD(SetAccFilter )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask )                  PURE;
  STDMETHOD(AddFilterIds )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask)                   PURE;
  STDMETHOD(RemFilterIds )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask)                   PURE;
};
#undef INTERFACE

typedef ICanControl*  PCANCONTROL;
typedef ICanControl2* PCANCONTROL2;

/*****************************************************************************
 * Interface ID of ICanControl  {D213C143-7039-4D1D-ACED-1390289C8EBA}
 * Interface ID of ICanControl2 {EFD3A8E2-09E3-4115-869F-93E579167431}
 ****************************************************************************/
DEFINE_GUID(IID_ICanControl,
       0xD213C143,0x7039,0x4D1D,0xAC,0xED,0x13,0x90,0x28,0x9C,0x8E,0xBA);

DEFINE_GUID(IID_ICanControl2,
       0xEFD3A8E2,0x9E3,0x4115,0x86,0x9F,0x93,0xE5,0x79,0x16,0x74,0x31);


/*****************************************************************************
 * Interface:
 *  ICanChannel
 *
 * Description:
 *  CAN communication channel interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE ICanChannel
DECLARE_INTERFACE_(ICanChannel, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanChannel methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(Initialize)(THIS_ IN UINT16 wRxFifoSize,
                              IN UINT16 wTxFifoSize)                 PURE;
  STDMETHOD(Activate  )(THIS)                                        PURE;
  STDMETHOD(Deactivate)(THIS)                                        PURE;
  STDMETHOD(GetReader )(THIS_ OUT PFIFOREADER* ppReader)             PURE;
  STDMETHOD(GetWriter )(THIS_ OUT PFIFOWRITER* ppWriter)             PURE;
  STDMETHOD(GetStatus )(THIS_ OUT PCANCHANSTATUS pStatus)            PURE;
};
#undef  INTERFACE
#define INTERFACE ICanChannel2
DECLARE_INTERFACE_(ICanChannel2, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanChannel2 methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(Initialize   )(THIS_ IN UINT32 dwRxFifoSize,
                                 IN UINT32 dwTxFifoSize,
                                 IN UINT32 dwFilterSize,
                                 IN UINT8  bFilterMode)              PURE;
  STDMETHOD(Activate     )(THIS)                                     PURE;
  STDMETHOD(Deactivate   )(THIS)                                     PURE;
  STDMETHOD(GetReader    )(THIS_ OUT PFIFOREADER* ppReader)          PURE;
  STDMETHOD(GetWriter    )(THIS_ OUT PFIFOWRITER* ppWriter)          PURE;
  STDMETHOD(GetStatus    )(THIS_ OUT PCANCHANSTATUS2 pStatus)        PURE;
  STDMETHOD(GetControl   )(THIS_ OUT ICanControl2** ppCanCtrl)       PURE;
  STDMETHOD(GetFilterMode)(THIS_ IN  UINT8  bSelect,
                                 OUT PUINT8 pbMode)                  PURE;
  STDMETHOD(SetFilterMode)(THIS_ IN  UINT8  bSelect,
                                 IN  UINT8  bMode,
                                 OUT PUINT8 pbPrev)                  PURE;
  STDMETHOD(SetAccFilter )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask)                   PURE;
  STDMETHOD(AddFilterIds )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask)                   PURE;
  STDMETHOD(RemFilterIds )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask)                   PURE;
};
#undef INTERFACE

typedef ICanChannel*  PCANCHANNEL;
typedef ICanChannel2* PCANCHANNEL2;

/*****************************************************************************
 * Interface ID of ICanChannel  {66FF396B-22EA-42AA-BC7E-4919F89FB45C}
 * Interface ID of ICanChannel2 {F2528F90-3A10-46D0-8F82-0871C1E7DBB9}
 ****************************************************************************/
DEFINE_GUID(IID_ICanChannel,
       0x66FF396B,0x22EA,0x42AA,0xBC,0x7E,0x49,0x19,0xF8,0x9F,0xB4,0x5C);

DEFINE_GUID(IID_ICanChannel2,
       0xF2528F90,0x3A10,0x46D0,0x8F,0x82,0x08,0x71,0xC1,0xE7,0xDB,0xB9);


/*****************************************************************************
 * Interface:
 *  ICanScheduler
 *
 * Description:
 *  Cyclic CAN message scheduler interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE ICanScheduler
DECLARE_INTERFACE_(ICanScheduler, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanScheduler methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(Resume      )(THIS)                                      PURE;
  STDMETHOD(Suspend     )(THIS)                                      PURE;
  STDMETHOD(Reset       )(THIS)                                      PURE;
  STDMETHOD(GetStatus   )(THIS_ OUT PCANSCHEDULERSTATUS pStatus)     PURE;
  STDMETHOD(AddMessage  )(THIS_ IN  PCANCYCLICTXMSG pMessage,
                                OUT PUINT32         pdwIndex)        PURE;
  STDMETHOD(RemMessage  )(THIS_ IN UINT32 dwIndex)                   PURE;
  STDMETHOD(StartMessage)(THIS_ IN UINT32 dwIndex,
                                IN UINT32 dwCount)                   PURE;
  STDMETHOD(StopMessage )(THIS_ IN UINT32 dwIndex)                   PURE;
};
#undef INTERFACE
#define INTERFACE ICanScheduler2
DECLARE_INTERFACE_(ICanScheduler2, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanScheduler2 methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(Resume      )(THIS)                                      PURE;
  STDMETHOD(Suspend     )(THIS)                                      PURE;
  STDMETHOD(Reset       )(THIS)                                      PURE;
  STDMETHOD(GetStatus   )(THIS_ OUT PCANSCHEDULERSTATUS2 pStatus)    PURE;
  STDMETHOD(AddMessage  )(THIS_ IN  PCANCYCLICTXMSG2 pMessage,
                                OUT PUINT32          pdwIndex)       PURE;
  STDMETHOD(RemMessage  )(THIS_ IN UINT32 dwIndex)                   PURE;
  STDMETHOD(StartMessage)(THIS_ IN UINT32 dwIndex,
                                IN UINT32 dwCount)                   PURE;
  STDMETHOD(StopMessage )(THIS_ IN UINT32 dwIndex)                   PURE;
};
#undef INTERFACE

typedef ICanScheduler*  PCANSCHEDULER;
typedef ICanScheduler2* PCANSCHEDULER2;

/*****************************************************************************
 * Interface ID of ICanScheduler  {C936498F-EC52-4C74-B3FE-A472BA7365BA}
 * Interface ID of ICanScheduler2 {70E3E8B8-852F-48E0-BDDB-B10A0C44CB3F}
 ****************************************************************************/
DEFINE_GUID(IID_ICanScheduler,
       0xC936498F,0xEC52,0x4C74,0xB3,0xFE,0xA4,0x72,0xBA,0x73,0x65,0xBA);

DEFINE_GUID(IID_ICanScheduler2,
       0x70E3E8B8,0x852F,0x48E0,0xBD,0xDB,0xB1,0x0A,0x0C,0x44,0xCB,0x3F);


/*****************************************************************************
 * Interface:
 *  ICanSocket
 *
 * Description:
 *  CAN socket interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE ICanSocket
DECLARE_INTERFACE_(ICanSocket, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanSocket methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetSocketInfo  )(THIS_ OUT PBALSOCKETINFO pInfo)         PURE;
  STDMETHOD(GetCapabilities)(THIS_ OUT PCANCAPABILITIES pCaps)       PURE;
  STDMETHOD(GetLineStatus  )(THIS_ OUT PCANLINESTATUS pStatus)       PURE;
  STDMETHOD(CreateChannel  )(THIS_ IN  BOOL         fExclusive,
                                   OUT PCANCHANNEL* ppChannel)       PURE;
};
#undef INTERFACE
#define INTERFACE ICanSocket2
DECLARE_INTERFACE_(ICanSocket2, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanSocket2 methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetSocketInfo  )(THIS_ OUT PBALSOCKETINFO pInfo)         PURE;
  STDMETHOD(GetCapabilities)(THIS_ OUT PCANCAPABILITIES2 pCaps)      PURE;
  STDMETHOD(GetLineStatus  )(THIS_ OUT PCANLINESTATUS2 pStatus)      PURE;
  STDMETHOD(CreateChannel  )(THIS_ IN  BOOL          fExclusive,
                                   OUT PCANCHANNEL2* ppChannel)      PURE;
};
#undef INTERFACE

typedef ICanSocket*  PCANSOCKET;
typedef ICanSocket2* PCANSOCKET2;

/*****************************************************************************
 * Interface ID of ICanSocket  {48965C25-9F8D-4E4D-8DE6-F029808C9ED2}
 * Interface ID of ICanSocket2 {D914F5A7-12FA-41F6-8AB4-4EA89F1C5F1E}
 ****************************************************************************/
DEFINE_GUID(IID_ICanSocket,
       0x48965C25,0x9F8D,0x4E4D,0x8D,0xE6,0xF0,0x29,0x80,0x8C,0x9E,0xD2);

DEFINE_GUID(IID_ICanSocket2,
       0xD914F5A7,0x12FA,0x41F6,0x8A,0xB4,0x4E,0xA8,0x9F,0x1C,0x5F,0x1E);


/*****************************************************************************
 * Interface:
 *  ICanStatus
 *
 * Description:
 *  CAN status interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE ICanStatus
DECLARE_INTERFACE_(ICanStatus, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanStatus methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetSocketInfo  )(THIS_ OUT PBALSOCKETINFO pInfo)         PURE;
  STDMETHOD(GetCapabilities)(THIS_ OUT PCANCAPABILITIES pCaps)       PURE;
  STDMETHOD(GetLineStatus  )(THIS_ OUT PCANLINESTATUS pStatus)       PURE;
  STDMETHOD(GetRuntimeInfo )(THIS_ OUT PCANRTINFO pInfo)             PURE;
};
#undef INTERFACE
#define INTERFACE ICanStatus2
DECLARE_INTERFACE_(ICanStatus2, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ICanStatus2 methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetSocketInfo  )(THIS_ OUT PBALSOCKETINFO pInfo)         PURE;
  STDMETHOD(GetCapabilities)(THIS_ OUT PCANCAPABILITIES2 pCaps)      PURE;
  STDMETHOD(GetLineStatus  )(THIS_ OUT PCANLINESTATUS2 pStatus)      PURE;
  STDMETHOD(GetRuntimeInfo )(THIS_ OUT PCANRTINFO pInfo)             PURE;
};
#undef INTERFACE

typedef ICanStatus  *PCANSTATUS;
typedef ICanStatus2 *PCANSTATUS2;

/*****************************************************************************
 * Interface ID of ICanStatus  {10795437-1028-4A32-910D-0E19236224DA}
 * Interface ID of ICanStatus2 {F686F1A4-4AC5-438B-A676-18CF6B9BA422}
 ****************************************************************************/
DEFINE_GUID(IID_ICanStatus,
       0x10795437,0x1028,0x4A32,0x91,0x0D,0x0E,0x19,0x23,0x62,0x24,0xDA);

DEFINE_GUID(IID_ICanStatus2,
       0xF686F1A4,0x4AC5,0x438B,0xA6,0x76,0x18,0xCF,0x6B,0x9B,0xA4,0x22);


/*****************************************************************************
 * Interface:
 *  ICanObject
 *
 * Description:
 *  CAN object interface for exclusive access to a CAN controller.
 *  The interface combines the functionality of the ICanSocket,
 *  ICanControl and ICanChannel interfaces into a single interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE ICanObject
DECLARE_INTERFACE_(ICanObject, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;

  //~~~ informational ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetCapabilities)(THIS_ OUT PCANCAPABILITIES pCaps)       PURE;
  STDMETHOD(GetLineStatus  )(THIS_ OUT PCANLINESTATUS pStatus)       PURE;

  //~~~ line control ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(DetectBaud)(THIS_ IN UINT16           wTimeoutMs,
                              IN OUT PCANBTRTABLE pBtrTable)         PURE;
  STDMETHOD(InitLine  )(THIS_ IN PCANINITLINE pInit,
                              IN UINT16       wRxFifoSize,
                              IN UINT16       wTxFifoSize)           PURE;
  STDMETHOD(ResetLine )(THIS)                                        PURE;
  STDMETHOD(StartLine )(THIS)                                        PURE;
  STDMETHOD(StopLine  )(THIS)                                        PURE;

  //~~~ ID filter ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(SetAccFilter)(THIS_ IN UINT8  bSelect,
                                IN UINT32 dwCode,
                                IN UINT32 dwMask )                   PURE;
  STDMETHOD(AddFilterIds)(THIS_ IN UINT8  bSelect,
                                IN UINT32 dwCode,
                                IN UINT32 dwMask)                    PURE;
  STDMETHOD(RemFilterIds)(THIS_ IN UINT8  bSelect,
                                IN UINT32 dwCode,
                                IN UINT32 dwMask)                    PURE;

  //~~~ communication channel  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetReceiveFifo )(THIS_ OUT PFIFOREADER* ppReader)        PURE;
  STDMETHOD(GetTransmitFifo)(THIS_ OUT PFIFOWRITER* ppWriter)        PURE;
};
#undef  INTERFACE
#define INTERFACE ICanObject2
DECLARE_INTERFACE_(ICanObject2, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;

  //~~~ informational ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetCapabilities)(THIS_ OUT PCANCAPABILITIES2 pCaps)      PURE;
  STDMETHOD(GetLineStatus  )(THIS_ OUT PCANLINESTATUS2 pStatus)      PURE;

  //~~~ line control ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(DetectBaud)(THIS_ IN     UINT8        bOpMode,
                              IN     UINT8        bExMode,
                              IN     UINT16       wTimeoutMs,
                              IN OUT PCANBTPTABLE pBtpTable)         PURE;
  STDMETHOD(InitLine  )(THIS_ IN PCANINITLINE2 pCanInitLine,
                              IN UINT32        dwRxFifoSize,
                              IN UINT32        dwTxFifoSize,
                              IN UINT32        dwFilterSize,
                              IN UINT8         bFilterMode)          PURE;
  STDMETHOD(ResetLine )(THIS)                                        PURE;
  STDMETHOD(StartLine )(THIS)                                        PURE;
  STDMETHOD(StopLine  )(THIS)                                        PURE;

  //~~~ ID filter ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetFilterMode)(THIS_ IN  UINT8  bSelect,
                                 OUT PUINT8 pbMode)                  PURE;
  STDMETHOD(SetFilterMode)(THIS_ IN  UINT8  bSelect,
                                 IN  UINT8  bMode,
                                 OUT PUINT8 pbPrev)                  PURE;
  STDMETHOD(SetAccFilter )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask )                  PURE;
  STDMETHOD(AddFilterIds )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask)                   PURE;
  STDMETHOD(RemFilterIds )(THIS_ IN UINT8  bSelect,
                                 IN UINT32 dwCode,
                                 IN UINT32 dwMask)                   PURE;

  //~~~ communication channel  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetReceiveFifo )(THIS_ OUT PFIFOREADER* ppReader)        PURE;
  STDMETHOD(GetTransmitFifo)(THIS_ OUT PFIFOWRITER* ppWriter)        PURE;
};

#undef INTERFACE

typedef ICanObject*  PCANOBJECT;
typedef ICanObject2* PCANOBJECT2;

/*****************************************************************************
 * Interface ID of ICanObject  {B346CF15-5CD5-4E08-9EF1-3AEA3F34AE0C}
 * Interface ID of ICanObject2 {264853AB-7D0C-4690-924D-97F06B5BD31C}
 ****************************************************************************/
DEFINE_GUID(IID_ICanObject,
       0xB346CF15,0x5CD5,0x4E08,0x9E,0xF1,0x3A,0xEA,0x3F,0x34,0xAE,0x0C);

DEFINE_GUID(IID_ICanObject2,
       0x264853AB,0x7D0C,0x4690,0x92,0x4D,0x97,0xF0,0x6B,0x5B,0xD3,0x1C);


/*****************************************************************************
 * Interface:
 *  ILinMonitor
 *
 * Description:
 *  LIN message monitor interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE ILinMonitor
DECLARE_INTERFACE_(ILinMonitor, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ILinMonitor methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(Initialize)(THIS_ IN UINT16 wRxFifoSize)                 PURE;
  STDMETHOD(Activate  )(THIS)                                        PURE;
  STDMETHOD(Deactivate)(THIS)                                        PURE;
  STDMETHOD(GetReader )(THIS_ OUT PFIFOREADER* ppReader)             PURE;
  STDMETHOD(GetStatus )(THIS_ OUT PLINMONITORSTATUS pStatus)         PURE;
};
#undef INTERFACE

typedef ILinMonitor* PLINMONITOR;

/*****************************************************************************
 * Interface ID of ILinMonitor {29068E57-6B69-4010-911C-7A9128521BE0}
 ****************************************************************************/
DEFINE_GUID(IID_ILinMonitor,
       0x29068E57,0x6B69,0x4010,0x91,0x1C,0x7A,0x91,0x28,0x52,0x1B,0xE0);


/*****************************************************************************
 * Interface:
 *  ILinControl
 *
 * Description:
 *  LIN control interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE ILinControl
DECLARE_INTERFACE_(ILinControl, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ILinControl methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(InitLine     )(THIS_ IN PLININITLINE pInit)              PURE;
  STDMETHOD(ResetLine    )(THIS)                                     PURE;
  STDMETHOD(StartLine    )(THIS)                                     PURE;
  STDMETHOD(StopLine     )(THIS)                                     PURE;
  STDMETHOD(WriteMessage )(THIS_ IN BOOL fSend, IN PLINMSG pLinMsg)  PURE;
  STDMETHOD(GetLineStatus)(THIS_ OUT PLINLINESTATUS pStatus)         PURE;
};
#undef INTERFACE

typedef ILinControl* PLINCONTROL;

/*****************************************************************************
 * Interface ID of ILinControl {4D58EF55-4F01-451E-911F-17C223A8B8F0}
 ****************************************************************************/
DEFINE_GUID(IID_ILinControl,
       0x4D58EF55,0x4F01,0x451E,0x91,0x1F,0x17,0xC2,0x23,0xA8,0xB8,0xF0);


/*****************************************************************************
 * Interface:
 *  ILinSocket
 *
 * Description:
 *  LIN socket interface.
 ****************************************************************************/
#undef  INTERFACE
#define INTERFACE ILinSocket
DECLARE_INTERFACE_(ILinSocket, IUnknown)
{
  //~~~ IUnknown methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(QueryInterface)(THIS_ IN REFIID riid, OUT PVOID *ppv)    PURE;
  STDMETHOD_(ULONG,AddRef )(THIS)                                    PURE;
  STDMETHOD_(ULONG,Release)(THIS)                                    PURE;
  //~~~ ILinSocket methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  STDMETHOD(GetSocketInfo  )(THIS_ OUT PBALSOCKETINFO pInfo)         PURE;
  STDMETHOD(GetCapabilities)(THIS_ OUT PLINCAPABILITIES pCaps)       PURE;
  STDMETHOD(GetLineStatus  )(THIS_ OUT PLINLINESTATUS pStatus)       PURE;
  STDMETHOD(CreateMonitor  )(THIS_ IN  BOOL         fExclusive,
                                   OUT PLINMONITOR* ppMonitor)       PURE;
};
#undef INTERFACE

typedef ILinSocket* PLINSOCKET;

/*****************************************************************************
 * Interface ID of ILinSocket {EF865638-6D07-4A46-A838-4BAFED359B1A}
 ****************************************************************************/
DEFINE_GUID(IID_ILinSocket,
       0xEF865638,0x6D07,0x4A46,0xA8,0x38,0x4B,0xAF,0xED,0x35,0x9B,0x1A);


/*##########################################################################*/
/*##                                                                      ##*/
/*##   exported API functions                                             ##*/
/*##                                                                      ##*/
/*##########################################################################*/

/*****************************************************************************
 * initialization
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  VciInitialize( void );

/*****************************************************************************
 * error handling
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  VciFormatErrorA( IN  HRESULT hrError,
                   OUT PCHAR   pszError,
                   IN  UINT32  dwLength);

EXTERN_C HRESULT VCIAPI
  VciFormatErrorW( IN  HRESULT hrError,
                   OUT PWCHAR  pszError,
                   IN  UINT32  dwLength);

#ifdef UNICODE
#define VciFormatError  VciFormatErrorW
#else
#define VciFormatError  VciFormatErrorA
#endif //UNICODE

/*****************************************************************************
 * version information
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  VciGetVersion( OUT PVCIVERSIONINFO pVersInfo );

/*****************************************************************************
 * utility functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  VciCreateLuid( OUT PVCIID pVciid );

EXTERN_C HRESULT VCIAPI
  VciLuidToCharA( IN  REFVCIID rVciid,
                  OUT PCHAR    pszLuid,
                  IN  UINT32   dwLength );

EXTERN_C HRESULT VCIAPI
  VciLuidToCharW( IN  REFVCIID rVciid,
                  OUT PWCHAR   pszLuid,
                  IN  UINT32   dwLength );

#ifdef UNICODE
#define VciLuidToChar  VciLuidToCharW
#else
#define VciLuidToChar  VciLuidToCharA
#endif //UNICODE

EXTERN_C HRESULT VCIAPI
  VciCharToLuidA( IN  PCHAR  pszLuid,
                  OUT PVCIID pVciid );

EXTERN_C HRESULT VCIAPI
  VciCharToLuidW( IN  PWCHAR pszLuid,
                  OUT PVCIID pVciid );

#ifdef UNICODE
#define VciCharToLuid  VciCharToLuidW
#else
#define VciCharToLuid  VciCharToLuidA
#endif //UNICODE

EXTERN_C HRESULT VCIAPI
  VciGuidToCharA( IN  REFGUID rGuid,
                  OUT PCHAR   pszGuid,
                  IN  UINT32  dwLength );

EXTERN_C HRESULT VCIAPI
  VciGuidToCharW( IN  REFGUID rGuid,
                  OUT PWCHAR  pszGuid,
                  IN  UINT32  dwLength );

#ifdef UNICODE
#define VciGuidToChar  VciGuidToCharW
#else
#define VciGuidToChar  VciGuidToCharA
#endif //UNICODE

EXTERN_C HRESULT VCIAPI
  VciCharToGuidA( IN  PCHAR pszGuid,
                  OUT PGUID pGuid );

EXTERN_C HRESULT VCIAPI
  VciCharToGuidW( IN  PWCHAR pszGuid,
                  OUT PGUID  pGuid );

#ifdef UNICODE
#define VciCharToGuid  VciCharToGuidW
#else
#define VciCharToGuid  VciCharToGuidA
#endif //UNICODE

/*****************************************************************************
 * time management functions
 ****************************************************************************/

EXTERN_C UINT64 VCIAPI
  VciQueryTickCounter( void );

EXTERN_C UINT32 VCIAPI
  VciQueryTickFrequency( void );

EXTERN_C UINT64 VCIAPI
  VciQueryCurrentTime( void );

/*****************************************************************************
 * resource manager specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  VciCreateFifo( OUT PVCIID pResid,
                 IN  UINT16 wCapacity,
                 IN  UINT16 wElemSize,
                 IN  REFIID riid,
                 OUT PVOID* ppv );

EXTERN_C HRESULT VCIAPI
  VciAccessFifo( IN  REFVCIID rResid,
                 IN  REFIID   riid,
                 OUT PVOID*   ppv );


/*****************************************************************************
 * device manager specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  VciGetDeviceManager( OUT IVciDeviceManager** ppDevMan );

EXTERN_C HRESULT VCIAPI
  VciQueryDeviceByHwid( IN  REFGUID      rHwid,
                        OUT IVciDevice** ppDevice );

EXTERN_C HRESULT VCIAPI
  VciQueryDeviceByClass( IN  REFGUID      rClass,
                         IN  UINT32       dwInst,
                         OUT IVciDevice** ppDevice );

#endif //_VCISDK_H_
