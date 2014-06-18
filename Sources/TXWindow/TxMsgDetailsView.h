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
 * @brief Interface file for CTxMsgDetailsView class
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgDetailsView class
 */

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "TxWindow_resource.h"
#include "Utility/RadixEdit.h"          // For the RAdix Edit control definition
#include "FlexListCtrl.h"
#include "CMsgSignalTemp.h"
#include "Utility/SignalMatrix.h"
#include "Utility/MsgInterpretation.h"  // For message interpretation
class CTxMsgDetailsView : public CFormView
{
public:
    enum { IDD = IDD_DLG_TX_MSG_DETAILS };
    CComboBox   m_omComboChannelID;
    CSignalMatrix m_odSignalMatrix;
    CFlexListCtrl m_omLctrSigList;
    CComboBox m_omComboMsgIDorName;
    CButton m_omButtonUpdateMsg;
    CRadixEdit m_odDLC;
    CRadixEdit m_odDB8;
    CRadixEdit m_odDB7;
    CRadixEdit m_odDB6;
    CRadixEdit m_odDB5;
    CRadixEdit m_odDB4;
    CRadixEdit m_odDB3;
    CRadixEdit m_odDB2;
    CRadixEdit m_odDB1;
    BOOL m_bIsRTR;
    CString m_omStrMsgIDorName;
    int m_nRBTNFrameFormat;
    int m_nChannelID;

    CMsgSignal* m_pouDBPtr;

    void vSetMsgDBPtr(void* pMsgDB);

    /**
     * @brief To set given CAN values in to GUI
     * @param[in] psTxMsg pointer to message frame detail
     *
     * This function will format the data passed as parameter
     * and update the details into the controls below the
     * message frame list.
     */
    void vSetValues(STXCANMSGDETAILS* psTxMsg);

    /**
     * @brief To Disable signal values list control
     *
     * This function is used to disable signal details related
     * components. This will be called if the numeric mode has been
     * changed by the user.
     */
    void vDisableSignalComponents();

    /**
     * @brief To Enable/ Disable Add button after validating data
     *
     * Enables add button if the selected message block count is
     * lesser then the maximum
     *
     * @param[in] bEnable Enable / Disable
     */
    void vEnableAddButton(BOOL bEnable);

    /**
     * @brief To Update Signal list control with signals of the give CAN message
     *
     * This function will update the signal list with the CAN
     * message signals. It will do nothing if it is not a database
     * message. This will be called from the onitemchange of
     * message list.
     *
     * @param[in] sMsg CAN information
     */
    BOOL bUpdateSignalList(STCAN_MSG sMsg);

    /**
     * @brief To Set Status bar text
     *
     * This function is will update the status bar in this dialog.
     * This is used to give an indication to the user about invalid
     * inputs entered if any.
     *
     * @param[in] pStrText Pointer to the Display Text
     * @return Success or Failure
     */
    BOOL bSetStatusText(const char* pStrText);

    /**
     * @brief To populate Message Name combobox with database messages
     *
     * Fills database message combo box with database
     * messages
     */
    void vPopulateMessageComboBox();

    /**
     * @brief Adjust Message Combo Box Width
     *
     * This function adjusts the width of the Message Combo Box
     * according to its contents.
     */
    void vAdjustWidthMessageComboBox();

    /**
     * @brief To set properties of controls used
     *
     * This function changed the control properties for hex
     * and decimal settings.
     */
    void vSetControlProperties();

    /**
     * Updates the channel count value when a hardware
     * interface is updated.
     */
    void vUpdateChannelIDInfo();

    void vUpdateAllBlocksFrmDB();

    /** Used to update the changes to global list */
    afx_msg void vCallApplyChanges();

    /**
     * Called by Parameters when they are out of focus so as to update
     * values
     */
    afx_msg void vAutoUpdateModifyChanges();

    /**
     * @brief On Initial Update
     *
     * This function will be called by the framework during initial
     * show of this view. This function will register iteself in to
     * parent window class so that other views shall access it. And
     * this function will initialise signal list, message ID/Name
     * combobox and other UI components
     */
    virtual void OnInitialUpdate();

    /**
     * This function will be called by the framework before
     * despatching the message. This functioin shall be overriden to
     * process Enter key press to set the focus to next dialog
     * control as per the tab order
     *
     * @param[in] pMsg Pointer to Message
     */
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    /** protected constructor used by dynamic creation */
    CTxMsgDetailsView();

    virtual ~CTxMsgDetailsView();

    DECLARE_DYNCREATE(CTxMsgDetailsView)

