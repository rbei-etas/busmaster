#include "UdsMsgBlocksView.h"
#include "UDSWnd_Defines.h"

CUdsMsgBlocksView::CUdsMsgBlocksView(void)
{
}

CUdsMsgBlocksView::~CUdsMsgBlocksView(void)
{
}



/******************************************************************************/
/*  Function Name    :  psGetMsgBlockPointer                                  */
/*  Input(s)         :  nIndexCurBlock : Index of message block in the list   */
/*                      psCurrentMsgBlockList : header of msg block pointer   */
/*  Output           :                                                        */
/*  Functionality    :  This function will return message block pointer for   */
/*                      node at index nIndexCurBlock passed as parameter from */
/*                      the link list of message block.                       */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
//SMSGBLOCKLIST* CUdsMsgBlocksView::psGetMsgBlockPointer(INT nIndexCurBlock,
//        SMSGBLOCKLIST* psMsgBlockList)
//{
//    if(psMsgBlockList != NULL && nIndexCurBlock != -1)
//    {
//        INT nIndex = 0;
//        // Get the message block pointer in the list at the index
//        // nIndexCurBlock
//        while(nIndex != nIndexCurBlock)
//        {
//            if( psMsgBlockList != NULL )
//            {
//                psMsgBlockList = psMsgBlockList->m_psNextMsgBlocksList ;
//            }
//            nIndex++;
//        }
//    }
//    return psMsgBlockList;
//}
//