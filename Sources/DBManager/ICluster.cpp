#include "stdafx.h"
#include "ICluster.h"

IElement::IElement(ICluster* pCluster)
{
    m_pBaseCluster = pCluster;
    m_pBaseCluster->GetNextUniqueId(m_nUnId);
    m_oueElementType = eInvalidElement;
}

ERRORCODE IElement::GetName(std::string& strName)
{
    strName = m_strName;
    return EC_SUCCESS;
}

ERRORCODE IElement::SetName(std::string& strName)
{
    //if ( m_strName != strName )
    {
        eNameChangeActionData ouData;
        ouData.m_strNewName = strName;
        ouData.m_strOldName = m_strName;

        m_strName = strName;
        if( nullptr != m_pBaseCluster )
        {
            m_pBaseCluster->NotifyClusterChange(eNameChange, this->m_nUnId, m_oueElementType, &ouData);
        }
        return EC_SUCCESS;
    }
    return EC_SUCCESS;
}

ERRORCODE IElement::GetUniqueId(UID_ELEMENT& unId)
{
    unId = m_nUnId;
    return EC_SUCCESS;
}

UID_ELEMENT IElement::GetUniqueId()
{
    return m_nUnId;
}





eClusterElementType IElement::GetElementType()
{
    return m_oueElementType;
}

ERRORCODE IElement::SetElementType(eClusterElementType& ouElementType)
{
    m_oueElementType = ouElementType;
    return EC_SUCCESS;
}

