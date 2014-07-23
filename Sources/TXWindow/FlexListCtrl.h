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
 * @brief This file contain the definition of CFlexListCtrl class
 * @author Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CFlexListCtrl class
 */

#pragma once

#include "Utility/Utility_Structs.h"                 // For data struct definition
#include "Utility/ComboItem.h"
#include "Utility/NumEdit.h"

class CFlexListCtrl : public CFFListCtrl
{
public:
    CFlexListCtrl();
    virtual ~CFlexListCtrl();

    /**
     * @brief
     *   This is used to map the Row X Column 2d data into
     *   one dimentional map key. This value should be greater then the maximum
     *   Expected list column count
     *
     * This function will set the virtual column count that is used
     * to derive the lineat key from row-column index. This will
     * clear all cmap's content as the unique key generator got
     * modified.
     *
     * @param[in] nColumn No. of expected Columns
     */
    void vSetMapColumnCount(int nColumn);

    /**
     * @brief
     *   This will show appropriate controls to edit the column data. This will be
     *   called when validation fails and to prompt to reenter the data
     *
     * This function will be called to show the control to edit the
     * item. This will be useful to show the control during invalid
     * input entered by the user
     *
     * @param[in] nRow Row Index
     * @param[in] nColumn Column Index
     */
    void vGoToEditMode(int nItem, int nSubItem);

    /**
     * @brief
     *   This is used to get the modification information stored during last
     *   notification message. This will be useful to get Row & Column information
     *   of the modified element
     *
     * This function will return the last edit item details. This is
     * required to get the Row & Col information from the
     * OnItemChanged handler. To avoid validation during insert it
     * will return false telling that data is invalid.
     *
     * @param[in] rs_DispInfo Reference to Display Information Structure
     * @param[in] sInfo User Program info structure
     * @return Success or Failure
     */
    BOOL sGetModificationStructure(LV_DISPINFO&);

    /**
     * @brief
     *   This will show the editable controls over the list item. This will use
     *   backend map to get the type and parameter information
     *
     * This function will show the UI control to change the list item
     * text. This will get the type information from the Cmap and
     * will call approp. function to create and show that control.
     *
     * @param[in] nRow Index of the Row
     * @param[in] nColumn Index of the Column
     */
    void vShowControl(int nItem, int nSubItem);

    /**
     * @brief This will set the column type and edit control type
     *
     * This function will be called to set the list item type. This
     * will call lGetMapID to derive a unique number from row-column
     * index. This will update the CMap with the type information.
     *
     * @param[in] nRow Row Index
     * @param[in] nColumn Column Index
     * @param[in] sInfo List info structure
     */
    void vSetColumnInfo(int nRow, int nColunm, SLISTINFO eType);

    /**
     * @brief This will set numeric control parameter of a column
     *
     * This function will be called to set the list item numeric
     * information. This will set the value in the numeric CMap
     *
     * @param[in] nRow Row Index
     * @param[in] nColumn Column Index
     * @param[in] sInfo Numeric info structure
     */
    void vSetNumericInfo(int nRow, int nColunm, const SNUMERICINFO& sInfo);

    /**
     * @brief This will return the column type and edit control type
     *
     * This function will be called to get the list item type. This
     * will call lGetMapID to derive a unique number from row-column
     * index. This will return the value stored in the CMap
     *
     * @param[in] nRow Index of the intrested Row
     * @param[in] nColumn Index of the intrested Column
     * @return Column Type info
     */
    SLISTINFO sGetColumnInfo(int nRow, int nColunm);

    /**
     * @brief This function will create an editable combobox.
     *
     * This function will create and show editable combo box.This
     * control will be placed over the list item. The combobox will
     * be filled with the data given the omList.
     *
     * @param[in] nRow Index of the Row
     * @param[in] nColumn Index of the Column
     * @param[in] omList List of strings
     * @return Pointer to the control
     */
    CComboItem* pomComboList( int nItem, int nSubItem,
                              const CStringArray& omList);

    /**
     * @brief This function will create a non-editable combobox.
     *
     * This function will create and show non editable combo box.This
     * control will be placed over the list item. The combobox will
     * be filled with the data given the omList.
     *
     * @param[in] nRow Index of the Row
     * @param[in] nColumn Index of the Column
     * @param[in] omList List of strings
     * @return Pointer to the control
     */
    CComboItem* pomComboItem(int nItem, int nSubItem,
                             const CStringArray& omList);

    /**
     * @brief This function will create an editbox.
     *
     * This function will create and show an editbox.
     *
     * @param[in] nRow Index of the Row
     * @param[in] nColumn Index of the Column
     * @return Pointer to the control
     */
    CEdit* pomEditItem(int nItem, int nSubItem);

    /**
     * @brief This function will create a numeric editbox.
     *
     * This function will create and show a numeric edit control with
     * or without spin control
     *
     * @param[in] nRow Index of the Row
     * @param[in] nColumn Index of the Column
     * @param[in] sInfo Numeric info like Min val, Max val, Base...
     * @return Pointer to the control
     */
    CNumEdit* pomNumItem(int nItem, int nSubItem, const SNUMERICINFO& sInfo);

    /**
     * @brief This will set the user program informatrion of a column
     *
     * This function will be called to set the list item user program
     * information. This will set the value in the user prog CMap
     *
     * @param[in] nRow Row Index
     * @param[in] nColumn Column Index
     * @param[in] sInfo User Program info structure
     */
    void vSetUserProgInfo(int nRow, int nColunm,
                          const SUSERPROGINFO& sUSerProgInfo);

protected:
    /**
     * Notification Handler. This handler will be called after once
     * the item editing is complete. This will update the list item
     * text and will copy the DISPINFO data. This will set col 0 val
     * to invoke item changed event.
     */
    afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

    /**
     * Event Handler. This handler will hide the controls if
     * anytihng is displayed for editing. This is done by setting the
     * focus to the list control.
     */
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    /**
     * Event Handler. This handler will hide the controls if
     * anytihng is displayed for editing. This is done by setting the
     * focus to the list control.
     */
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    /**
     * Event Handler. This handler will show the controls to edit the
     * selected data from the list control. This will call
     * ShowControl function with the item and subitem indices.
     */
    afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);

    /**
     * Event Handler. This handler will show the controls to edit the
     * selected data from the list control only if signle click
     * activate property is set. This will make list control editable
     * with a single click.
     */
    afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);

private:
    /** Signle Click edit or double click edit */
    BOOL m_bSingleClickActivate;

    /** CMap for list item types */
    CListTypes m_omListItemType;

    /**  CMap for numeric parameters */
    CNumericInfo m_omNumDetails;

    /** CMap for User Program information */
    CUserProgList m_omUserProg;

    /** Structure to store last modified information */
    LV_DISPINFO m_sModifiedInfo;

    /**
     * Flag to indicate that controls are being created. This is to avoid
     * validation while loading the signal list
     */
    BOOL m_bCreating;

    /** This is to store virtual column count */
    int m_nVirtualColumnCount;

    /**
     * This function will return the unique map index. This will use
     * virtual column count to make the 2d number to linear.
     *
     * @param[in] nRow Index of the Row
     * @param[in] nColumn Index of the Column
     * @return Unique map index derived from Row & Col val
     */
    long lGetMapID(int nRow, int nCol);

    DECLARE_MESSAGE_MAP()
};
