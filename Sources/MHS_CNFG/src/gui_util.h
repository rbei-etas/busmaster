/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      gui_util.h
 * \brief     Library to talk to Tiny-CAN devices
 * \author    Klaus Demlehner
 * \copyright Copyright (c) 2011, MHS-Elektronik GmbH & Co. KG
 *
 * Library to talk to Tiny-CAN devices.
 * You find the latest versions at http://www.tiny-can.com/
 */

#pragma once

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HEX_BYTE    0
#define HEX_CAN     1
#define HEX_WORD    2
#define HEX_DWORD   3
#define HEX_0xBYTE  4
#define HEX_0xCAN   5
#define HEX_0xWORD  6
#define HEX_0xDWORD 7

    char* GetWidgetTextDup(HWND ctrl_wnd);
    char* GetDlgItemTextDup(HWND hdlg, WORD id);
    void GetDlgItemTextCpy(char* str, HWND hdlg, WORD id, int max);
    uint32_t GetTextWidgetAsLong(HWND ctrl_wnd, int base);
    double GetTextWidgetAsDouble(HWND ctrl_wnd);
    void SetDlgItemHex(HWND hdlg, WORD id, uint32_t format_id, uint32_t value);
    uint32_t GetDlgItemHex(HWND hdlg, WORD id);
    void FillComboBox(HWND hdlg, WORD id, const char** str_tab,
                      const DWORD* data_tab, WORD tab_len, DWORD value);
    void SetComboBoxWnd(HWND ctrl_wnd, DWORD value);
    void SetComboBox(HWND hdlg, WORD id, DWORD value);
    DWORD GetComboBoxWnd(HWND ctrl_wnd);
    DWORD GetComboBox(HWND hdlg, WORD id);
    void SetCheckButton(HWND ctrl_wnd, WORD check);
    WORD GetCheckButton(HWND ctrl_wnd);

#ifdef __cplusplus
}
#endif
