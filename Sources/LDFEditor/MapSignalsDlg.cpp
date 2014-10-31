#include "MapSignalsDlg.h"
#include "LDFDatabaseManager.h"
#include "IClusterProps.h"
#include "LDFUtility.h"
#include "Defines.h"
#include "qdialogbuttonbox.h"
#include <sstream>      // std::ostringstream
#include <fstream>
#include "qmessagebox.h"
#include "qlineedit.h"
#include "qcheckbox.h"
#include "LDFTagDefines.h"

#define defCOL_SIGNAL_NAME 0
#define defCOL_START_BIT   1
#define defCOL_LENGTH      2

MapSignalsDlg::MapSignalsDlg(UID_ELEMENT uidEcu, int nFrameLen, std::map<UID_ELEMENT, SignalInstanse> mapSignals, QWidget* parent)
{
    IElement* pElement = nullptr;
    m_pouEcu = nullptr;
    m_nFrameLen = nFrameLen;
    if(EC_SUCCESS == LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->GetElement(eEcuElement, uidEcu, &pElement))
    {
        m_pouEcu = (IEcu*)pElement;
    }

    m_mapSignalUIDSigInst = mapSignals;
    ui.setupUi(this);



    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onSelectionOk()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onSelectionCancel()));

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Signals.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Signals.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Signals.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);
    std::string strEcuName;
    m_pouEcu->GetName(strEcuName);
    setWindowTitle(tr("Signals Published by ECU - %1").arg(strEcuName.c_str()));

    SetUpUi();
}


MapSignalsDlg::~MapSignalsDlg(void)
{
}

void MapSignalsDlg::SetUpUi()
{
    std::list<IFrame*> lstFrames;
    std::map<ISignal*, ISignal*> mapSignals;
    std::string strSignal;
    SignalProps ouSigProps;
    QVariant qVariant;
    UID_ELEMENT uidSignal;
    ui.tableSignals->setRowCount(0);
    std::map<UID_ELEMENT, UID_ELEMENT> mapTempSignals;

    if(nullptr != m_pouEcu)
    {
        ((IEcu*)m_pouEcu)->GetSignalList(eTx, mapSignals);

        int nRowCount = 0;
        //for(auto itrFrame : lstFrames)
        {
            //itrFrame->GetSignalList(mapSignals);

            ui.tableSignals->setRowCount(mapSignals.size());
            QList<QVariant> ouColumns;
for(auto itrSignal : mapSignals)
            {
                uidSignal = itrSignal.first->GetUniqueId();
                if(mapTempSignals.find(uidSignal) == mapTempSignals.end())
                {
                    strSignal = "";
                    itrSignal.first->GetName(strSignal);

                    itrSignal.first->GetProperties(ouSigProps);

                    QIntValidator* pSigStartBitValidator = new QIntValidator();
                    pSigStartBitValidator->setRange(0, 63);

                    QLineEdit* pRawEdit = new QLineEdit(nullptr);
                    pRawEdit->setValidator(pSigStartBitValidator);
                    pRawEdit->setFrame(false);

                    ouColumns.push_back(QVariant(strSignal.c_str()));
                    ouColumns.push_back(QVariant());
                    ouColumns.push_back(QVariant(GetString(ouSigProps.m_ouLINSignalProps.m_nLength, 10)));

                    qVariant.setValue(itrSignal.first->GetUniqueId());

                    ui.tableSignals->InsertRow(nRowCount, ouColumns);

                    ui.tableSignals->item(nRowCount, defCOL_SIGNAL_NAME)->setData(Qt::UserRole, qVariant);

                    ui.tableSignals->item(nRowCount, defCOL_SIGNAL_NAME)->setCheckState(Qt::Unchecked);
                    auto itrMap = m_mapSignalUIDSigInst.find(itrSignal.first->GetUniqueId());
                    if( itrMap != m_mapSignalUIDSigInst.end())
                    {
                        ui.tableSignals->item(nRowCount, defCOL_SIGNAL_NAME)->setCheckState(Qt::Checked);
                        pRawEdit->setText(GetString(itrMap->second.m_nStartBit, 10));
                    }

                    ui.tableSignals->setCellWidget(nRowCount++, defCOL_START_BIT, pRawEdit);
                    ouColumns.clear();
                    mapTempSignals[uidSignal] = uidSignal;
                }
            }
        }
    }
}

void MapSignalsDlg::onSelectionOk()
{
    std::string ouErrString = "";

    if(EC_FAILURE == vCollectSelectedSignals(ouErrString))
    {
        m_bIsError = true;

        QMessageBox omMsgBox;
        omMsgBox.setText(ouErrString.c_str());
        omMsgBox.exec();
    }
    else
    {
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        accept();
    }
}

void MapSignalsDlg::onSelectionCancel()
{
    reject();
}

