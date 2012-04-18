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
 * \file      Comment.cpp
 * \brief     Implementation file for the Comment class.
 * \authors   Padmaja A, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the Comment class.
 */

/* Project includes */
#include "Comment.h"
#include "Definitions.h"

/**
 * \brief Constructor
 *
 * Constructor of CComment
 */
CComment::CComment()
{
    m_msgType = '\0';
    m_elementName = "";
    m_comment = "";
	m_msgID = 0;
}

/**
 * \brief     operator= overloading
 * \param[in] tCmt Comment to copy content from
 * \return    Local comment with new content
 *
 * Copies the content from tCmt into the local object.
 */
CComment& CComment::operator=(CComment& tCmt)
{
    //copies all members of tCmt object to this object.
    m_elementName=tCmt.m_elementName;
    m_comment=tCmt.m_comment;
    m_msgID=tCmt.m_msgID;
    m_msgType=tCmt.m_msgType;
    return(*this);
}

/**
 * \brief Destrutor
 *
 * Destructor of CComment
 */
CComment::~CComment()
{
}

/**
 * \brief     Format the net comment
 * \param[in] fileInput Input file
 * \param[in] m_listComment list of CComments
 *
 * Parses the net comments.
 */
void CComment::Format_netComment(fstream &fileInput, list<CComment>& m_listComment)
{
    char *pcToken, *pcLine;
    char acLine[defCON_MAX_LINE_LEN];
    string comment;
    //Reads all the net comments,parses the comments and stores them to a list.
    while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_DESC_NET]\n")!=0)
    {
        pcLine=acLine;
        m_elementName="";
        pcToken=pcLine;
        comment = pcToken;
        //parses net comment.
        while(strstr(pcToken,"\";") == NULL)
        {
            fileInput.getline(acLine,defCON_MAX_LINE_LEN);
            pcToken = acLine;
            comment += pcToken;
        }
        m_comment= comment;
        //adds the comment to the list.
        m_listComment.push_back(*this);
    }
}

/**
 * \brief     Format the node comments
 * \param[in] fileInput Input file
 * \param[in] m_listComment List of CComments
 *
 * Parses the node comments.
 */
void CComment::Format_nodeComment(fstream &fileInput, list<CComment>& m_listComment)
{
    char *pcToken, *pcLine;
    char acLine[defCON_MAX_LINE_LEN];
    string comment;

    //Reads all the node comments,parses the comments and stores them to a list.
    while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_DESC_NODE]\n")!=0)
    {
        pcLine=acLine;
        //reads the node name.
        pcToken=strtok(pcLine," ");
        m_elementName=pcToken;

        //m_elementName = m_elementName.Left(defCON_MAX_MSGN_LEN);
        //get the comment.
        pcToken = strtok(NULL,"");
        comment = pcToken;
        while(strstr(pcToken,"\";") == NULL)
        {
            //parses the comment.
            fileInput.getline(acLine,defCON_MAX_LINE_LEN);
            pcToken = acLine;
            comment += pcToken;
        }
        m_comment= comment;
        //adds the comment to the list.
        m_listComment.push_back(*this);
    }
}

/**
 * \brief     Format message comments
 * \param[in] fileInput Input file
 * \param[in] m_listComment List of CComments
 *
 * Parses the message comments.
 */
void CComment::Format_msgComment(fstream &fileInput, list<CComment>& m_listComment)
{
    char *pcToken, *pcLine;
    char acLine[defCON_MAX_LINE_LEN];
    string comment;
    //Reads all the mesg comments,parses the comments and stores them to a list.
    while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_DESC_MSG]\n")!=0)
    {
        pcLine=acLine;
        //get msgid
        pcToken=strtok(pcLine," ");
        m_msgID=unsigned int(atoi(pcToken));
        //get the message type and validates the message.
        pcToken=strtok(NULL," ");
        m_msgType=*pcToken;
        if(m_msgType =='X')
            m_msgID -=2147483648;
        //get the comment
        pcToken = strtok(NULL,"");
        comment = pcToken;
        while(strstr(pcToken,"\";") == NULL)
        {   //parses the comment
            fileInput.getline(acLine,defCON_MAX_LINE_LEN);
            pcToken = acLine;
            comment += pcToken;
        }
        m_comment= comment;
        //adds the comment to the list.
        m_listComment.push_back(*this);
    }
}

/**
 * \brief     Format signal comments
 * \param[in] fileInput Input file
 * \param[in] m_listComment List of CComments
 *
 * Parses the signal comments.
 */
void CComment::Format_sigComment(fstream &fileInput, list<CComment>& m_listComment)
{
    char *pcToken, *pcLine;
    char acLine[defCON_MAX_LINE_LEN];
    string comment;
    //Reads all the signal comments,parses the comments and stores them to a list.
    while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_DESC_SIG]\n")!=0)
    {
        pcLine=acLine;
        //get the mesgid.
        pcToken=strtok(pcLine," ");
        m_msgID=unsigned int(atoi(pcToken));
        //get the mesg type and validates.
        pcToken=strtok(NULL," ");
        m_msgType=*pcToken;
        if(m_msgType =='X')
            m_msgID -=2147483648;
        //get signal name.
        pcToken = strtok(NULL," ");
        m_elementName=pcToken;

        //	m_elementName = m_elementName.Left(defCON_MAX_MSGN_LEN);
        //get comment.
        pcToken = strtok(NULL,"");
        comment = pcToken;
        while(strstr(pcToken,"\";") == NULL)
        {
            fileInput.getline(acLine,defCON_MAX_LINE_LEN);
            pcToken = acLine;
            comment += pcToken;
        }
        m_comment= comment;
        //adds the comment to list.
        m_listComment.push_back(*this);
    }
}
