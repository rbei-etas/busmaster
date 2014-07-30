#pragma once
#include "afxcmn.h"
#include <map>
using namespace std;
#include "TxWindowEthernet_resource.h"

// CEthernetTxDefaultConfigDlg dialog

class CEthernetTxDefaultConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CEthernetTxDefaultConfigDlg)

public:
	CEthernetTxDefaultConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEthernetTxDefaultConfigDlg();

// Dialog Data
	enum { IDD = IDD_DLG_ETH_TX_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
