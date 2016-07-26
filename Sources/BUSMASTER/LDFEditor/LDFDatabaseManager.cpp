#include "LDFDatabaseManager.h"
#include "qsettings.h"

LDFDatabaseManager* LDFDatabaseManager::m_pouLdfDatabaseManager = nullptr;

LDFDatabaseManager::LDFDatabaseManager(void)
{
    m_pouLDFClsuter = nullptr;
    m_pouLdfDatabaseManager = nullptr;
    m_strCurrentLDFFilePath = "";
    m_bDisplayInHex = 0;
    readLDFSettings();
    m_bDocumentModified= false;
    m_pfCreateLDFCluster = nullptr;
}

bool LDFDatabaseManager::bIsDocumentModified()
{
    return m_bDocumentModified;
}

void LDFDatabaseManager::setDocumentModified(bool bModified)
{
    m_bDocumentModified = bModified;
    emit DocumentModified();
}


void LDFDatabaseManager::DeleteDatabaseManager()
{
    if ( nullptr != m_pouLdfDatabaseManager )
    {
        delete m_pouLdfDatabaseManager;
        m_pouLdfDatabaseManager = nullptr;
    }
}

bool LDFDatabaseManager::bIsDisplayHexOn()
{
    return m_bDisplayInHex;
}

void LDFDatabaseManager::SetDisplayHex(bool bMode)
{
    m_bDisplayInHex = bMode;
    emit DisplayModeChanged();
}



LDFDatabaseManager::~LDFDatabaseManager(void)
{
    saveLDFSettings();
}


void LDFDatabaseManager::saveLDFSettings()
{
    QSettings settings("RBEI-ETAS", "BUSMASTER_LDFEditor");

    settings.beginGroup("LDFUISettings");
    settings.setValue("hexMode", m_bDisplayInHex);

    settings.setValue("recentFileList", m_strRecentFileList);


    settings.endGroup();
}

void LDFDatabaseManager::readLDFSettings()
{
    QSettings settings("RBEI-ETAS", "BUSMASTER_LDFEditor");

    settings.beginGroup("LDFUISettings");

    m_bDisplayInHex = settings.value("hexMode").value<bool>();

    m_strRecentFileList.clear();
    m_strRecentFileList = settings.value("recentFileList").toStringList();

    settings.endGroup();
}


LDFDatabaseManager* LDFDatabaseManager::GetDatabaseManager()
{
    if ( nullptr == m_pouLdfDatabaseManager )
    {
        m_pouLdfDatabaseManager = new LDFDatabaseManager();
    }
    return m_pouLdfDatabaseManager;
}


ICluster* LDFDatabaseManager::GetLDFCluster()
{
    if (nullptr == m_pfCreateLDFCluster)
    {
        mDbManagerDll.setFileName("DBManager");
        m_pfCreateLDFCluster = (CreateLDFCluster)mDbManagerDll.resolve("CreateLDFCluster");
        if (nullptr == m_pfCreateLDFCluster)
        {
            return nullptr;
        }
    }
    if ( nullptr == m_pouLDFClsuter )
    {
        m_pfCreateLDFCluster(&m_pouLDFClsuter);
    }
    return m_pouLDFClsuter;
}

int LDFDatabaseManager::SetLDFFilePath(const std::string& strFilePath)
{
    m_strCurrentLDFFilePath = strFilePath;

    if ( strFilePath != "" )
    {
        auto itr = m_strRecentFileList.removeAll(strFilePath.c_str());
        m_strRecentFileList.prepend(strFilePath.c_str());
    }
    emit DataUpdated();
    return 0;
}

int LDFDatabaseManager::GetMaxRecentFileCount()
{
    return 5;
}


void LDFDatabaseManager::GetRecentFileList(QStringList& strRecentFileList)
{
    strRecentFileList = m_strRecentFileList;
}


int LDFDatabaseManager::LoadLDFFilePath(std::string& strFilePath)
{
    //GetLDFCluster()->LoadFromFile(strFilePath);
    //SetLDFFilePath(strFilePath);
    return 0;
}

std::string LDFDatabaseManager::GetLDFFilePath()
{
    return m_strCurrentLDFFilePath;
}

/*
void LDFDatabaseManager::ValidateFile()
{
    const char* pchTempFileName = tmpnam(NULL);  // Get temp name
    FILE* fp = fopen(pchTempFileName, "w");  // Create the file

    GetLDFCluster()->SaveToFile(std::string(pchTempFileName));

    ICluster* pouLDFClsuter;
    CreateLDFCluster(&pouLDFClsuter);
    std::list<ParsingResults> ouWarnings;
    std::list<ParsingResults> ouErrors;
    pouLDFClsuter->LoadFromFile(std::string(pchTempFileName), ouErrors, ouWarnings);

    if ( nullptr
    fclose(fp);
    remove(pchTempFileName);
}



*/
