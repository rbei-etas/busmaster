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
 * \file      Comment.h
 * \brief     CComment class.
 * \author    Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CComment class.
 */

#pragma once

/* C++ includes */
#include <fstream>
#include <list>
#include <string>

using namespace std;

class CComment
{

public:
    //data members
    unsigned int m_msgID;
    char m_msgType;
    string m_comment;
    string m_elementName;

    //member functions
    CComment();
    CComment& operator=(CComment& tCmt);
    virtual ~CComment();
    void Format_netComment(fstream &fileInput, list<CComment>& m_listComment);
    void Format_nodeComment(fstream &fileInput, list<CComment>& m_listComment);
    void Format_msgComment(fstream &fileInput, list<CComment>& m_listComment);
    void Format_sigComment(fstream &fileInput, list<CComment>& m_listComment);
};
