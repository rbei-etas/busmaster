/**
 * VCiViewIF.h
 *
 * iView VCI I/F Library class declarations for VCI Comms Applications
 *
 * These library classes acts as a API to allow applications to drive the
 * iView VCI protocol stack using a high level interface. It uses the
 * VCI interface classes created for the D-PDU-API library.
 *
 * To use these classes applications should derive a host class from
 * from the interface class to receive incomming data frames. Instantiate a
 * protcol class object for each protocol channel required with the host
 * class as the argument.  Instantiate a system class object with
 * the host class as the argument for non protocol operations.
 *
 */
#ifndef _CVCICLIENT_H
#define _CVCICLIENT_H

#include <string>
#include <map>
#include "EXTERNAL\vci.h"

/*
 * Module detection macros
 */
#define API_MDNS_SERVICE_TYPE   "_tmvci._tcp"

#ifndef _WIN32
#define BASENAME(x) basename(x)
#else
#define BASENAME(x) (x)
#endif

#ifndef _MSC_VER
#include <stdint.h>
#else
/* Use MSVC type names */
#if defined(_WIN64)
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
#else
typedef signed char int8_t;
typedef unsigned __int8 uint8_t;
#endif

typedef unsigned __int16 uint16_t;
typedef __int16 int16_t;

typedef unsigned __int32 uint32_t;
typedef __int32 int32_t;
/* define STDIO_FILENO */
#define STDIN_FILENO    _fileno( stdin )
#define STDOUT_FILENO   _fileno( stdout )
#endif

typedef uint8_t     CHAR8;
typedef uint8_t     UNUM8;
typedef int8_t      SNUM8;
typedef uint16_t    UNUM16;
typedef int16_t     SNUM16;
typedef uint32_t    UNUM32;
typedef int32_t     SNUM32;

typedef enum E_IVIEW_STATUS
{
    IVIEW_NOERROR       = 0x00000000,
    IVIEW_ERR_FCT_FAILED    = 0x00000001
} T_IVIEW_STATUS;

enum HWType_e
{
    IVIEW_SWIFT = 100,
    VCI_SIMULATOR   = 101,
    MAN_T200    = 200,
    IVIEW_PRIMA = 201,
    SPX_MIDRANGE    = 202,
    IVIEW_ULTRA = 300,
    SPX_HIEND   = 301,
    PDUAPI_DEVEL    = 527   // Safety net for older releases.
};

#define API_DNS_SD_TXT_MOD_TYP_ID   "ModuleTypeID"
#define API_DNS_SD_TXT_HW_SN        "HwSerialNumber"
#define API_DNS_SD_TXT_HW_NAME      "HwName"
#define API_DNS_SD_TXT_HW_VER       "HwVersion"
#define API_DNS_SD_TXT_HW_DATE      "HwDate"
#define API_DNS_SD_TXT_HW_IF        "HwInterface"
#define API_DNS_SD_TXT_FW_NAME      "FwName"
#define API_DNS_SD_TXT_FW_VER       "FwVersion"
#define API_DNS_SD_TXT_FW_DATE      "FwDate"
#define API_DNS_SD_TXT_SW_VER       "SwVersion"
#define API_DNS_SD_TXT_VEND_INF     "AdditionalInfo"

typedef enum iVIEW_Event_e
{
    iVIEW_SERVICE_ALL_FOR_NOW,
    iVIEW_SERVICE_ADD,
    iVIEW_SERVICE_REMOVE
} iVIEW_Event_t;

/** CVCiViewBrowser I/F class
 *  Interface for the CVCiViewBrowser class to call back with
 *  the information when an iView is detected.
 */
class CVCiViewBrowserIF
{
public:
    /* iViewResolver. Called when a service is discovered or removed.
     * \param Service   mDNS (DNS-SD) Service Name
     * \param Type      Service type for the information.
     * \param Hostname  Name of the host hosting the service.
     * \param Address   IPV4 address of the host hosting the service.
     * \param Port      TCP Port number on the host to use for the
     *          service.
     * \param TxtList   DNS Service Discovery Name<->Value pairs
     *          of TXT records provided by the mDNS service.
     */
    virtual void iViewResolver(
        iVIEW_Event_t   State,
        std::string&    Service,
        std::string&    Type,
        std::string&    Hostname,
        std::string&    Address,
        UNUM16      Port,
        std::map< std::string, std::string >& TxtList ) = 0 ;
};

