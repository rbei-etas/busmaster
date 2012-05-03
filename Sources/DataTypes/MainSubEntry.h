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
 * \file      MainSubEntry.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

struct SGUIPARAMS
{
    char       m_acTitleName[MAX_PATH];
    char       m_acMainListName[MAX_PATH];
    char       m_acUnSelListName[MAX_PATH];
    char       m_acSelListName[MAX_PATH];

    CImageList* m_pomImageList;     //Pointer to loaded imagelist
    INT         m_unUnSelIconIndex; // Icon will appear in left list cntrl
    INT         m_unSelIconIndex;   // Icon will appear in rightlist cntrl
    BOOL        m_bCombine;  // Whether Main Entry should be combined with subentry name
    // while displaying in the list
    SGUIPARAMS();
};

typedef struct tagSubEntry
{
    unsigned int m_unSubEntryID;    // ID
    CString m_omSubEntryName;       // Name

    tagSubEntry();
    ~tagSubEntry();
    BOOL operator ==(const tagSubEntry& RefObj) const;

} SSUBENTRY;

typedef CList<SSUBENTRY, SSUBENTRY&> CSubEntryList;

// Structure for Signal Watch Implementation
typedef struct tagMainEntry
{
    UINT m_unMainEntryID;             // ID
    CString m_omMainEntryName;        // Name
    CSubEntryList m_odUnSelEntryList; // Sub entries which are not selected
    CSubEntryList m_odSelEntryList;   // Sub entries which are selected

    tagMainEntry();
    ~tagMainEntry();
    void vResetAll(void);
    tagMainEntry& operator=(tagMainEntry& RefObj);
    BOOL operator==(const tagMainEntry& RefObj) const;

} SMAINENTRY;

typedef CList<SMAINENTRY, SMAINENTRY&> CMainEntryList;
