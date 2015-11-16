#pragma once
#include <qobject>
#include "ICluster.h"
#include "qstringlist.h"


class LDFDatabaseManager : public QObject
{
    Q_OBJECT
private:
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

    int SetLDFFilePath(std::string& strFilePath);
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

    //void ValidateFile();

    static LDFDatabaseManager* GetDatabaseManager();
    static void DeleteDatabaseManager();
    static LDFDatabaseManager* m_pouLdfDatabaseManager;
};