/**
 * \class CVCiViewBrowser
 * \brief iView Browser class definition
 * \details
 *  This class abstracts the iView  platform specific detection.
 *  It takes the callback interface  to be  registered to dynamically
 *  detect iView modules.
 *
 */
class CVCiViewBrowser
{
protected:
    /**
     * CVCiViewBrowser
     * \brief Contructor for the class.
     * \param ServiceType - String containing the mDNS service type to
     *  search, resolve information and provide updates for via the
     *  callback function.
     * \param Callback - User defined callback function.
     * \param UserData - User supplied parameter that is passed back with
     *  callback function. This is generally used to provide context
     *  to the callback function i.e. a class instance pointer.
     *
     */
    CVCiViewBrowser( std::string&, CVCiViewBrowserIF* ) {};
public:
    virtual ~CVCiViewBrowser() {};
    /**
     * Start
     * \brief Starts the mDNS thread to start resolving mDNS enabled nodes
     * \param N/A
     * \return Success/Failure on starting the mDNS thread.
     */
    virtual bool Start( void ) = 0;
    /**
     * Stop
     * \brief Stop the mDNS thread resolving mDNS enabled nodes
     * \param N/A
     * \return Success/Failure on stop the mDNS thread.
     */
    virtual bool Stop( void ) = 0;
};

class CComm;

/** CVCiViewHostIF
 * \brief
 *  Class to provide the callbacks for unsolicited data.
 *  inherit from this class and implement the virtual methods
 *  to receive unsolicited data records from the CVCiViewIF class
 *  Rx thread.
 */
class CVCiViewHostIF
{
public:
    virtual ~CVCiViewHostIF() {};
    virtual void RxEvent(
        UNUM32              Id,
        vci_event_record_s*     VCIEvent ) = 0;
    virtual void RxData(
        UNUM32              Id,
        vci_data_record_with_data*  VCIData ) = 0;
};

/**
 * \brief VCI Communication Interface
 * \details
 *  This class opens and configures a suuplied VCI CComm channel.
 *  This channel can be configured, data can be transmitted and will
 *  asynchronously supply incomming data.
 */