ERRORCODE MapSignalsDlg::vCollectSelectedSignals(std::string& ouErrorString)
{
    ouErrorString = "";
    m_mapSignalUIDSigInst.clear();
    //QTableWidgetItem *pItem = nullptr;
    QLineEdit*  pStartBit = nullptr;
    QVariant qVariant;
    SignalInstanse ouSigInstance;
    UID_ELEMENT uidSig;
    //FrameProps ouFrameProps;
    int nFrameLengthInBits;
    std::string strSB;

    nFrameLengthInBits = (m_nFrameLen * 8);
    for(unsigned int unIndex = 0; unIndex < ui.tableSignals->rowCount(); unIndex++)
    {
        //pItem = ui.tableSignals->item(0,0);
        pStartBit = (QLineEdit*)ui.tableSignals->cellWidget(unIndex, defCOL_START_BIT);

        if(nullptr != pStartBit)
        {
            if(ui.tableSignals->item(unIndex, defCOL_SIGNAL_NAME)->checkState() == Qt::Checked)
            {
                qVariant = ui.tableSignals->item(unIndex, defCOL_SIGNAL_NAME)->data(Qt::UserRole);
                ouSigInstance.m_nStartBit =  GetUnsignedInt(pStartBit->text(), 10);
                uidSig = qVariant.value<UID_ELEMENT>();
                m_mapSignalUIDSigInst[uidSig] = ouSigInstance;

                // Validate Signal Start Bit
                if(EC_FAILURE == vValidateSignalStartBit(uidSig, nFrameLengthInBits,  pStartBit->text(), ouErrorString))
                {
                    return EC_FAILURE;
                }
            }
        }
    }

    ouErrorString = "";
    // Validate if Signal overlapping exists
    if(EC_FAILURE == nValidateSignalOverlapping(nFrameLengthInBits, ouErrorString))
    {
        return EC_FAILURE;
    }

    return EC_SUCCESS;
}

void MapSignalsDlg::vGetSelectedSignals(std::map<UID_ELEMENT, SignalInstanse>& mapSignals)
{
    mapSignals = m_mapSignalUIDSigInst;
}

ERRORCODE MapSignalsDlg::vValidateSignalStartBit(UID_ELEMENT uidSignal, int nFrameLengthInBits, QString strSigSB, std::string& ouErrorString)
{
    IElement* pSignal = nullptr;

    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->GetElement(eSignalElement, uidSignal, &pSignal);
    SignalProps omSigProps;
    std::string omSignalName;

    if(nullptr != pSignal)
    {
        pSignal->GetName(omSignalName);
        ((ISignal*)pSignal)->GetProperties(omSigProps);

        int nStartBit = m_mapSignalUIDSigInst[uidSignal].m_nStartBit;
        int nEndBit = nStartBit + (omSigProps.m_ouLINSignalProps.m_nLength-1);

        if(strSigSB.isEmpty() == true)
        {
            std::ostringstream omStringStrm;

            omStringStrm <<defERROR << omSignalName.c_str() << defERR_EMPTY_STARTBIT;

            ouErrorString = omStringStrm.str();

            return EC_FAILURE;
        }

        // Step1: Validate Signal StartBit
        if(nStartBit < 0 || nStartBit > defMAX_SIGNAL_STARTBIT)
        {
            std::ostringstream omStringStrm;

            omStringStrm <<defERROR << omSignalName.c_str() << defSIGNAL_STARTBIT_INVALID << nStartBit;

            ouErrorString = omStringStrm.str();

            return EC_FAILURE;
        }

        // Step2: Validate Signal startbit if exceeds frame length
        if(nStartBit >= nFrameLengthInBits)
        {
            std::ostringstream omStringStrm;
            omStringStrm <<defERROR << omSignalName.c_str() << defERR_SIGNAL_STARTBIT << nStartBit
                         << defERR_FRAME_LENGTH << (nFrameLengthInBits/8) << defBYTES;
            ouErrorString = omStringStrm.str();
            return EC_FAILURE;
        }

        if(nEndBit >= nFrameLengthInBits)
        {
            std::ostringstream omStringStrm;
            omStringStrm <<defERROR << omSignalName.c_str() << defERR_SIGNAL_OFFSET << nStartBit << defHIPHEN << nEndBit
                         << defERR_FRAME_LENGTH << (nFrameLengthInBits/8) << defBYTES;
            ouErrorString = omStringStrm.str();
            return EC_FAILURE;
        }
    }

    return EC_SUCCESS;
}

ERRORCODE MapSignalsDlg::nValidateSignalOverlapping(int nFrameLength, std::string& ouErrorString)
{
    int nArrSignalBit[64];
    int nSignalEndBit;
    UID_ELEMENT uidSignal;
    memset(nArrSignalBit, 0, sizeof(nArrSignalBit));
    std::ostringstream omStringStrm;
    std::string omSignalName;
    QLineEdit* pItem = nullptr;
    QString omSigName = "";
    int nCount = ui.tableSignals->rowCount();
    for(int nIndex = 0; nIndex < nCount; nIndex++)
    {
        pItem = (QLineEdit*)ui.tableSignals->cellWidget(nIndex, defCOL_START_BIT);

        if(ui.tableSignals->item(nIndex, defCOL_SIGNAL_NAME)->checkState() == Qt::Checked)
        {
            uidSignal = ui.tableSignals->item(nIndex, defCOL_SIGNAL_NAME)->data(Qt::UserRole).value<UID_ELEMENT>();
            omSigName = ui.tableSignals->item(nIndex, defCOL_SIGNAL_NAME)->text();
            int nStartBit = GetUnsignedInt(pItem->text(), 10);

            std::string strName = omSigName.toStdString();
            int nLength = GetUnsignedInt(ui.tableSignals->item(nIndex, defCOL_LENGTH)->text(), 10);

            // Signal End bit
            nSignalEndBit = nStartBit + (nLength-1);

            for(int nStartIndex = nStartBit; nStartIndex <= nSignalEndBit; nStartIndex++)
            {
                if(nArrSignalBit[nStartIndex] == 1)
                {
                    omStringStrm << defSIGNAL << "<B>" << omSigName.toStdString() << "</B>" << defSIGNAL_OVERLAPPING;
                    ouErrorString =  omStringStrm.str();
                    return EC_FAILURE;
                }

                nArrSignalBit[nStartIndex] = 1;
            }
        }
    }

    return EC_SUCCESS;
}