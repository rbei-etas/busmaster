#pragma once
#include <QObject>
#include <QStringList>
#include <QLibrary>

#include "ICluster.h"

class LDFDatabaseManager : public QObject
{
    Q_OBJECT
private:

    typedef ERRORCODE (*ParseDBFile)(std::string& strFileName, std::list<ClusterResult>& ouClusterResultList);
    typedef ERRORCODE (*DeleteCluster)(ICluster* pCluster);
    typedef ERRORCODE (*CreateLDFCluster)(ICluster** pouLdfCluster);

    CreateLDFCluster m_pfCreateLDFCluster;
    QLibrary mDbManagerDll;
    ICluster* m_pouLDFClsuter;
    std::string m_strCurrentLDFFilePath;
    bool m_bDisplayInHex;
    QStringList m_strRecentFileList;
    bool m_bDocumentModified;
signals:
    void DataUpdated();
    void DisplayModeChanged();
    void DocumentModified();
public:
    LDFDatabaseManager(void);
    ~LDFDatabaseManager(void);

    int SetLDFFilePath(const std::string &strFilePath);
    std::string GetLDFFilePath();
    int LoadLDFFilePath(std::string& strFilePath);
    ICluster* GetLDFCluster();


    bool bIsDocumentModified();
    void setDocumentModified(bool bModified);

    bool bIsDisplayHexOn();
    void SetDisplayHex(bool);

    void GetRecentFileList(QStringList& strRecentFileList);
    int GetMaxRecentFileCount();

    void saveLDFSettings();
    void readLDFSettings();

    // void ValidateFile();

    static LDFDatabaseManager* GetDatabaseManager();
    static void DeleteDatabaseManager();
    static LDFDatabaseManager* m_pouLdfDatabaseManager;
};

