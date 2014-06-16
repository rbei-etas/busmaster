// UDS_TimmingWnd.cpp: archivo de implementación
//

#include "stdafx.h"
#include "UDS_TimmingWnd.h"
#include "UDS_Resource.h"
//#include "afxdialogex.h"


// Cuadro de diálogo de CUDS_TimmingWnd

IMPLEMENT_DYNAMIC(CUDS_TimmingWnd, CDialog)

CUDS_TimmingWnd::CUDS_TimmingWnd(CWnd* pParent /*=NULL*/)
    : CDialog(CUDS_TimmingWnd::IDD, pParent)
{

}

CUDS_TimmingWnd::~CUDS_TimmingWnd()
{
}

void CUDS_TimmingWnd::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUDS_TimmingWnd, CDialog)
END_MESSAGE_MAP()


// Controladores de mensajes de CUDS_TimmingWnd
