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

#include <CVCiView.h>
#include <CMutex.h>
#include <CSemaphore.h>
#include <CQueue.h>
#include <CUtil.h>
#include <CTimer.h>
#ifdef _WIN32
#include "CBonjourmDNS.h"
#else
#include "CAvahimDNS.h"
#endif

/*
 * Module detection macros
 */
#define API_MDNS_SERVICE_TYPE	"_tmvci._tcp"

#ifndef _WIN32
#define BASENAME(x) basename(x)
#else
#define BASENAME(x) (x)
#endif

enum HWType_e {
	IVIEW_SWIFT	= 100,
	VCI_SIMULATOR	= 101,
	MAN_T200	= 200,
	IVIEW_PRIMA	= 201,
	SPX_MIDRANGE	= 202,
	IVIEW_ULTRA	= 300,
	SPX_HIEND	= 301,
	PDUAPI_DEVEL	= 527	// Safety net for older releases.
};

#define API_DNS_SD_TXT_MOD_TYP_ID	"ModuleTypeID"
#define API_DNS_SD_TXT_HW_SN		"HwSerialNumber"
#define API_DNS_SD_TXT_HW_NAME		"HwName"
#define API_DNS_SD_TXT_HW_VER		"HwVersion"
#define API_DNS_SD_TXT_HW_DATE		"HwDate"
#define API_DNS_SD_TXT_HW_IF		"HwInterface"
#define API_DNS_SD_TXT_FW_NAME		"FwName"
#define API_DNS_SD_TXT_FW_VER		"FwVersion"
#define API_DNS_SD_TXT_FW_DATE		"FwDate"
#define API_DNS_SD_TXT_SW_VER		"SwVersion"
#define API_DNS_SD_TXT_VEND_INF		"AdditionalInfo"

/** CVCiViewHostIF
 * \brief
 *	Class to provide the callbacks for unsolicited data.
 *	inherit from this class and implement the virtual methods
 *	to receive unsolicited data records from the CiViewComms class
 *	Rx thread.
 */
class CVCiViewHostIF
{
public:
	virtual ~CVCiViewHostIF() {};
	virtual T_PDU_ERROR RxEvent(
			UNUM32				Id,
			vci_event_record_s*		VCIEvent ) = 0;
	virtual T_PDU_ERROR RxData(
			UNUM32				Id,
			vci_data_record_with_data*	VCIData ) = 0;
};

/**
 * \brief VCI Message Server
 * \details
 * 	This class configured a VCI channel and waits for incomming data
 * 	to which it will respond.
 */
class CVCiViewIF:
	public CVCiView {
public:
	CVCiViewIF( UNUM32, CVCiViewHostIF*, CComm* );
	virtual ~CVCiViewIF(){};
	/** Connect
	 * \brief This is not a valid way to connect from this class.
	 * \return T_PDU_ERROR
	 * \retval PDU_STATUS_NOERROR		Function call successful.
	 * \retval PDU_ERR_FCT_FAILED		Function call failed.
	virtual T_PDU_ERROR Connect()
	{
		return PDU_ERR_FCT_FAILED;
	}
	 */
	/** AddFilter
	 * \brief
	 * 	Add filtering based on specified criteria
	 * \param [in]  Type		Filter type
	 * \param [in]  NumDataBytes	Number of data bytes
	 * \param [in]  pData		Data bytes
	 * \param [in]  NumFlowBytes	Number of flow bytes
	 * \param [in]  pFlow		Flow bytes
	 * \param [out] FilterID	Filter ID
	 * \return \ref T_PDU_ERROR
	 * \retval PDU_STATUS_NOERROR	Function call successful.
	 * \retval PDU_ERR_FCT_FAILED	Function call failed.
	 */
	virtual T_PDU_ERROR AddFilter(
			UNUM32		Type,
			UNUM32		Flags,
			UNUM32		NumDataBytes,
			UNUM8*		pData,
			UNUM32		NumFlowBytes,
			UNUM8*		pFlow,
			UNUM32&		FilterID );
	/** DelFilter
	 * \brief
	 * 	Deletes a filter defined through AddFilter
	 * \param [in]  FilterID	Filter ID returned from AddFilter
	 * \return \ref T_PDU_ERROR
	 * \retval PDU_STATUS_NOERROR	Function call successful.
	 * \retval PDU_ERR_FCT_FAILED	Function call failed.
	 */
	virtual T_PDU_ERROR DelFilter(
			UNUM32		FilterID );
	/** Id
	 * \brief
	 * 	Set the I/F id. This is returned with the data to identify
	 *	the originating interface.
	 * \param [in] Id	The required interface identifier.
	 */
	virtual void Id(
			UNUM32		Id )
	{
		m_Id = Id;
	}
protected:
	T_PDU_ERROR RxData(vci_data_record_with_data*);
	T_PDU_ERROR RxEvent(vci_event_record_s*);
private:
	UNUM32		m_Id;
	CVCiViewHostIF*	m_Host;
};

typedef CmDNS* (__stdcall *CreatemDNS_t)(
		char*		Service,
		CmDNSIF*	DNSIF );

typedef CComm* (__stdcall *CreateCCommTCP_t)(
		UNUM32		Addr,
		UNUM16		Port );

typedef CVCiViewIF* (__stdcall *CreateCVCiViewIF_t)(
		UNUM32		Id,
		CVCiViewHostIF* HostClass,
		CComm*		Pipe );

extern "C" __declspec(dllexport) CmDNS* __stdcall CreatemDNS(
		std::string	Service,
		CmDNSIF*	DNSIF );

extern "C" __declspec(dllexport) CComm* __stdcall CreateCCommTCP(
		UNUM32		Addr,
		UNUM16		Port );

extern "C" __declspec(dllexport) CVCiViewIF* __stdcall CreateCVCiViewIF(
		UNUM32		Id,
		CVCiViewHostIF* HostClass,
		CComm*		Pipe );

#endif