class CVCiViewIF
{
public:
    CVCiViewIF( UNUM32, CVCiViewHostIF*, CComm* ) {};
    virtual ~CVCiViewIF() {};
    /** Connect
     * \brief Create and send VCI Connect record.
     * \param ProtocolId        iView VCI protocol Id
     * \param PhysicalId        iView VCI physical Id
     * \param PrimaryPin        iView pin Id
     * \param SecondaryPin      iView pin Id
     * \param Termination       iView termination id
     * \param Baudrate      Baud rate
     * \return T_PDU_ERROR
     * \retval PDU_STATUS_NOERROR       Function call successful.
     * \retval PDU_ERR_FCT_FAILED       Function call failed.
     */
    virtual T_IVIEW_STATUS Connect(
        UNUM32      ProtocolId,
        UNUM32      PhysicalId,
        UNUM32      PrimaryPin,
        UNUM32      SecondaryPin,
        UNUM32      Terminiation,
        UNUM32      Baudrate,
        UNUM32      Flags ) = 0;
    /** Connected
     * \brief Report on the connected status of the VCI
     * \return bool
     * \retval true         VCI Connected
     * \retval false        VCI Not Connected
     */
    virtual bool Connected() = 0;
    /** Disconnect
     * \brief Disconnect the protocol from the physical bus.
     * \return \ref T_PDU_ERROR
     * \retval PDU_STATUS_NOERROR   Function call successful.
     * \retval PDU_ERR_FCT_FAILED   Function call failed.
     */
    virtual T_IVIEW_STATUS Disconnect() = 0;
    /** SendData
     * \brief Encapsulate raw data into a iView Data Record and send
     *  it using a CComm implementation.
     * \return \ref T_PDU_ERROR
     * \retval PDU_STATUS_NOERROR       Function call successful.
     * \retval PDU_ERR_FCT_FAILED   Function call failed.
     */
    virtual T_IVIEW_STATUS SendDataRec(
        const UNUM8*        Data,
        UNUM32          Length,
        UNUM32          Flags = 0,
        UNUM32          User = 0,
        UNUM32          Timeout = 0 ) = 0;
    /** IOCtl
     * \brief Send an IOCtl paramter via configuration request/response
     *  packet
     * \param URID      Unique response identifier.
     * \param ID        VCI IOCtl ID
     * \param Data      Simple UNUM32 Data value or UNUM8 byte stream
     * \return T_PDU_ERROR
     * \retval PDU_STATUS_NOERROR       Function call successful.
     * \retval PDU_ERR_FCT_FAILED       Function call failed.
     */
    virtual T_IVIEW_STATUS IOCtl(
        const UNUM32    ID ) = 0;
    virtual T_IVIEW_STATUS IOCtl(
        const UNUM32    ID,
        const UNUM32&   Data ) = 0;
    virtual T_IVIEW_STATUS IOCtl(
        const UNUM32    ID,
        UNUM32&     Data ) = 0;
    virtual T_IVIEW_STATUS IOCtl(
        const UNUM32    ID,
        UNUM8*      Data ) = 0;
    /** AddFilter
     * \brief
     *  Add filtering based on specified criteria
     * \param [in]  Type        Filter type
     * \param [in]  FilterLen   Number of filter bytes
     * \param [in]  pData       Data bytes
     * \param [in]  pMask       Mask bytes
     * \param [in]  Flow        Number of flow filter bytes
     * \param [in]  pFlow       Flow bytes
     * \param [out] FilterID    Filter ID
     * \return \ref T_PDU_ERROR
     * \retval PDU_STATUS_NOERROR   Function call successful.
     * \retval PDU_ERR_FCT_FAILED   Function call failed.
     */
    virtual T_IVIEW_STATUS AddFilter(
        UNUM32      Type,
        UNUM32      Flags,
        UNUM32      FilterLen,
        UNUM8*      pData,
        UNUM8*      pMask,
        UNUM32      FlowLen,
        UNUM8*      pFlow,
        UNUM32&     FilterID ) = 0;
    /** DelFilter
     * \brief
     *  Deletes a filter defined through AddFilter
     * \param [in]  FilterID    Filter ID returned from AddFilter
     * \return \ref T_PDU_ERROR
     * \retval PDU_STATUS_NOERROR   Function call successful.
     * \retval PDU_ERR_FCT_FAILED   Function call failed.
     */
    virtual T_IVIEW_STATUS DelFilter(
        UNUM32      FilterID ) = 0;
    /** Id
     * \brief
     *  Set the I/F id. This is returned with the data to identify
     *  the originating interface.
     * \param [in] Id   The required interface identifier.
     */
    virtual void Id( UNUM32 Id ) = 0;
};

typedef CVCiViewBrowser* (__stdcall* CreateBrowser_t)(
    char*           Service,
    CVCiViewBrowserIF*  HostIF );

typedef CComm* (__stdcall* CreateCCommTCP_t)(
    UNUM32      Addr,
    UNUM16      Port );

typedef CVCiViewIF* (__stdcall* CreateCVCiViewIF_t)(
    UNUM32      Id,
    CVCiViewHostIF* HostClass,
    CComm*      Pipe );

extern "C" __declspec(dllexport) CVCiViewBrowser* __stdcall CreateBrowser(
    char*           Service,
    CVCiViewBrowserIF*  HostIF );

extern "C" __declspec(dllexport) CComm* __stdcall CreateCCommTCP(
    UNUM32      Addr,
    UNUM16      Port );

extern "C" __declspec(dllexport) CVCiViewIF* __stdcall CreateCVCiViewIF(
    UNUM32      Id,
    CVCiViewHostIF* HostClass,
    CComm*      Pipe );

#endif

