#pragma once

#include "UDS_Resource.h"


// Cuadro de diálogo de CUDS_SettingsWnd

class CUDS_SettingsWnd : public CDialog
{
    DECLARE_DYNAMIC(CUDS_SettingsWnd)

public:
    CUDS_SettingsWnd(CWnd* pParent = NULL);   // Constructor estándar
    virtual ~CUDS_SettingsWnd();

    // Datos del cuadro de diálogo
    enum { IDD = IDM_UDS };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

    DECLARE_MESSAGE_MAP()
};
