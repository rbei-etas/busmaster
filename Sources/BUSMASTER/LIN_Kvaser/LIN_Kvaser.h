/**
 * \file      LIN_Kvaser.h
 * \author    a1@uincore.net
 * \copyright Copyright (c) 2015, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

class CLIN_Kvaser: public CWinApp
{
public:
    CLIN_Kvaser(void);

    // Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