    /**
     * @brief DDX/DDV support
     *
     * This function will map DDX object with UI control for data
     * exchange
     *
     * @param[in] pDX Pointer to DDX object
     */
    virtual void DoDataExchange(CDataExchange* pDX);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    /**
     * This function is called when user changes the edit
     * control value of combo box for message ID
     */
    afx_msg void OnEditchangeCombMsgIdName();

    /**
     * This function will be called when the Message Name/ID
     * combobox content got modified. This will update DLC, signal
     * list and Signal Matrix.
     */
    afx_msg void OnSelchangeCombMsgIdName();

    /**
     * This function will be called by frame work when user
     * changes DLC edit box value
     */
    afx_msg void OnUpdateEditDLC();

    /**
     * This function will validate data bytes and update Add and
     * Apply buttons
     */
    afx_msg void OnUpdateEditDataBytes();

    /**
     * This function will be called during Standard/Extended type
     * change. This will validate the message parameters and will
     * update the data if found ok.
     */
    afx_msg void OnRbtnMsgtypeStd();

    /**
     * This function will be called when the message name combobox
     * losses the focus. This will update the dependent data.
     */
    afx_msg void OnChkbMsgtypeRtr();

    /**
     * This function will update the data in the controls to
     * the message frame list of the message block currently
     * selected. It added as new item if selected item index
     * is invalid or update to the selected index.
     */
    afx_msg void OnButtonAddMsg();

    /**
     * This function will be called when there is a change in the
     * list control. The change could be because of selection
     * change by the user or due to the editing of the list item.
     * This will update signal matrix in case of selection change
     * and will update Global Tx data for data change after proper
     * validation.
     */
    afx_msg void OnItemchangedLstcSigDetails(NMHDR* pNMHDR, LRESULT* pResult);

    /**
     * This function will be called by the framework during combobox
     * item selection change. This handler will update CAN message
     * data with the selected channel ID
     */
    afx_msg void OnSelchangeCombChannelId();

    DECLARE_MESSAGE_MAP()

private:
    /** Image List for Signal List */
    CImageList m_omSigImageList;

    /** Pointer to store currently selected message detail */
    sMESSAGE* m_psSelectedMsgDetails;

    /** Temp data Array */
    UNION_BYTE m_unData[ defMAX_BYTE ];

    /** Bit Array */
    UINT m_bData [ defMAX_BYTE* defBITS_IN_BYTE ];

    /** Local variable to denote text update by prog not by user */
    BOOL m_bUpdating;

    /** Dirty Flag */
    BOOL m_bIsMsgDirty;

    /**
     * @brief To initialise signal list control
     *
     * This function will be called from OnInitDialog to init
     * signal list.
     */
    void vInitSignalListCtrl();

    /**
     * @brief To set up signal name double click handler procedure
     *
     * This function will be called when the user double clicks the
     * Signal Name from the list. This will display the signal
     * details.
     *
     * @param[in] pList Pointer to List Control
     * @param[in] nItem Item Index
     * @param[in] nSubItem Sub Item Index
     * @param[in] uParam User Parameter (Here it is this pointer)
     */
    static void vSignalNameHandler(CListCtrl* pList,
                                    int nItem, int nSubItem,
                                    void* uParam );

    /**
     * @brief To get signal details of selected signal in the signal list
     *
     * This function will return the signal pointer of the signal
     * pointed by the index. If it is not found it will return nullptr
     *
     * @param[in] nIndex Selected Signal index
     * @return Selected Signal Pointer
     */
    sSIGNALS* psGetSelectedSignalStruct(int nIndex);

    /**
     * @brief To update message details from the user entered values
     *
     * This function will update message details from the
     * control.
     *
     * @param[in] psMsgDetails pointer to current msg details
     * @return TRUE or FALSE
     */
    BOOL bUpdateMessageDetail(STCAN_MSG* psMsgDetails);

    /**
     * @brief To get Message ID from Message ID/ Name combobox
     *
     * This function will return a valid messsage Id or -1
     * from combo box edit box
     *
     * @return Message ID
     */
    INT nGetMessageID();

    /**
     * @brief TO update selected message details with user entered values
     *
     * This function will be called to update the data from the UI
     * in to the shared global data structure. This will be called
     * from various UI controls to get immediate update
     */
    BOOL vUpdateSelectedMessageDetails();

    /**
     * @brief To show signal values got from interpretation module
     * This function will update the signal list with the
     * interpretted value. This will reuse the exising rows by
     * setting the new text if bDeleteExistingEntries is FALSE
     *
     * @param[in] romSigInfo Reference to Interpretted Signal Information
     */
    void vShowSignalValues(const CSignalInfoArray& romSigInfo);

