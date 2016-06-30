#pragma once

#include "qtablewidget.h"
#include "qfiledialog.h"
#include "ICluster.h"
#include "LDFDatabaseManager.h"
#include <qprocess>
#include <qtemporaryfile>
#define VALIDATE_AND_RETURN(x)  if ( nullptr == x ) return EC_FAILURE;
#define VALIDATE_POINTER(x)     if ( nullptr == x ) return;

#define defTREE_ITEM_LINCLUSTER         0
#define defTREE_ITEM_MASTER             1
#define defTREE_ITEM_SLAVE              2
#define defTRRE_ITEM_UNCONDITIONAL      3
#define defTREE_ITEM_SPORADIC           4
#define defTREE_ITEM_EVENT_TRIGGERED    5
#define defTREE_ITEM_DIAGNOSTIC         6
#define defTREE_ITEM_NORMAL_SIGNAL      7
#define defTREE_ITEM_DIAG_SIGNAL        8
#define defTREE_ITEM_CODING             9
#define defTREE_ITEM_SCHEDULETABLE      10
#define defTREE_ITEM_SIGNALGROUP        11
#define defTREE_ITEM_NODECOMP           12
#define defIdentifier_RegExp            "[a-zA-Z_][a-zA-Z0-9_]*"
#define defIdentifier_RegExp_MULTIPLE   "([a-zA-Z_][a-zA-Z0-9_]*,)+"
#define defUnit_RegExp                  "[^\"]*"
#define defHex_RegExp                   "(0x)?[0-9A-Fa-f][0-9A-Fa-f]*"



enum eTreeElementType
{
    eTreeHeader,
    eLdfItem
};

class TreeItemID
{

public:
    eTreeElementType m_eTreeElementType;
    UID_ELEMENT m_uidElement; // Tree Element Type
};

Q_DECLARE_METATYPE(TreeItemID);


class QLDFElementDialog : public QDialog
{
public:
    QLDFElementDialog(QWidget* pParent);
    void accept();
};


class QLDFTableWidgetItem: public QTableWidgetItem
{
public:
    QLDFTableWidgetItem(QString strName);
};

class QLDFViewerProcess:public QProcess
{
    Q_OBJECT;
    QTemporaryFile* m_ouTempFile;
public:
    QLDFViewerProcess();
    void start();
public slots:
    void OnProcessExit();
};



QString toString( QVariant& ouVariant );


int GetString(eFrameType oueFrameType, std::string& strString);
int GetString(eDiagType oueLinDiagType, std::string& strString);

int GetString(double dVal, QString& strText);
int GetString(int nVal, QString& strText );
unsigned int GetUnsignedInt(const QString& strText);
unsigned int GetUnsignedInt(QString& strText, int nBase);

QString GetString(int nVal, bool bPrintHex = true);
QString GetString(unsigned int nVal,bool bPrintHexSymbol = true);
QString GetString(double dVal);
QString GetString(unsigned __int64 nVal, bool bPrintHexSymbol = true);

QString GetString(int nVal, int nBase);
QString GetString(unsigned int nVal, int nBase);
QString GetString(unsigned __int64 nVal, int nBase);


int GetType(std::string strString, eDiagType& ouDiagType);

unsigned char ucCalculatePID(unsigned char ucId);

int CreateDefaultItems(double fLDFVersion);
int CreateDefaultPropertySettings(double fLDFVersion);
int CreateDefauleNodeItems(double fLDFVersion);
int CreateMasterDiagFrame(IFrame*& pouFrame, ICluster* pBaseCluster);
int CreateSlaveDiagFrame(IFrame*& pouFrame, ICluster* pBaseCluster);
bool bAreEqual(double a, double b);
ERRORCODE nGetSubscribers(std::list<IEcu*> ouECUlst, std::string& strRxEcus);