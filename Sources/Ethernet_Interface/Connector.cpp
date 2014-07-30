

#pragma once
#include "Connector.h"
static u_char s_EthFrame[1500];

#define MAX_BUFF_ALLOWED 1500

CRITICAL_SECTION sg_CritSectReadSync;
static pcap_pkthdr *sg_Header = new pcap_pkthdr;
static u_char *sg_Pkt_data = new u_char[1540];
HANDLE sg_hDataReceivedEvent;   // Event on which the thread works on
bool   g_bStopReceptionLoop = false;
static pcap_t *sg_phandle = NULL;

CConnector::CConnector()
{
	m_sResult = NULL;
	m_ListenSocket = INVALID_SOCKET;
	m_ClientSocket = INVALID_SOCKET;
	m_unNetmask = 0xffffff;				//default value of netmask
	InitializeCriticalSection(&sg_CritSectReadSync);
	sg_hDataReceivedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}


CConnector::~CConnector()
{
	pcap_freealldevs(m_pAllDevs);
	DeleteCriticalSection(&sg_CritSectReadSync);
}


pcap_if_t* CConnector::SearchAdapters()
{
	/* Retrieve the device list */
	if(pcap_findalldevs(&m_pAllDevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		return NULL;
	}
	
	return m_pAllDevs;
}

HRESULT CConnector::ConnectToAdapter(pcap_if_t* pAdapter)
{
		/* Open the adapter */
	if ((sg_phandle = pcap_open_live(pAdapter->name,	// name of the device
							 65536,			// portion of the packet to capture. 
											// 65536 grants that the whole packet will be captured on all the MACs.
							 1,				// promiscuous mode (nonzero means promiscuous)
							 1000,			// read timeout
							 errbuf			// error buffer
							 )) == NULL)
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n");
		/* Free the device list */
		pcap_freealldevs(m_pAllDevs);
		return -1;
	}
	
	/* Check the link layer. We support only Ethernet for simplicity. */
	if(pcap_datalink(sg_phandle) != DLT_EN10MB)
	{
		fprintf(stderr,"\nThis program works only on Ethernet networks.\n");
		/* Free the device list */
		pcap_freealldevs(m_pAllDevs);
		return -1;
	}

	
	if(pAdapter->addresses != NULL)
	{
        /* Retrieve the mask of the first address of the interface */
        m_unNetmask = ((struct sockaddr_in *)(pAdapter->addresses->netmask))->sin_addr.S_un.S_addr;
	}
    else
	{
        /* If the interface is without addresses we suppose to be in a C class network */
        m_unNetmask = 0xffffff;
	}

	/* At this point, we don't need any more the device list. Free it */
	//pcap_freealldevs(m_pAllDevs);
}

HANDLE CConnector::GetEvent()
{
	//Does not raise event as expected when packet arrives, hence not used
	return pcap_getevent(sg_phandle);
}



/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{

    struct tm ltime;
    char timestr[16];
    time_t local_tv_sec;

    /*
     * unused variables
     */
	EnterCriticalSection(&sg_CritSectReadSync);

	if(g_bStopReceptionLoop == true)
	{
		//Set the event to get out of the thread "EthernetMsgReadThreadProc" in Ethernet_Interface
		SetEvent(sg_hDataReceivedEvent);
		pcap_breakloop(sg_phandle);
		return;
	}
	memcpy(sg_Header, header, sizeof(pcap_pkthdr));

	for(int i =0; i < header->len; i++)
	{
		sg_Pkt_data[i] = pkt_data[i];
	}

	LeaveCriticalSection(&sg_CritSectReadSync);

	SetEvent(sg_hDataReceivedEvent);
}

UINT ReceiveData(LPVOID pParam)
{
	//CConnector * pConnector = (CConnector* )pParam;
	pcap_loop(sg_phandle, 0, packet_handler, NULL);
	return 0;
}

HRESULT CConnector::SetReceiveCallBackFn()
{
	AfxBeginThread(ReceiveData, this);
	
	return S_OK;
}

HRESULT CConnector:: ReceivePackets(PPKTHDR** pPktHdr,u_char** pcPktData)
{
	/* Retrieve the packets */
    //return pcap_next_ex( m_phandle, pPktHdr, pcPktData);   
	/* start the capture */
    
	if(g_bStopReceptionLoop == true)
	{
		return S_FALSE;
	}
	EnterCriticalSection(&sg_CritSectReadSync);
	(*pPktHdr)->caplen = sg_Header->caplen;
	(*pPktHdr)->len = sg_Header->len;
	(*pPktHdr)->ts = sg_Header->ts;

	for(int i =0; i < sg_Header->len; i++)
	{
		(*pcPktData)[i] = sg_Pkt_data[i];
	}
	LeaveCriticalSection(&sg_CritSectReadSync);
	return S_OK;
}

unsigned char hex_digit_value(char c)
{
    if ('0' <= c && c <= '9') { return c - '0'; }
    if ('a' <= c && c <= 'f') { return c + 10 - 'a'; }
    if ('A' <= c && c <= 'F') { return c + 10 - 'A'; }
	c= 0xff;
    return -1;
}


HRESULT CConnector::SendMsg(const STETHERNET_MSG& sEthMsg)
{
	for (int i = 0, j= 0; i < 6; i++, j+=2)
    {
        s_EthFrame[i] = (hex_digit_value(sEthMsg.m_ucDestMac[j]) << 4) | ( hex_digit_value(sEthMsg.m_ucDestMac[j+1]));            
    }

	for (int i = 6, j= 0; i < 12; i++, j+=2)
    {
        s_EthFrame[i] = (hex_digit_value(sEthMsg.m_ucSourceMAC[j]) << 4) | ( hex_digit_value(sEthMsg.m_ucSourceMAC[j+1]));            
    }

	for (int i = 0; i < sEthMsg.m_unDataLen ; i++)
    {
		s_EthFrame[i + 12] = sEthMsg.m_ucData[i];            
    }

	if (pcap_sendpacket(sg_phandle, s_EthFrame, sEthMsg.m_unDataLen + 12 /* MAC addresses */) != 0)
    {
        fprintf(stderr,"\nError sending the packet: %s\n", pcap_geterr(sg_phandle));
		return S_FALSE;
    }
	return S_OK;
}

HRESULT CConnector::SetFilter(string strFilter)
{
	struct bpf_program fcode;
    //compile the filter
	if (pcap_compile(sg_phandle, &fcode, strFilter.c_str(), 1, m_unNetmask) <0 )
    {
		AfxMessageBox("Unable to compile the packet filter. Check the syntax.\n");
        /* Free the device list */
        //pcap_freealldevs(alldevs);
        return -1;
    }
    
    //set the filter
    if (pcap_setfilter(sg_phandle, &fcode)<0)
    {
        AfxMessageBox("Error setting the filter.\n");
        /* Free the device list */
        //pcap_freealldevs(alldevs);
        return -1;
    }
}

void CConnector::StartStopReceptionLoop(bool bStopReceptionLoop)
{
	g_bStopReceptionLoop = bStopReceptionLoop;
}