    /**
     * @brief To validate user entered message ID
     *
     * This function will be called to validate and get the message
     * ID.Incase of invalid input found in the UI this funtion will
     * return -1.
     *
     * @return Message ID or -1
     */
    int nGetValidMessageID();

    /**
     * @brief Get parent window pointer
     * This Function will return parent window pointer. That is
     * pointer to CTxMsgChildFrame. This will return nullptr incase of
     * failure
     *
     * @return Pointer to CTxMsgChildFrame
     */
    CWnd* pomGetParentWindow() const;

    /**
     * @brief To get Tx Blocks View Pointer
     *
     * This function will return CTxMsgBlocksView pointer. This will
     * get child window pointer to get view pointer.
     *
     * @return Pointer to CTxMsgBlocksView or nullptr incase of failure
     */
    CWnd* pomGetBlocksViewPointer() const;

    /**
     * @brief To get List View Pointer
     *
     * This function will return CTxMsgListView pointer. This will
     * get child window pointer to get view pointer.
     *
     * @return Pointer to CTxMsgListView or nullptr incase of failure
     */
    CWnd* pomGetListViewPointer() const;

    /**
     * @brief To get Functions View Poniter
     *
     * This function will return CTxFunctionsView pointer. This
     * will get child window pointer to get view pointer.
     *
     * @return Pointer to CTxFunctionsView or nullptr incase of failure
     */
    CWnd* pomGetFunctionsViewPointer() const;

    /**
     * @brief To validate user entered CAN message data
     *
     * This function will validate if the user has entered
     * valid data in Code and DLC edit controls
     *
     * @return TRUE or FALSE
     */
    BOOL bValidateData();

    /**
     * @brief To check data bytes boundary condition
     *
     * This function will validate the values in the edit controls
     * for data bytes in Decimal mode.
     *
     * @return TRUE/FALSE
     */
    BOOL bCheckIfValueIsMoreThan255(USHORT usNoOfEditCtrlsToCheck);

    /**
     * @brief To set the dirty bit of the selected message
     *
     * This function will update the dirty flag of the message. The
     * dirty flag will be set if the databytes are directly edited.
     * The flag will be cleared if the data got updated from signal
     * list with validation.
     *
     * @param[in] bDirty Dirty or Not
     */
    void SetDirty(BOOL bDirty = TRUE);

    /**
     * @brief To add a message in to the selected message block
     *
     * This function will be called from OnUpdateMessage. The
     * new message frame will be added in the currently
     * message block at the end as node in the list of frame.
     *
     * @return TRUE or FALSE
     */
    BOOL bAddMsgInBlock();

    /**
     * @brief To highlight selected signal postion in signal matrix
     *
     * This function will update the highlight of the signal matrix
     * with the selected signal information
     *
     * @param[in] nSelectedIndex Selected Signal index
     */
    void vUpdateSignalMatrix(int nSelectedIndex);

    /**
     * @brief To validate physical value change
     *
     * This function will validate and update the physical value
     * after edit. This will update Signal Matrix, data bytes. This
     * will finally update Global Tx block data.
     *
     * @param[in] nItem Item Index
     * @param[in] nSubItem SubItem Index
     */
    void vUpdateFromPhysicalValue(int nItem, int nSubItem);

    /**
     * @brief To validate raw value change
     *
     * This function will validate and update the raw value
     * after edit. This will update Signal Matrix, data bytes. This
     * will finally update Global Tx block data.
     *
     * @param[in] nItem Item Index
     * @param[in] nSubItem SubItem Index
     */
    void vUpdateFromRawValue(int nItem, int nSubItem);

    /**
     * @brief To update CAN message data bytes
     *
     * This function will be called to update the data bytes with
     * the signal values from the signal list. This will take care
     * of frame format Big/Little endian.
     */
    void vUpdateDataBytes();

    /**
     * @brief Update state of data bytes according to DLC
     *
     * This function sets/resets the Read Only flag of the data bytes
     * in accordance to the DLC setting.
     */
    void vUpdateStateDataBytes();

    /**
     * @brief To pack signal value in to the CAN message data bytes
     *
     * This function will update internal signal matrix and data
     * array. These two will be used to form data bytes from the
     * signal values.
     *
     * @param[in] psSignal Pointer to Signal Details
     * @param[in] nI64SignVal 64 bit Data
     */
    void vUpdateSignalData(sSIGNALS* psSignal,
                           __int64 nI64SignVal );

    /**
     * @brief To set default values for Message ID, DLC, databytes and Channel ID
     *
     * This function will set default values to UI controls.
     */
    void vSetDefaultValues();

    void vUpdateSignalMatrix(void);
};
