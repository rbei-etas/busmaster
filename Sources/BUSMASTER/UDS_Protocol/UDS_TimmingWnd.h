#pragma once
#include "UDS_Resource.h"


// Cuadro de diálogo de CUDS_TimmingWnd

class CUDS_TimmingWnd : public CDialog
{
    DECLARE_DYNAMIC(CUDS_TimmingWnd)

public:
    CUDS_TimmingWnd(CWnd* pParent = NULL);   // Constructor estándar
    virtual ~CUDS_TimmingWnd();

    // Datos del cuadro de diálogo
    enum { IDD = IDD_TIMMING_UDS };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

    DECLARE_MESSAGE_MAP()
};
